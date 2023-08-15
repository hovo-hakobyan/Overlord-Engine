//DX10 - FLAT SHADER
//Digital Arts & Entertainment

int ResolutionX = 1280;
int ResolutionY = 720;
float VignetteRadius = 0.8f;
float VignetteAmount = 0.5f;

Texture2D gTexture;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};


RasterizerState gRS_BackCulling
{
    CullMode = BACK;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

struct VertexInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD1;
};

// VERTEX SHADER
//-------------
PixelInput VS(VertexInput input)
{
	PixelInput output = (PixelInput)0;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
	return output;
}

//MAIN PIXEL SHADER
//*****************
float4 PS(PixelInput input) : SV_TARGET 
{
	float2 center = float2(0.5, 0.5); // Center of the screen
    float2 screenCoord = input.position.xy / float2(ResolutionX, ResolutionY); // Assuming you have access to screen resolution
    
    float distance = length(screenCoord - center);
    
    // Calculate vignette factor based on distance from center
    float vignetteFactor = smoothstep(VignetteRadius, VignetteRadius - VignetteAmount, distance);
    
    // Apply the vignette effect by darkening the pixel color
    float4 color = gTexture.Sample(samPoint, input.texCoord);
    color.rgb *= vignetteFactor;
    
    return color;
}


//TECHNIQUES
//**********
technique10 DefaultTechnique
{
	pass p0
	{
		SetRasterizerState(gRS_BackCulling);
        SetDepthStencilState(EnableDepth, 0);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}