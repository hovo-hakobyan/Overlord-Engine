#include "stdafx.h"
#include "PlayerTank.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Components/BoxControllerComponent.h"

PlayerTank::PlayerTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot,const TankDesc& tankDesc):
	m_StartLocation{startLoc},
	m_StartRotation{startRot},
	m_TankDesc{tankDesc},
	m_MoveAcceleration{tankDesc.maxMoveSpeed / tankDesc.moveAccelerationTime}
{
	m_TankDesc.actionId_MoveForward = MoveForward;
	m_TankDesc.actionId_MoveBackward = MoveBackward;
	m_TankDesc.actionId_MoveLeft = MoveLeft;
	m_TankDesc.actionId_MoveRight = MoveRight;
	m_TankDesc.actionId_Shoot = Shoot;
}

void PlayerTank::Initialize(const SceneContext& sceneContext)
{
	//Model
	m_pModelComponent = new ModelComponent(L"Meshes/Tank2.ovm");
	AddComponent(m_pModelComponent);

	//Material
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/tank/tank2Diffuse.png");
	m_pModelComponent->SetMaterial(m_pMaterial);

	auto pTransform = GetTransform();
	XMFLOAT3 tankSize{ 0.2f,0.2f,0.2f};
	pTransform->Rotate(m_StartRotation);
	pTransform->Scale(tankSize);


	m_pAnimator = m_pModelComponent->GetAnimator();

	//Input
	auto inputAction = InputAction(MoveLeft, InputState::down, VK_LEFT);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveRight, InputState::down, VK_RIGHT);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveForward, InputState::down, VK_UP);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveBackward, InputState::down, VK_DOWN);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Shoot, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	sceneContext.pInput->AddInputAction(inputAction);

	//Controller
	m_pBoxControllerComponent = AddComponent(new BoxControllerComponent(m_TankDesc.controller));
	m_pBoxControllerComponent->Translate(XMFLOAT3{m_StartLocation.x,m_StartLocation.y + m_TankDesc.controller.halfHeight ,m_StartLocation.z});

	
}

void PlayerTank::Update(const SceneContext& sceneContext)
{
	constexpr float epsilon = 0.01f;
	XMFLOAT2 move = { 0.0f,0.0f };

	auto pBoxShape = m_pBoxControllerComponent->GetBoxShape();
	auto pTransform = GetTransform();
	if (sceneContext.pInput->IsActionTriggered(MoveForward))
	{
		move.y = 1;
		pTransform->Rotate(0.0f, 180.0f, 0.0f, true);
		pBoxShape->setLocalPose(PxTransform{ PxQuat{0.0f,PxVec3{1,0,0}} });

	}
	else if (sceneContext.pInput->IsActionTriggered(MoveBackward))
	{
		move.y = -1;
		pTransform->Rotate(0.0f, 0.0f, 0.0f, true);
		pBoxShape->setLocalPose(PxTransform{ PxQuat{3.14159f,PxVec3{1,0,0}} });
	}

	if (sceneContext.pInput->IsActionTriggered(MoveRight))
	{
		move.x = 1;
		pTransform->Rotate(0.0f, 270.0f, 0.0f, true);
		pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });

	}
	else if (sceneContext.pInput->IsActionTriggered(MoveLeft))
	{
		move.x = -1;
		pTransform->Rotate(0.0f, 90.0f, 0.0f, true);
		pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
	}

	float deltaTime = sceneContext.pGameTime->GetElapsed();
	float currentAcceleration = m_MoveAcceleration * deltaTime;
	//prioritize moving right
	if (fabs(move.x) > epsilon)
	{
		m_MoveSpeed += currentAcceleration;
		m_MoveSpeed = m_MoveSpeed >= m_TankDesc.maxMoveSpeed ? m_TankDesc.maxMoveSpeed : m_MoveSpeed;
		XMFLOAT3 displacement = { m_MoveSpeed * move.x * deltaTime,0.0f,0.0f };
		m_pBoxControllerComponent->Move(displacement);

	}
	else if (fabs(move.y) > epsilon)
	{
		m_MoveSpeed += currentAcceleration;
		m_MoveSpeed = m_MoveSpeed >= m_TankDesc.maxMoveSpeed ? m_TankDesc.maxMoveSpeed : m_MoveSpeed;
		XMFLOAT3 displacement = {0.0f,0.0f, m_MoveSpeed * move.y * deltaTime };
		m_pBoxControllerComponent->Move(displacement);
	}
	
	
}
