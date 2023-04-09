#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial():
	Material<UberMaterial>(L"Effects/UberShader.fx")
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(L"Textures/Skulls_Diffusemap.tga");
	m_pSpecularTexture = ContentManager::Load<TextureData>(L"Textures/Skulls_Heightmap.tga");
	m_pNormal = ContentManager::Load<TextureData>(L"Textures/Skulls_Normalmap.tga");
	m_pEnvironment = ContentManager::Load<TextureData>(L"Textures/Sunol_Cubemap.dds");
	m_pOpacity = ContentManager::Load<TextureData>(L"Textures/Skulls_Heightmap.tga");
}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
	SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecularTexture);
	SetVariable_Texture(L"gTextureNormal", m_pNormal);
	SetVariable_Texture(L"gCubeEnvironment", m_pEnvironment);
	SetVariable_Texture(L"gTextureOpacity", m_pOpacity);

	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Scalar(L"gUseTextureNormal", true);
	SetVariable_Scalar(L"gTextureOpacityIntensity", true);
}
