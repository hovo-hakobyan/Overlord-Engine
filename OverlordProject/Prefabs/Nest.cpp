#include "stdafx.h"
#include "Nest.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

Nest::Nest(const XMFLOAT3& loc):
	m_Location{loc}
{
	
}

void Nest::Initialize(const SceneContext&)
{
	auto pxMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	m_pModelComponent = new ModelComponent(L"Meshes/Tank2.ovm");
	AddComponent(m_pModelComponent);

	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/tank/tank2Diffuse.png");
	m_pModelComponent->SetMaterial(m_pMaterial);

	
	m_pAnimator = m_pModelComponent->GetAnimator();

	auto pTransform = GetTransform();
	pTransform->Translate(m_Location);

	m_pRigidBody = AddComponent(new RigidBodyComponent(true));
	m_pRigidBody->AddCollider(PxBoxGeometry{ 0.5f,1.5f,0.5f }, *pxMat);
	
	SetTag(L"Nest");
}
