#include "stdafx.h"
#include "GroundMaterial.h"

GroundMaterial::GroundMaterial():
	Material<GroundMaterial>(L"Effects/Ground.fx")
{
	m_pGroundDiffuse = ContentManager::Load<TextureData>(L"Textures/ground/soilAlbedo.tif");
	m_pGroundNormalMap = ContentManager::Load<TextureData>(L"Textures/ground/soilNormal.tif");
}

void GroundMaterial::InitializeEffectVariables()
{
	SetVariable_Texture(L"gTextureDiffuse", m_pGroundDiffuse);
	SetVariable_Texture(L"gTextureNormal", m_pGroundNormalMap);
}
