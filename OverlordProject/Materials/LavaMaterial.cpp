#include "stdafx.h"
#include "LavaMaterial.h"

LavaMaterial::LavaMaterial() :
	Material<LavaMaterial>(L"Effects/LavaShader.fx")
{
	m_pLavaTexture = ContentManager::Load<TextureData>(L"Textures/Lava/LavaTexture.jpg");
	m_pNoiseTexture = ContentManager::Load<TextureData>(L"Textures/Lava/LavaNoise.jpg");
}

void LavaMaterial::InitializeEffectVariables()
{
	SetVariable_Texture(L"gTex1", m_pLavaTexture);
	SetVariable_Texture(L"gTex0", m_pNoiseTexture);
}
