#include "stdafx.h"
#include "PlayerTank.h"
#include "Materials/DiffuseMaterial_Skinned.h"

PlayerTank::PlayerTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot):
	m_StartLocation{startLoc},
	m_StartRotation{startRot}
{
}

void PlayerTank::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pModelComponent = new ModelComponent(L"Meshes/Tank2.ovm");
	AddComponent(m_pModelComponent);

	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/tank/tank2Diffuse.png");

	m_pModelComponent->SetMaterial(m_pMaterial);

	auto pTransform = GetTransform();


	XMFLOAT3 tankSize{ 0.2f,0.2f,0.2f};

	pTransform->Translate(m_StartLocation.x,m_StartLocation.y + tankSize.y,m_StartLocation.z);
	pTransform->Rotate(m_StartRotation);
	pTransform->Scale(tankSize);


	auto& physX = PxGetPhysics();
	auto pPhysicsMaterial = physX.createMaterial(.5f, .5f, .0f);

	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Tank2.ovpc");
	const auto convexMeshGeometry = PxConvexMeshGeometry{ pConvexMesh,PxMeshScale{PxVec3{tankSize.x + 0.1f,tankSize.y,tankSize.z}} };

	auto pRigidBody =AddComponent(new RigidBodyComponent());
	pRigidBody->AddCollider(convexMeshGeometry, *pPhysicsMaterial);
	pRigidBody->SetKinematic(true);


	m_pAnimator = m_pModelComponent->GetAnimator();
}
