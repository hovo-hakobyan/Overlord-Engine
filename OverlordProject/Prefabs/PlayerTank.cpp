#include "stdafx.h"
#include "PlayerTank.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Components/BoxControllerComponent.h"

PlayerTank::PlayerTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot,const TankDesc& tankDesc):
	m_StartLocation{startLoc},
	m_StartRotation{startRot},
	m_TankDesc{tankDesc}
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
	auto inputAction = InputAction(MoveLeft, InputState::down, 'A');
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveRight, InputState::down, 'D');
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveForward, InputState::down, 'W');
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveBackward, InputState::down, 'S');
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Shoot, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	sceneContext.pInput->AddInputAction(inputAction);

	m_pBoxControllerComponent = AddComponent(new BoxControllerComponent(m_TankDesc.controller));
	m_pBoxControllerComponent->Translate(XMFLOAT3{m_StartLocation.x,m_StartLocation.y + m_TankDesc.controller.halfHeight ,m_StartLocation.z});

}

void PlayerTank::Update(const SceneContext& sceneContext)
{
	constexpr float epsilon = 0.01f;
	XMFLOAT2 move = { 0.0f,0.0f };

	if (sceneContext.pInput->IsActionTriggered(MoveForward))
	{
		move.y = 1;
	}
	else if (sceneContext.pInput->IsActionTriggered(MoveBackward))
	{
		move.y = -1;
	}

	if (sceneContext.pInput->IsActionTriggered(MoveRight))
	{
		move.x = 1;
	}
	else if (sceneContext.pInput->IsActionTriggered(MoveLeft))
	{
		move.x = -1;
	}

	//prioritize moving right
	if (fabs(move.x) > epsilon)
	{
	
	}
	else if (fabs(move.y) > epsilon)
	{
		
	}

	
	

}
