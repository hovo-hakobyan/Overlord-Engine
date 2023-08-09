#include "stdafx.h"
#include "WorldButton.h"
#include "Materials/DiffuseMaterial.h"
#include <codecvt>
#include <locale>

WorldButton::WorldButton(const std::string& texturePath):
	m_Path{texturePath}
{
}

void WorldButton::Initialize(const SceneContext&)
{
	auto& physX = PxGetPhysics();
	auto pPhysicsMaterial = physX.createMaterial(.5f, .5f, .1f);

	m_pModelComp = new ModelComponent(L"Meshes/Button.ovm");

	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();

	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, m_Path.c_str(), -1, nullptr, 0);
	std::wstring wideStr(bufferSize, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, m_Path.c_str(), -1, &wideStr[0], bufferSize);

	m_pMaterial->SetDiffuseTexture(wideStr);

	auto pBorderMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pBorderMat->SetDiffuseTexture(L"Textures/ground/soilAlbedo.tif");

	m_pModelComp->SetMaterial(pBorderMat, 0);
	m_pModelComp->SetMaterial(m_pMaterial,1);

	AddComponent(m_pModelComp);

	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Button.ovpc");
	const auto convexGeo = PxConvexMeshGeometry{ pConvexMesh };

	auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(convexGeo, *pPhysicsMaterial,true);


}
