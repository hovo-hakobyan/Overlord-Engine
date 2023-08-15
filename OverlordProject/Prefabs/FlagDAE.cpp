#include "stdafx.h"
#include "FlagDAE.h"
#include "Materials/DiffuseMaterial.h"

FlagDAE::FlagDAE()
{
}

void FlagDAE::Initialize(const SceneContext&)
{
	m_pModelComp = new ModelComponent(L"Meshes/flag.ovm");
	m_pFlagMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();

	m_pFlagMaterial->SetDiffuseTexture(L"Textures/dae.png");

	m_pModelComp->SetMaterial(m_pFlagMaterial);

	AddComponent(m_pModelComp);
}
