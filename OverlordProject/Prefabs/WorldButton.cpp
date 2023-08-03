#include "stdafx.h"
#include "WorldButton.h"
#include "Materials/DiffuseMaterial.h"

WorldButton::WorldButton(const std::wstring& texturePath):
	m_Path{texturePath}
{
}

void WorldButton::Initialize(const SceneContext&)
{
	auto& physX = PxGetPhysics();
	auto pPhysicsMaterial = physX.createMaterial(.5f, .5f, .1f);

	m_pModelComp = new ModelComponent(L"Meshes/Button.ovm");

	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pMaterial->SetDiffuseTexture(m_Path);

	auto pBorderMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pBorderMat->SetDiffuseTexture(L"Textures/ground/soilAlbedo.tif");

	m_pModelComp->SetMaterial(pBorderMat, 0);
	m_pModelComp->SetMaterial(m_pMaterial,1);

	AddComponent(m_pModelComp);

	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Button.ovpc");
	const auto convexGeo = PxConvexMeshGeometry{ pConvexMesh };

	auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(convexGeo, *pPhysicsMaterial);


}
