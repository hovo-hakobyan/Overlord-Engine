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

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};
/// Create Rasterizer State (Backface culling) 
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
	// Step 1: find the dimensions of the texture (the texture has a method for that
    float width = 0;
    float height = 0;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy (UV space for 1 pixel)	
    float du = 1.0f / width;
    float dy = 1.0f / height;
    
    float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
	// Step 3: Create a double for loop (5 iterations each)
    for (int x = -2; x < 3; ++x)
    {
        for (int y = -2; y < 3; ++y)
        {
            // Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	        // Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
            float2 offset = float2(x * du * 2.0f, y * dy * 2.0f);
            finalColor += gTexture.Sample(samPoint, input.TexCoord + offset);
        }

    }
	
	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
    finalColor /= 25;
    
	// Step 5: return the final color
    return finalColor;
}


//TECHNIQUE
//---------
technique11 Blur
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