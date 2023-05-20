#include "stdafx.h"
#include "WaterMaterial.h"

WaterMaterial::WaterMaterial():
	Material<WaterMaterial>(L"Effects/WaterShader.fx")
{
	m_pWaterTexture = ContentManager::Load<TextureData>(L"Textures/water/waterNoise.dds");
}

void WaterMaterial::UpdateTime(float gameTime)
{
	SetVariable_Scalar(L"time", gameTime);
}

void WaterMaterial::InitializeEffectVariables()
{
	SetVariable_Texture(L"gWaterTexture", m_pWaterTexture);
}
