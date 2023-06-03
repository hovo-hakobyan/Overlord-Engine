#include "stdafx.h"
#include "Nest.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"

Nest::Nest(const XMFLOAT3& loc, BattleCityScene* pGameScene):
	m_Location{loc},
	m_pGameScene{pGameScene}
{
	
}

void Nest::Initialize(const SceneContext&)
{
	auto pxMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	m_pModelComponent = new ModelComponent(L"Meshes/king.ovm");
	AddComponent(m_pModelComponent);

	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/King.png");
	m_pModelComponent->SetMaterial(m_pMaterial);

	
	m_pAnimator = m_pModelComponent->GetAnimator();
	m_pAnimator->SetAnimation(0);
	m_pAnimator->Play();

	auto pTransform = GetTransform();
	pTransform->Translate(m_Location);
	pTransform->Rotate(0.0f, 180.f, 0.0f);
	pTransform->Scale(0.05f, 0.05f, 0.05f);

	m_pRigidBody = AddComponent(new RigidBodyComponent(true));
	m_pRigidBody->AddCollider(PxBoxGeometry{ 0.5f,1.5f,0.5f }, *pxMat);
	
	SetTag(L"Nest");
}

void Nest::Update(const SceneContext&)
{
	if (!m_pAnimator->IsPlaying())
	{
		
	}
	if (m_pGameScene->GetGameEnded())
	{
		std::cout << "game ended " << std::endl;
	}
}
