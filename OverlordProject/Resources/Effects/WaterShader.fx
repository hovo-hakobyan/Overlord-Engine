//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;
float4x4 gMatrixViewInverse : VIEWINVERSE;
float time : TIME;
float gPI = 3.14159265359f;

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


float waveAmplitude
<
	string UIName = "Wave Amplitude";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 5;
	float UIStep = 0.1f;
> = 0.05f;

float waveFrequency
<
	string UIName = "Wave Frequency";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 5;
	float UIStep = 0.1f;
> = 0.7f;

float2 gWaterMovementSpeed = float2(0.01f, 0.01f);

struct VS_DATA
{
    float3 position : POSITION0;
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
    float displacement = waveAmplitude * sin(2 * gPI * waveFrequency * (time + input.position.x));
    output.position.y += displacement;
	
    return output;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(PS_DATA input) : SV_Target
{
	
    float2 uv = input.uv;
    uv += time * gWaterMovementSpeed.y;
    float4 waterColor = gWaterTexture.SampleLevel(gWaterSampler, uv, 0);
    return waterColor;
}


//TECHNIQUES
//**********
technique11 DefaultTechnique
{
    pass p0
    {
        SetRasterizerState(gRS_NoCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}