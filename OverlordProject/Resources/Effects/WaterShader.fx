//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4 gColorDiffuse : COLOR = float4(1.0, 1.0, 1.0, 1.0);

Texture2D gWaterTexture
<
	string UIName = "Water Texture";
	string UIWidget = "Texture";
>;

SamplerState gWaterSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
};
float time;

float waveAmplitude
<
	string UIName = "Wave Amplitude";
	string UIWidget = "slider";
> = 15.6f;

float waveFrequency
<
	string UIName = "Wave Frequency";
	string UIWidget = "slider";
> = 3.4f;

float2 gWaterMovementSpeed = float2(0.005f, 0.005f);

struct VS_DATA
{
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct PS_DATA
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
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
    float offset = sin(input.position.x * waveAmplitude + time) * waveFrequency;
    float offset1 = sin(input.position.z * waveAmplitude + time) * waveFrequency;
    output.position.y += offset + offset1;
    return output;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(PS_DATA input) : SV_Target
{
    float2 uv = input.uv;
    uv += time * gWaterMovementSpeed.y;
    uv += time * gWaterMovementSpeed.x;
    float4 waterColor = gWaterTexture.SampleLevel(gWaterSampler, uv, 0);
    return waterColor;
}


//TECHNIQUES
//**********
technique10 DefaultTechnique
{
    pass p0
    {
        SetRasterizerState(gRS_NoCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}