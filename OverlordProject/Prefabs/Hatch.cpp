#include "stdafx.h"
#include "Hatch.h"
#include "Materials/SimpleDiffuse.h"

Hatch::Hatch(const XMFLOAT3& loc, const XMFLOAT3& rot, float scale, const std::wstring& filePath):
	m_Location{loc},
	m_Rotation{rot},
	m_Scale{scale},
	m_FilePath{filePath}
{
}



void Hatch::CreateMaterial(const std::wstring& diffusePath, const std::wstring& normalPath)
{
	m_pModelComponent = new ModelComponent(m_FilePath);
	AddComponent(m_pModelComponent);

	m_pMaterial = MaterialManager::Get()->CreateMaterial<SimpleDiffuseMaterial>();
	m_pMaterial->SetDiffuseTexture(diffusePath);
	m_pMaterial->SetNormalMapTexture(normalPath);

	m_pModelComponent->SetMaterial(m_pMaterial);
}

void Hatch::Initialize(const SceneContext& /*sceneContext*/)
{
	
	auto pTransform = GetTransform();

	pTransform->Translate(m_Location);
	pTransform->Rotate(m_Rotation);
	pTransform->Scale(0.5f * m_Scale, 0.5f * m_Scale, 0.2f * m_Scale);
}
