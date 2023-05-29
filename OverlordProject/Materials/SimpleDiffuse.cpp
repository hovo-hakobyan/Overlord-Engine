#include "stdafx.h"
#include "SimpleDiffuse.h"

SimpleDiffuseMaterial::SimpleDiffuseMaterial():
	Material<SimpleDiffuseMaterial>(L"Effects/SimpleDiffuse.fx")
{
	
	
}

void SimpleDiffuseMaterial::InitializeEffectVariables()
{

	
}

void SimpleDiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pSimpleDiffuse = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureDiffuse", m_pSimpleDiffuse);
}

void SimpleDiffuseMaterial::SetNormalMapTexture(const std::wstring& assetFile)
{
	m_pSimpleDiffuseNormalMap = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureNormal", m_pSimpleDiffuseNormalMap);
}
