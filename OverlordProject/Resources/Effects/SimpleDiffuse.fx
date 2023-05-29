//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;

float3 m_LightDirection : DIRECTION
<
	string UIName = "LightDirection";
	string Object = "TargetLight";
> = float3(-1.377f,-1.377f,1.477f);

Texture2D gTextureNormal
<
	string UIName = "Normal Texture";
	string UIWidget = "Texture";
>;

Texture2D gTextureDiffuse
<
	string UIName = "Diffuse Texture";
	string ResourceType = "Texture";
>;


SamplerState gDiffuseSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState gNormalMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

// Input structure for vertex shader
struct VSInput
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 normal : NORMAL;
};

// Output structure for vertex shader
struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 normal : NORMAL;
};

//STATES
//******
RasterizerState gRS_NoCulling { CullMode = NONE; };


//MAIN VERTEX SHADER
//******************
PSInput MainVS(VSInput vsInput)
{
	PSInput output = (PSInput)0;
	output.position = mul(float4(vsInput.position,1),gMatrixWorldViewProj);
	output.texCoord = vsInput.texCoord;
	output.tangent = mul(vsInput.tangent,(float3x3)gMatrixWorld);
	output.normal = mul(vsInput.normal,(float3x3)gMatrixWorld);
	

	return output;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(PSInput input) : SV_TARGET
{
	
	float3 normal = normalize(input.normal);
	float3 tangent = normalize(input.tangent);
	
	//Step2 
	float3 binormal = normalize(cross(tangent,normal));
	
	//Step3
	//Matrix consisting of thetangent, binoramal and normal
	float3x3 localAxis = float3x3(tangent,binormal,normal);
	
	//Sample from texture and convert to [0,1] range
	float3 sampledNormal = gTextureNormal.Sample(gNormalMapSampler, input.texCoord).xyz * 2 -1;
	
	//Transform sampled normal with localAxis
	float3 newNormal = normalize(mul(sampledNormal,localAxis));

	//step 5
	float diffuseLightValue = max(dot(-newNormal,m_LightDirection),0);
	
	float3 diffuse = gTextureDiffuse.Sample(gDiffuseSampler,input.texCoord) * diffuseLightValue;
	
	return float4(diffuse,1.0f);
}


//TECHNIQUES
//**********
technique11 DefaultTechnique {
	pass p0 {
		SetRasterizerState(gRS_NoCulling);	
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}