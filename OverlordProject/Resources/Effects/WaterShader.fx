//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;
float4x4 gMatrixViewInverse : VIEWINVERSE;
float4 gColorDiffuse : COLOR = float4(1.0, 1.0, 1.0, 1.0);
float time : TIME;

//Fresnel variables
float gFresnelHardness = 10;
float gFresnelPower = 10;
float gFresnelMultiplier = 10;
float gOpacity = 0.6f;

//Light
float3 m_LightDirection : DIRECTION
<
	string UIName = "LightDirection";
	string Object = "TargetLight";
> = float3(-.577f, -.577f, .577f);

Texture2D gWaterTexture
<
	string UIName = "Water Texture";
	string UIWidget = "Texture";
>;

Texture2D m_opacityTexture
<
	string UIName = "Opacity Texture";
	string ResourceType = "Texture";
>;

Texture2D gFoamTexture
<
	string UIName = "Foam Texture";
	string UIWidget = "Texture";
>;

Texture2D gNormalMapTexture
<
	string UIName = "Normal Map Texture";
	string UIWidget = "Texture";
>;

Texture2D gNoiseTexture
<
	string UIName = "Noise Texture";
	string UIWidget = "Texture";
>;

//Blend State to use Alpha Blending
BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
    SrcBlend = SRC_ALPHA;
};
SamplerState gWaterSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
};

float waveAmplitude
<
	string UIName = "Wave Amplitude";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 5;
	float UIStep = 0.1f;
> = 1.9f;

float waveFrequency
<
	string UIName = "Wave Frequency";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 5;
	float UIStep = 0.1f;
> = 0.1f;

float2 gWaterMovementSpeed = float2(0.1f, 0.05f);
float4 gFoamColor = float4(1.0, 1.0, 1.0, 1.0); // Foam color
float gFoamThreshold = 0.5; // Foam texture threshold
float2 gNormalMapStrength = float2(1.0, 1.0); // Normal map strength
float2 gNoiseScale = float2(0.2, 0.2); // Noise texture scale
float gNoiseStrength = 0.1; // Noise strength

struct VS_DATA
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct PS_DATA
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
};

//STATES
//******
RasterizerState gRS_NoCulling
{
    CullMode = NONE;
};


//MAIN VERTEX SHADER
//******************
PS_DATA MainVS(VS_DATA input)
{
    PS_DATA output = (PS_DATA) 0;
    output.position = mul(float4(input.position, 1.0f), gMatrixWorldViewProj);
    output.uv = input.uv;
	
	// Offset the position based on the vertex index
    float offset = sin(input.position.x + waveAmplitude + time) * waveFrequency; // Adjust the offset as needed
    float offset1 = sin(input.position.z + waveAmplitude + time) * waveFrequency;
    output.position.y += offset + offset1;
	
    output.normal = normalize(mul(float4(input.normal, 0.0f), gMatrixWorld));
    output.worldPosition = mul(float4(input.position, 1.0f), gMatrixWorld);
    return output;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(PS_DATA input) : SV_Target
{
	//Fresnel calculations
    float3 cameraPosition = float3(gMatrixViewInverse[3][0], gMatrixViewInverse[3][1], gMatrixViewInverse[3][2]);
    float3 viewVector = normalize(cameraPosition - input.worldPosition);
	
    float fresnel = 1 - saturate(abs(dot(input.normal, viewVector)));
    fresnel = pow(fresnel, gFresnelPower);
    fresnel *= gFresnelMultiplier;
	
    float fresnelMask = 1 - saturate(dot(float3(0, -1, 0), input.normal));
    fresnelMask = pow(fresnelMask, gFresnelHardness);
	
    float lightValue = max(dot(input.normal, -m_LightDirection), 0.0f);
	
    float2 uv = input.uv;
    uv += time * gWaterMovementSpeed.y;
    uv += time * gWaterMovementSpeed.x;
    float4 waterColor = gWaterTexture.SampleLevel(gWaterSampler, uv, 0) * lightValue;
    waterColor *= fresnel * fresnelMask;
    return waterColor;
}


//TECHNIQUES
//**********
technique11 DefaultTechnique
{
    pass p0
    {
        SetRasterizerState(gRS_NoCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}