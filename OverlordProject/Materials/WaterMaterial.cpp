#include "stdafx.h"
#include "WaterMaterial.h"

WaterMaterial::WaterMaterial():
	Material<WaterMaterial>(L"Effects/WaterShader.fx")
{
	m_pWaterTexture = ContentManager::Load<TextureData>(L"Textures/water/WaterTexture.jpg");
	m_pFoamTexture = ContentManager::Load<TextureData>(L"Textures/water/waterFoam.dds");
	m_pNormalTexture = ContentManager::Load<TextureData>(L"Textures/water/waterNM1.dds");
	m_pNoiseTexture = ContentManager::Load<TextureData>(L"Textures/water/waterNoise.dds");
}

void WaterMaterial::UpdateTime(float gameTime)
{
	SetVariable_Scalar(L"time", gameTime);
}

void WaterMaterial::InitializeEffectVariables()
{
	SetVariable_Texture(L"gWaterTexture", m_pWaterTexture);
	SetVariable_Texture(L"gFoamTexture", m_pFoamTexture);
	SetVariable_Texture(L"gNormalMapTexture", m_pNormalTexture);
	SetVariable_Texture(L"gNoiseTexture", m_pNoiseTexture);
}
