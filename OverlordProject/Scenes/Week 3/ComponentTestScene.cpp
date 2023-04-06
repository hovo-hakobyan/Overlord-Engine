#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Prefabs/SpherePrefab.h"

ComponentTestScene::ComponentTestScene():
	GameScene(L"ComponentTestScene") {}

void ComponentTestScene::Initialize()
{
	auto& physX = PxGetPhysics();
	auto pBouncyMaterial = physX.createMaterial(.5f, .5f, 1.f);

	//Groundplane
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	//Sphere Red (Group 0)
	auto pSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Red });
	AddChild(pSphere);

	pSphere->GetTransform()->Translate(0, 30.f, 0);

	auto pRigidBody = pSphere->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);
	pRigidBody->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);

	//Sphere Green (Group 1)
	pSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Green });
	AddChild(pSphere);

	pSphere->GetTransform()->Translate(0, 20.f, 0);

	 pRigidBody = pSphere->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);
	pRigidBody->SetConstraint(RigidBodyConstraint::TransX | RigidBodyConstraint::TransZ, false);

	//Sphere Blue (Group 2)
	pSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Blue });
	AddChild(pSphere);

	pSphere->GetTransform()->Translate(0, 10.f, 0);

	 pRigidBody = pSphere->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);

}

void ComponentTestScene::Update()
{
}

void ComponentTestScene::Draw()
{
}

void ComponentTestScene::OnGUI()
{
}
