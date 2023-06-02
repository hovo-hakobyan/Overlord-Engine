#include "stdafx.h"
#include "Shell.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

Shell::Shell(const XMFLOAT3& loc, const XMFLOAT3& rot, const XMFLOAT3& dir, GameObject* parent):
	m_Location{loc},
	m_Rotation{rot},
	m_Direction{dir},
	m_pParent{parent}
{
}

void Shell::Initialize(const SceneContext&)
{
	//auto& physX = PxGetPhysics();
	//auto pPhysicsMaterial = physX.createMaterial(1.0f, 1.0f, .0f);

	m_pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pMat->SetDiffuseTexture(L"Textures/tank/shell.png");

	const auto pModel = new ModelComponent(L"Meshes/Shell.ovm");
	pModel->SetMaterial(m_pMat);

	AddComponent(pModel);
	auto pTransform = GetTransform();
	pTransform->Translate(m_Location);
	pTransform->Rotate(m_Rotation);
	pTransform->Scale(0.5f, 0.1f, 0.5f);

}

void Shell::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();

	m_CurrentLifeTime += deltaTime;

	if (m_CurrentLifeTime >= m_Lifetime)
	{
		m_pParent->RemoveChild(this, true);
	}
}
