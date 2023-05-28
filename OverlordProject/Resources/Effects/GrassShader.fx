//DX10 - FLAT SHADER
//Digital Arts & Entertainment

//Implemented using this tutorial
//https://roystan.net/articles/grass-shader/

//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4 gBottomColor : COLOR = float4(0.095f, 0.493f, 0.157f, 1.0f);
float4 gTopColor : COLOR = float4(0.674f, 0.875f, 0.396f, 1.0f);
float gTime : TIME;

float gTwoPI = 6.28378530718f;
float gPi = 3.14159265359f;
int gBladeSegments = 3;


float gBendAmount
<
	string UIName = "Bend Rotation";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.01f;
> = 0.6f;

float gBladeWidth
<
	string UIName = "Blade Width";
	string UIWidget = "slider";
	float UIMin = 0.001f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
> = 0.001f;

float gBladeHeight
<
	string UIName = "Blade Height";
	string UIWidget = "slider";
	float UIMin = 1.0f;
	float UIMax = 5.0f;
	float UIStep = 0.1f;
> = 2.0f;

float gBladeWidthRandom
<
	string UIName = "Blade Width Random";
	string UIWidget = "slider";
	float UIMin = 0.01f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
> = 0.01f;

float gBladeHeightRandom
<
	string UIName = "Blade Height Random";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
> = 0.45f;

float gBladeForward
<
	string UIName = "Blade Forward";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 1.0f;
	float UIStep = 0.01f;
> = 0.38f;

float gBladeCurve
<
	string UIName = "Blade Curve";
	string UIWidget = "slider";
	float UIMin = 1.f;
	float UIMax = 4.0f;
	float UIStep = 0.5f;
> = 2.0f;

//Wind
float gWindHarmony
<
	string UIName = "Wind Harmony";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 1.0f;
	float UIStep = 0.01f;
> = 0.1f;

float3 gWindDirection
<
	string UIName = "Wind Direction";
	float UIMin = 0.1f;
	float UIMax = 1.0f;
	float UIStep = 0.01f;
> = float3(0.2f, 1.0f, 0.2f);

float gWindStrength
<
	string UIName = "Wind Strength";
	float UIMin = 0.1f;
	float UIMax = 1.5f;
	float UIStep = 0.01f;
> = 0.5f;

//STATES
//******
RasterizerState gRS_NoCulling
{
    CullMode = NONE;
};

//**********
// STRUCTS *
//**********
struct VS_INPUT
{
    float4 vertex : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
};


struct GS_DATA
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

//**********
// Helper functions
//**********
//https://gist.github.com/keijiro/ee439d5e7388f3aafc5296005c8c3f33
//returns a matrix rotating "angle" amout around "axis"
float3x3 AngleAxis3x3(float angle, float3 axis)
{
    float c, s;
    sincos(angle, s, c);

    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return float3x3(
        t * x * x + c, t * x * y - s * z, t * x * z + s * y,
        t * x * y + s * z, t * y * y + c, t * y * z - s * x,
        t * x * z - s * y, t * y * z + s * x, t * z * z + c
    );
}

//Shout out to ChatGPT
float rand(float3 seed)
{
    return frac(sin(dot(seed, float3(12.9898, 78.233, 45.5432))) * 43758.5453);
}

//****************
// VERTEX SHADER *
//****************
VS_INPUT MainVS(VS_INPUT vsData)
{
    return vsData;
}

//***************
// GEOMETRY SHADER *
//***************


GS_DATA CreateVertex(float3 pos, float2 uv)
{
    GS_DATA o = (GS_DATA) 0;
    o.position = mul(float4(pos, 1.0f), gMatrixWorldViewProj);
    o.uv = uv;
    return o;
}

GS_DATA GenerateGrassVertex(float3 vertexPosition, float width, float height, float forward, float2 uv, float3x3 transformMat)
{
	//pos prevents the triangles being rendered on top of each other
		//mulitply by tangenttolocal matrix to align vertex with their input point's normal
    float3 tangentPoint = float3(width, forward, height);
	
    float3 localPosition = vertexPosition + mul(transformMat, tangentPoint);
    return CreateVertex(localPosition, uv);
}

//3 blades
//7 vertices each blade
[maxvertexcount(21)]
void BladeGenerator(triangle VS_INPUT IN[3], inout TriangleStream<GS_DATA> triStream)
{
    for (int i = 0; i < 3; ++i)
    {
        float3 pos = IN[i].vertex;
        float3 normal = normalize(IN[i].normal);
        float3 tangent = normalize(IN[i].tangent);
        float3 binormal = normalize(cross(normal, tangent));
	
        float3x3 tangentToLocal = float3x3(tangent, binormal, normal);
	
		//Matrix for random facing 
		//Use pos for seed to keep it consistent between frames
        float3x3 facingRotationMat = AngleAxis3x3(rand(pos) * gTwoPI, float3(0, 0.0, 1));
		//range is between 0-90 deg;
        float3x3 bendRotationMat = AngleAxis3x3(rand(pos.zzx) * gBendAmount * gPi * 0.5, float3(-1, 0, 0));
		
        float windOffset = pos.x * gWindHarmony + gTime;
        float windAngle = sin(windOffset) * gWindStrength;
        float3x3 windRotation = AngleAxis3x3(windAngle, gWindDirection);
		
		//This matrix includes wind sway, so we only apply this to the top vertex
        float3x3 transformationMat = mul(mul(mul(tangentToLocal, windRotation), facingRotationMat), bendRotationMat);
		
		//We don't want sway at the bottom vertices
        float3x3 transformationMatFacing = mul(tangentToLocal, facingRotationMat);
		
		//add randomness to prevent the uniform look of the blades
		// / 2 - 1 to map it to [-1,1] range
        float height = (rand(pos.zyx) * 2 - 1) * gBladeHeightRandom + gBladeHeight;
        float width = (rand(pos.xzy) * 2 - 1) * gBladeWidthRandom + gBladeWidth;
		
        float forward = rand(pos.xxz) * gBladeForward;
		
		//We want to add segments to each blade
        for (int i = 0; i < gBladeSegments; i++)
        {
			//[0,1] how far are we along the blade
            float t = i / (float) gBladeSegments;
			
			//As we go up, the blade width decreases and the height increases
            float segmentHeight = height * t;
            float segmentWidth = width * (1 - t);
			//t to a power to shape the balde into a curve
            float segmentForward = pow(t, gBladeCurve) * forward;
			
			//The bottom vertex doesn't need wind animations
            float3x3 transformMatrix = i == 0 ? transformationMatFacing : transformationMat;
			
			//Left vertex
            triStream.Append(GenerateGrassVertex(pos, segmentWidth, -segmentHeight, -segmentForward, float2(0, t), transformMatrix));
	
			//Right vertex
            triStream.Append(GenerateGrassVertex(pos, -segmentWidth, -segmentHeight, -segmentForward, float2(1, t), transformMatrix));
        }
		
		//Top vertex
        triStream.Append(GenerateGrassVertex(pos, 0, -height, -forward, float2(0.5f, 1), transformationMat));
    }
  
}

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    return lerp(gBottomColor, gTopColor, input.uv.y);
}


//TECHNIQUES
//**********
technique11 DefaultTechnique
{
    pass p0
    {
        SetRasterizerState(gRS_NoCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, BladeGenerator()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}