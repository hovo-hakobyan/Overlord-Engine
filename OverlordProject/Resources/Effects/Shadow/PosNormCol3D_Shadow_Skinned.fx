float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-1.377f, -1.377f, 1.477f);
float gShadowMapBias = 0.01f;
float4x4 gBones[70];

Texture2D gShadowMap;

SamplerComparisonState cmpSampler
{
	// sampler state
    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;
    AddressV = MIRROR;

	// sampler comparison state
    ComparisonFunc = LESS_EQUAL;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap; // or Mirror or Clamp or Border
    AddressV = Wrap; // or Mirror or Clamp or Border
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 BoneIndices : BLENDINDICES;
    float4 BoneWeights : BLENDWEIGHTS;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 lPos : TEXCOORD1;
    float4 color : COLOR;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
    CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//TODO: complete Vertex Shader 
	//Hint: use the previously made shaders PosNormTex3D_Shadow and PosNormTex3D_Skinned as a guide
	
    float4 transformedPos = float4(0.f, 0.f, 0.f, 0.f);
    float3 transformedNormal = float3(0.f, 0.f, 0.f);

    for (int i = 0; i < 4; ++i)
    {
        float blendIdx = input.BoneIndices[i];
        if (blendIdx < 0)
        {
            continue;
        }
        float blendWeight = input.BoneWeights[i];

        transformedPos += mul(float4(input.pos, 1.0f), gBones[blendIdx]) * blendWeight;
        transformedNormal += normalize(mul(input.normal, (float3x3) gBones[blendIdx]) * blendWeight);

    }
    transformedPos.w = 1.0f;
    output.pos = mul(transformedPos, gWorldViewProj);

    output.normal = normalize(mul(transformedNormal, (float3x3) gWorld));
    output.texCoord = input.texCoord;
    output.color = input.Color;

    output.lPos = mul(transformedPos, gWorldViewProj_Light);
    return output;
}

float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)
    return float2(u, v);
}

float EvaluateShadowMap(float4 lpos)
{
	//re-homogenize position 
    lpos.xyz /= lpos.w;

	//if position is not visible to the light - dont illuminate it
	//results in hard light frustum
    if (lpos.x < -1.0f || lpos.x > 1.0f ||
		lpos.y < -1.0f || lpos.y > 1.0f ||
		lpos.z < 0.0f || lpos.z > 1.0f)
    {
        return 0.5f;
    }

	//Clip space to texture space
    lpos.x = lpos.x / 2 + 0.5f;
    lpos.y = lpos.y / -2 + 0.5f;

	//apply shadow map bias
    lpos.z -= gShadowMapBias;

    float shadowMapDepth = gShadowMap.Sample(samLinear, lpos.xy).r;

	//If shadow map value is less than clip space z, then there is an occluder -->SHADOW
    if (shadowMapDepth < lpos.z) 
        return 0.5f;

    return 1.0f;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float shadowValue = EvaluateShadowMap(input.lPos);

    float3 color_rgb = input.color.rgb;
    float color_a = input.color.a;
	
	//HalfLambert Diffuse :)
    float diffuseStrength = dot(input.normal, -gLightDirection);
    diffuseStrength = diffuseStrength * 0.5f + 0.5f;
    diffuseStrength = saturate(diffuseStrength);
    color_rgb = color_rgb * diffuseStrength;

    return float4(color_rgb * shadowValue, color_a);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
        SetRasterizerState(NoCulling);
        SetDepthStencilState(EnableDepth, 0);

        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}

