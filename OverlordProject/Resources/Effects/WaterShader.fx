//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;

Texture2D gWaterTexture
<
	string UIName = "Water Texture";
	string UIWidget = "Texture";
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
	float UIMin = 0;
	float UIMax = 5;
	float UIStep = 0.1f;
> = 15.6f;

float waveFrequency
<
	string UIName = "Wave Frequency";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 5;
	float UIStep = 0.1f;
> = 3.4f;

float2 gWaterMovementSpeed = float2(0.005f, 0.005f);
float4 gFoamColor = float4(0.0980f, 0.7294f, 0.8706f, 1.0); // Foam color
float gFoamThreshold = 0.96; // Foam texture threshold
float2 gNormalMapStrength = float2(1.0f, 0.4f); // Normal map strength
float2 gNoiseScale = float2(1.9f, 0.3f); // Noise texture scale
float gNoiseStrength = 0.0f; // Noise strength

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
    float offset = sin(input.position.x * waveAmplitude + time) * waveFrequency; // Adjust the offset as needed
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
	
	//Foam
    float4 foam = gFoamTexture.Sample(gWaterSampler, uv);
    waterColor = lerp(waterColor, gFoamColor, step(gFoamThreshold, foam.r));
	
	//Normal map 
    float4 normalMap = gNormalMapTexture.Sample(gWaterSampler, uv);
    float2 normalOffset = normalize(normalMap.xy - 0.5f) * gNormalMapStrength;
    uv += normalOffset;
	
	//Noise
    float4 noise = gNoiseTexture.Sample(gWaterSampler, uv * gNoiseScale);
    waterColor.rgb += noise.rgb * gNoiseStrength;
	
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