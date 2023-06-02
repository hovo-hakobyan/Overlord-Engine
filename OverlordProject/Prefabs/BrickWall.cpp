#include "stdafx.h"
#include "BrickWall.h"
#include "Materials/SimpleDiffuse.h"

BrickWall::BrickWall(const XMFLOAT3& loc, const XMFLOAT3& scale):
	m_Location{loc},
	m_Scale{scale}
{
	m_pBrickWallMaterial = MaterialManager::Get()->CreateMaterial<SimpleDiffuseMaterial>();
	m_pBrickWallMaterial->SetDiffuseTexture(L"Textures/wall/brickAlbedo.tif");
	m_pBrickWallMaterial->SetNormalMapTexture(L"Textures/wall/brickNormal.tif");
}

void BrickWall::Initialize(const SceneContext&)
{
	auto pxMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	PxBoxGeometry wallGeo{ m_Scale.x / 2.0f, m_Scale.y / 2.0f,m_Scale.z / 2.0f };

	auto pModelComponent = new ModelComponent(L"Meshes/BrickWall.ovm");
	pModelComponent->SetMaterial(m_pBrickWallMaterial);
	auto pTransform = GetTransform();
	pTransform->Translate(m_Location.x - m_Scale.x / 2.0f, m_Location.y + m_Scale.y / 2.0f, m_Location.z - m_Scale.z / 2.0f);
	pTransform->Scale(m_Scale.x * 2, m_Scale.y / 2.0f + 0.25f, m_Scale.z * 2);
	auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(wallGeo, *pxMat);
	AddComponent(pModelComponent);
}
