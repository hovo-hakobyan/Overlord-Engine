//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

RasterizerState gRS_BackCulling
{
    CullMode = BACK;
};


//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position, 1.0f);
    output.TexCoord = input.TexCoord;
	// Set the TexCoord
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    // Step 1: sample the texture
    float4 sample = gTexture.Sample(samPoint,input.TexCoord);
	// Step 2: calculate the mean value
    float meanValue = sample.x + sample.y + sample.z;
    meanValue /= 3.0f;
	// Step 3: return the color
	
    return float4( meanValue, meanValue, meanValue, 1.0f );
}


//TECHNIQUE
//---------
technique11 Grayscale
{
    pass P0
    {          
        // Set states...
        SetRasterizerState(gRS_BackCulling);
        SetDepthStencilState(EnableDepth, 0);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

