float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
    DepthEnable = FALSE;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
    uint TextureId : TEXCOORD0; //Can be ignored
    float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
    float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
    if (rotation != 0)
    {	
		float2 pivotPoint = (offset + pivotOffset);
		//Move vertex back to origin
		pos.xy -= pivotPoint;
				
		//Rotate
		pos.x = pos.x * rotCosSin.x - pos.y * rotCosSin.y;
		pos.y = pos.y * rotCosSin.x + pos.y * rotCosSin.y;
		
		//Retransform to initial position
		pos.xy += offset;
    }
    else
    {
		//Step 2.
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
		pos.xy -= pivotOffset;
    }

	//Geometry Vertex Output
    GS_DATA geomData = (GS_DATA) 0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
    float3 position = vertex[0].TransformData.xyz;
    float2 offset = vertex[0].TransformData.xy;
    float rotation = vertex[0].TransformData.w;
    float2 pivot = vertex[0].TransformData2.xy;
    float2 scale = vertex[0].TransformData2.zw;
	float4 color = vertex[0].Color;
    float2 texCoord = float2(0, 0); //Initial Texture Coordinate
	float2 rotCosSin = rotation == 0 ? float2(1.0f,0.0f) : float2(cos(rotation),sin(rotation));
	float2 pivotOffset = gTextureSize * scale * pivot;
	
	//...

	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB

	//VERTEX 1 [LT]
    CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!
	
	//VERTEX 2 [RT]
	position.x += gTextureSize.x * scale.x;
	texCoord.x = 1.0f;
    CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 3 [LB]
	position.y +=  gTextureSize.y * scale.y;
	position.x -= gTextureSize.x * scale.x;
	texCoord = float2(0.0f,1.0f);
	CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 4 [RB]
	position.x += gTextureSize.x * scale.x;
	texCoord.x = 1.0f;
    CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
}

// Default Technique
technique10 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
