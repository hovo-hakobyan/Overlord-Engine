#include "stdafx.h"
#include "ModelTestScene.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"

ModelTestScene::ModelTestScene():
	GameScene(L"ModelTestScene")
{
}

void ModelTestScene::Initialize()
{
	auto& physX = PxGetPhysics();

	//Materials
	auto pPhysicsMaterial = physX.createMaterial(.5f, .5f, .1f);

	m_pChair = new GameObject();
	AddChild(m_pChair);

	/*ColorMaterial* pColorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pColorMat->SetVariable_Vector(L"gColor", XMFLOAT4{ Colors::Red });*/

	DiffuseMaterial* pDiffuseMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pDiffuseMat->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	const auto pModel = new ModelComponent(L"Meshes/Chair.ovm");
	//pModel->SetMaterial(pColorMat);
	pModel->SetMaterial(pDiffuseMat);

	m_pChair->AddComponent(pModel);
	m_pChair->GetTransform()->Translate(0.f, 10.f, 0.f);

	const auto pChairConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	const auto convexMeshGeometry = PxConvexMeshGeometry{ pChairConvexMesh };

	auto pRigidBody = m_pChair->AddComponent(new RigidBodyComponent());
	pRigidBody->AddCollider(convexMeshGeometry, *pPhysicsMaterial);

	//For more realistic chair behavior
	PxRigidBody* rigidBody = pRigidBody->GetPxRigidActor()->is<PxRigidBody>();
	PxVec3 massLocalPos{ 0.f,4.f,0.f };
	PxRigidBodyExt::updateMassAndInertia(*rigidBody,10.f,&massLocalPos);

	GameSceneExt::CreatePhysXGroundPlane(*this);
}

void ModelTestScene::Update()
{
}

void ModelTestScene::Draw()
{
}

void ModelTestScene::OnGUI()
{
	
}
