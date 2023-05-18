//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4 gBottomColor : COLOR = float4(0.095f, 0.493f, 0.157f, 1.0f);
float4 gTopColor : COLOR = float4(0.674f, 0.875f, 0.396f, 1.0f);

float gTwoPI = 6.28378530718f;
float gPi = 3.14159265359f;
int gMaxPoints = 3;
float gBendAmount
<
	string UIName = "Bend Rotation";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.01f;
> = 0.0f;

float gBladeWidth
<
	string UIName = "Blade Width";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
> = 0.05f;

float gBladeHeight
<
	string UIName = "Blade Height";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
> = 0.5f;

float gBladeWidthRandom
<
	string UIName = "Blade Width Random";
	string UIWidget = "slider";
	float UIMin = 0.01f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
> = 0.02f;

float gBladeHeightRandom
<
	string UIName = "Blade Height Random";
	string UIWidget = "slider";
	float UIMin = 0.1f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
> = 0.3f;

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
    float3 vertex : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

// HULL SHADER DATA

//Output control point structure
struct BEZIER_CONTROL_POINT
{
    float3 vertex : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

//Output patch constant data struct
struct HS_CONSTANT_DATA_OUTPUT
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
	
    float3 normal[4] : NORMAL;
    float3 vertex[4] : POSITION;
    float3 tangent[4] : TANGENT;
	
};

// GEOMETRY SHADER DATA
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
// HULL SHADER *
//***************
HS_CONSTANT_DATA_OUTPUT PatchConstantFunction(
	InputPatch<VS_INPUT, gMaxPoints> inputPatch,
	uint patchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT output;
	
	//Set the tessellation factors
    output.edges[0] = 3.0f;
    output.edges[1] = 3.0f;
    output.edges[2] = 3.0f;
	
    output.inside = 3.0f;
	
	//Pass the control points to the domain shader
    for (uint i = 0; i < 3; ++i)
    {
        output.vertex[i] = inputPatch[i].vertex;
        output.normal[i] = inputPatch[i].normal;
        output.tangent[i] = inputPatch[i].tangent;
    }
	
    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_ccw")]
//[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
BEZIER_CONTROL_POINT MainHS(
	InputPatch<VS_INPUT, gMaxPoints> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    BEZIER_CONTROL_POINT output;
		
    output.vertex = ip[i].vertex;
    output.normal = ip[i].normal;
    output.tangent = ip[i].tangent;
		
    return output;
}

//Domain Shader
[domain("tri")]
BEZIER_CONTROL_POINT MainDS(HS_CONSTANT_DATA_OUTPUT input, float3 uvw : SV_DomainLocation, const OutputPatch<BEZIER_CONTROL_POINT, gMaxPoints> patch)
{
    BEZIER_CONTROL_POINT output;
	
    output.vertex = uvw.x * patch[0].vertex + uvw.y * patch[1].vertex + uvw.z * patch[2].vertex;
    output.normal = uvw.x * patch[0].normal + uvw.y * patch[1].normal + uvw.z * patch[2].normal;
    output.tangent = uvw.x * patch[0].tangent + uvw.y * patch[1].tangent + uvw.z * patch[2].tangent;
	
    return output;
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


[maxvertexcount(3)]
void BladeGenerator(triangle BEZIER_CONTROL_POINT IN[3], inout TriangleStream<GS_DATA> triStream)
{
    float3 pos = IN[0].vertex;
    float3 normal = normalize(IN[0].normal);
    float3 tangent = normalize(IN[0].tangent);
    float3 binormal = normalize(cross(normal, tangent));
	
    float3x3 tangentToLocal = float3x3(tangent, binormal, normal);
	
	//Matrix for random facing 
	//Use pos for seed to keep it consistent between frames
    float3x3 facingRotationMat = AngleAxis3x3(rand(pos) * gTwoPI, float3(0, 0.0, 1));
	//range is between 0-90 deg;
    float3x3 bendRotationMat = AngleAxis3x3(rand(pos.zzx) * gBendAmount * gPi * 0.5, float3(-1, 0, 0));
	
    float3x3 transformationMat = mul(mul(tangentToLocal, facingRotationMat), bendRotationMat);

	//add randomness to prevent the uniform look of the blades
	// / 2 - 1 to map it to [-1,1] range
    float height = (rand(pos.zyx) * 2 - 1) * gBladeHeightRandom + gBladeHeight;
    float width = (rand(pos.xzy) * 2 - 1) * gBladeWidthRandom + gBladeWidth;
	
	//pos prevents the triangles being rendered on top of each other
	//mulitply by tangenttolocal matrix to align vertex with their input point's normal
	
	//Left bottom vertex
    float3 vertexInPos = pos + mul(transformationMat, float3(width, 0.0f, 0.0f));
    triStream.Append(CreateVertex(vertexInPos, float2(0, 0)));
	
	//Right bottom vertex
    vertexInPos = pos + mul(transformationMat, float3(-width, 0.0f, 0.0f));
    triStream.Append(CreateVertex(vertexInPos, float2(1, 0)));
	
	//Top vertex
    vertexInPos = pos + mul(transformationMat, float3(0.0f, 0.0f, -height));
    triStream.Append(CreateVertex(vertexInPos, float2(0.5f, 1)));
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
technique10 DefaultTechnique
{
    pass p0
    {
        SetRasterizerState(gRS_NoCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetHullShader(CompileShader(hs_5_0, MainHS()));
        SetDomainShader(CompileShader(ds_5_0 / MainDS()));
        SetGeometryShader(CompileShader(gs_4_0, BladeGenerator()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}