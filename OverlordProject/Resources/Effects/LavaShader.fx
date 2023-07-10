//DX10 - FLAT SHADER
//Digital Arts & Entertainment

//CREDITS
//https://www.patreon.com/posts/stylized-lava-32245619

//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;
float gTime : TIME;


float4 gTintStart
<
	string UIName = "Main Tint Start";
	string UIWidget = "Color";
> = float4(0.5f,0.1f,0.1f,1.0f);


float4 gTintEnd
<
	string UIName = "Main Tint End";
	string UIWidget = "Color";
> = float4(0.6f,0.4f,0.1f,1.0f);

float gOffset
<
	string UIName = "Offset";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 0.7f;

float gSpeed
<
	string UIName = "Lava Speed";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 0.1f;

float gVertexDistortion
<
	string UIName = "Vertex Distortion";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 0.2f;

float gAmount
<
	string UIName = "Amount";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 3.7f;

float gDistortion
<
	string UIName = "Distortion";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 0.3f;

float gScale
<
	string UIName = "Scale";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 0.03f;

float gStrength
<
	string UIName = "Strength";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 1.4f;

float gScaleDist
<
	string UIName = "Scale Distortion";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 0.03f;

float gHeight
<
	string UIName = "Height";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 10.0f;
	float UIStep = 0.1f;
> = 0.1f;



float gSpeedDistortX
<
	string UIName = "Speed Distort X";
	string UIWidget = "slider";
	float UIMin = 1.f;
	float UIMax = 40.0f;
	float UIStep = 0.5f;
> = 0.01f;

float gSpeedDistortY
<
	string UIName = "Speed Distort Y";
	string UIWidget = "slider";
	float UIMin = 1.f;
	float UIMax = 40.0f;
	float UIStep = 0.5f;
> = 0.01f;

float gSpeedMainX
<
	string UIName = "Speed Main X";
	string UIWidget = "slider";
	float UIMin = 1.f;
	float UIMax = 40.0f;
	float UIStep = 0.5f;
> = 0.05;

float gSpeedMainY
<
	string UIName = "Speed Main Y";
	string UIWidget = "slider";
	float UIMin = 1.f;
	float UIMax = 40.0f;
	float UIStep = 0.5f;
> = 0.07f;


Texture2D gTex0
<
	string UIName = "Noise Texture";
	string UIWidget = "Texture";
>;

Texture2D gTex1
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Texture";
>;

SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

struct VS_INPUT
{
	float4 vertex : POSITION;
	float2 uv : TEXCOORD0;
	float4 color: COLOR;
};

struct VS_OUTPUT
{
	float2 uv: TEXCOORD3;
	float4 vertex: SV_POSITION;
	float4 scrPos : TEXCOORD2; //for depth
	float4 worldPos: TEXCOORD4; // for textures
	float4 color: COLOR0;
};


//STATES
//******
RasterizerState gRS_NoCulling { CullMode = NONE; };


//MAIN VERTEX SHADER
//******************
VS_OUTPUT MainVS(VS_INPUT v)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	v.vertex.y += (sin(gTime * gSpeed + (v.vertex.x * v.vertex.z * gAmount)) * gHeight) * v.color.r;
    v.vertex.w = 1.0f;
	output.vertex = mul(v.vertex,gMatrixWorldViewProj);
	output.worldPos = mul(v.vertex,gMatrixWorld);
	output.color = v.color;
	output.uv = v.uv;
	
	float3 ndcPos = output.vertex / output.vertex;
	output.scrPos = float4((ndcPos.x + 1)*0.5f * 1280, (ndcPos.y + 1) * 0.5f * 720,output.vertex.z,output.vertex.w);
	
	return output;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(VS_OUTPUT i) : SV_Target
{
	float4 color = float4(0,0,0,1);
	
	float2 uvDistort = i.worldPos.xz * gScaleDist;
	float speedDistortX = gTime * gSpeedDistortX;
	float speedDistortY = gTime * gSpeedDistortY;
	
	float2 speedDistortCombined = float2(speedDistortX,speedDistortY);
	
	//Distortion textures at different scales
	float d = gTex0.Sample(gTextureSampler,uvDistort + speedDistortCombined).r;
	float d2 = gTex0.Sample(gTextureSampler,(i.worldPos.xz * (gScaleDist * 0.5f)) * speedDistortCombined).r;
	
	float layeredDist = saturate((d + d2) * 0.5f);
	
	//main uv scaled
	float2 uvMain = i.worldPos.xz * gScale;
	
	//distortion added
	uvMain += layeredDist * gDistortion;
	
	//moving over time
	float speedMainX = gTime * gSpeedMainX;
	float speedMainY = gTime * gSpeedMainY;
	float2 speedMainCombined = float2(speedMainX,speedMainY);
	
	uvMain += speedMainCombined + (i.color.r * gVertexDistortion);
	
	float4 tempCol = gTex1.Sample(gTextureSampler,uvMain);
	tempCol += layeredDist;
	
	color = lerp(gTintStart,gTintEnd,tempCol * gOffset) * gStrength;
	return color;

}


//TECHNIQUES
//**********
technique11 DefaultTechnique {
	pass p0 {
		SetRasterizerState(gRS_NoCulling);	
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}