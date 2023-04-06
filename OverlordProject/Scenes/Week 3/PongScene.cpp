#include "stdafx.h"
#include "PongScene.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"

PongScene::PongScene() :
	GameScene(L"Pong Scene")
{
}

void PongScene::Initialize()
{
	auto& physX = PxGetPhysics();

	//Materials
	auto pBouncyMaterial = physX.createMaterial(.5f, .5f, 1.f);

	//Set a new fixed camera
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(0.f, 0.f, -30.f);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	//remove the old camera
	auto prevCamera = m_SceneContext.pCamera;
	RemoveChild(prevCamera->GetGameObject(),true);

	//Init paddles
	const XMFLOAT3 paddleSize{ 1.f,5.f,3.f };
	XMFLOAT3 paddleStart{ -5.f,0.f,0.f };
	PxBoxGeometry paddleGeo{ paddleSize.x / 2.f,paddleSize.y / 2.f, paddleSize.z / 2.f };

	//Left paddle
	m_pLeftPaddle = new CubePrefab(paddleSize, XMFLOAT4{ Colors::Red });
	AddChild(m_pLeftPaddle);
	m_pLeftPaddle->GetTransform()->Translate(paddleStart);
	auto pRigidBody = m_pLeftPaddle->AddComponent(new RigidBodyComponent());
	pRigidBody->AddCollider(paddleGeo, *pBouncyMaterial);
	pRigidBody->SetKinematic(true);
}

void PongScene::Update()
{

}

void PongScene::Draw()
{
}

void PongScene::OnGUI()
{
}
