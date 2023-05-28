#include "stdafx.h"
#include "WaterMaterial.h"

WaterMaterial::WaterMaterial():
	Material<WaterMaterial>(L"Effects/WaterShader.fx")
{
	m_pWaterTexture = ContentManager::Load<TextureData>(L"Textures/water/WaterTexture.jpg");
}

void WaterMaterial::InitializeEffectVariables()
{
	SetVariable_Texture(L"gWaterTexture", m_pWaterTexture);
}
