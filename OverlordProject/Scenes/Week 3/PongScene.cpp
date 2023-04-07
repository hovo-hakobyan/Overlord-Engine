#include "stdafx.h"
#include "PongScene.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

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
	auto prevCamera = m_SceneContext.pCamera;
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(0.f, 0.f, -30.f);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	//remove the old camera
	RemoveChild(prevCamera->GetGameObject(),true);

	//Init paddles
	const XMFLOAT3 paddleSize{ 1.f,6.f,2.5f };
	XMFLOAT3 paddleStart{ -20.f,0.f,0.f };
	PxBoxGeometry paddleGeo{ paddleSize.x / 2.f,paddleSize.y / 2.f, paddleSize.z / 2.f };

	//Left paddle
	m_pLeftPaddle = new CubePrefab(paddleSize, XMFLOAT4{ Colors::Red });
	AddChild(m_pLeftPaddle);
	m_pLeftPaddle->GetTransform()->Translate(paddleStart);
	m_pLeftRigidBody = m_pLeftPaddle->AddComponent(new RigidBodyComponent());
	m_pLeftRigidBody->AddCollider(paddleGeo, *pBouncyMaterial);
	m_pLeftRigidBody->SetKinematic(true);

	//Right paddle
	m_pRightPaddle = new CubePrefab(paddleSize, XMFLOAT4{ Colors::Green });
	AddChild(m_pRightPaddle);
	m_pRightPaddle->GetTransform()->Translate(-paddleStart.x, paddleStart.y, paddleStart.z);
	m_pRightRigidBody = m_pRightPaddle->AddComponent(new RigidBodyComponent());
	m_pRightRigidBody->AddCollider(paddleGeo, *pBouncyMaterial);
	m_pRightRigidBody->SetKinematic(true);

	//Input
	auto input = m_SceneContext.pInput;

	input->AddInputAction(InputAction{ InputIds::FirstUp,InputState::down,'W'});
	input->AddInputAction(InputAction{ InputIds::FirstDown,InputState::down,'S' });
	input->AddInputAction(InputAction{ InputIds::SecondUp,InputState::down,VK_UP });
	input->AddInputAction(InputAction{ InputIds::SecondDown,InputState::down,VK_DOWN });

}

void PongScene::Update()
{
	const auto input = m_SceneContext.pInput;
	float deltaTime = m_SceneContext.pGameTime->GetElapsed();

	if (input->IsActionTriggered(InputIds::FirstUp))
	{
		XMFLOAT3 currentPos{ m_pLeftRigidBody->GetPosition() };
		XMFLOAT3 destination{currentPos.x,currentPos.y + m_PaddleSpeed * deltaTime,currentPos.z};
		m_pLeftRigidBody->GetTransform()->Translate(destination);
	}
	if (input->IsActionTriggered(InputIds::FirstDown))
	{
		XMFLOAT3 currentPos{ m_pLeftRigidBody->GetPosition() };
		XMFLOAT3 destination{ currentPos.x,currentPos.y - m_PaddleSpeed * deltaTime,currentPos.z };
		m_pLeftRigidBody->GetTransform()->Translate(destination);
	}
	if (input->IsActionTriggered(InputIds::SecondDown))
	{
		XMFLOAT3 currentPos{ m_pRightRigidBody->GetPosition() };
		XMFLOAT3 destination{ currentPos.x,currentPos.y - m_PaddleSpeed * deltaTime,currentPos.z };
		m_pRightRigidBody->GetTransform()->Translate(destination);
	}
	if (input->IsActionTriggered(InputIds::SecondUp))
	{
		XMFLOAT3 currentPos{ m_pRightRigidBody->GetPosition() };
		XMFLOAT3 destination{ currentPos.x,currentPos.y + m_PaddleSpeed * deltaTime,currentPos.z };
		m_pRightRigidBody->GetTransform()->Translate(destination);
	}
}

void PongScene::Draw()
{
}

void PongScene::OnGUI()
{
}
