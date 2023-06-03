#include "stdafx.h"
#include "PlayerTank.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Components/BoxControllerComponent.h"
#include "Prefabs/Shell.h"
#include "Components/MuzzleComponent.h"
#include "Hatch.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"

PlayerTank::PlayerTank(const XMFLOAT3& loc, const XMFLOAT3& startRot,const TankDesc& tankDesc, BattleCityScene* gameScene):
	BaseTank(loc,startRot,tankDesc,gameScene)
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
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/tank/tank2Diffuse.png");
	m_pModelComponent->SetMaterial(m_pMaterial);

	auto pTransform = GetTransform();
	XMFLOAT3 tankSize{ 0.2f,0.2f,0.2f};
	pTransform->Rotate(m_StartRotation);
	pTransform->Scale(tankSize);
	pTransform->Translate(m_StartLocation);

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
	m_pBoxShape = m_pBoxControllerComponent->GetBoxShape();
	m_pBoxControllerComponent->SetCollisionGroup(CollisionGroup::Group5);
	m_pBoxControllerComponent->SetCollisionIgnoreGroup(CollisionGroup::Group7);

	m_pColliderGameObj = new GameObject();
	AddChild(m_pColliderGameObj);

	auto pxMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	PxBoxGeometry Geo{ 0.5f,0.3f,0.5f };
	m_pColliderGameObj->GetTransform()->Translate(m_StartLocation.x, 1.f, m_StartLocation.z);

	m_pCollider = m_pColliderGameObj->AddComponent(new RigidBodyComponent(true));
	m_pCollider->AddCollider(Geo, *pxMat);

	m_pColliderGameObj->SetTag(L"Friendly");
	m_IsDead = false;

}

void PlayerTank::Update(const SceneContext& sceneContext)
{
	if (m_pGameScene->GetGameEnded())
	{
		if (m_pAnimator->IsPlaying())
		{
			m_pAnimator->Reset();
		}
		return;
	}

	if (m_IsDead)
	{
		if (m_pAnimator->IsPlaying())
		{
			m_pAnimator->Reset();
		}
		m_pBoxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		return;
	}

	
	constexpr float epsilon = 0.01f;
	XMFLOAT2 move = { 0.0f,0.0f };

	auto pTransform = GetTransform();
	if (sceneContext.pInput->IsActionTriggered(MoveForward))
	{
		move.y = 1;
		pTransform->Rotate(0.0f, 180.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{0.0f,PxVec3{1,0,0}} });

	}
	else if (sceneContext.pInput->IsActionTriggered(MoveBackward))
	{
		move.y = -1;
		pTransform->Rotate(0.0f, 0.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{3.14159f,PxVec3{1,0,0}} });
	}

	if (sceneContext.pInput->IsActionTriggered(MoveRight))
	{
		move.x = 1;
		pTransform->Rotate(0.0f, 270.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });

	}
	else if (sceneContext.pInput->IsActionTriggered(MoveLeft))
	{
		move.x = -1;
		pTransform->Rotate(0.0f, 90.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
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
		auto tankPos = pTransform->GetPosition();
		m_pCollider->GetTransform()->Translate(tankPos.x,1.f, tankPos.z);

	}
	else if (fabs(move.y) > epsilon)
	{
		m_MoveSpeed += currentAcceleration;
		m_MoveSpeed = m_MoveSpeed >= m_TankDesc.maxMoveSpeed ? m_TankDesc.maxMoveSpeed : m_MoveSpeed;
		XMFLOAT3 displacement = {0.0f,0.0f, m_MoveSpeed * move.y * deltaTime };
		m_pBoxControllerComponent->Move(displacement);
		auto tankPos = pTransform->GetPosition();
		m_pCollider->GetTransform()->Translate(tankPos.x, 1.f, tankPos.z);
	}
	
	m_CurrentShootCooldown += deltaTime;
	//Shooting
	if (sceneContext.pInput->IsActionTriggered(Shoot) && m_CurrentShootCooldown >=m_MaxShootCooldown)
	{
		constexpr float spawnDistance = 1.0f;
		auto loc = pTransform->GetWorldPosition();
		XMFLOAT3 dir = pTransform->GetForward();
		dir.x *= -1;
		dir.z *= -1;
		auto pShell = new Shell(XMFLOAT3{loc.x + spawnDistance * dir.x,1.0f ,loc.z + spawnDistance * dir.z }, XMFLOAT3{ -90.0f * dir.x,-90.0f,-90.0f * dir.z }, dir, m_pGameScene,m_pColliderGameObj->GetTag());
		m_pGameScene->AddChild(pShell);

		m_CurrentShootCooldown = 0.0f;
		m_pAnimator->SetAnimation(0);
		m_pAnimator->SetAnimationSpeed(3.0f);
		m_pAnimator->Play();
	}	
	
	if (m_pAnimator->IsPlaying())
	{
		m_CurrentAnimTime += deltaTime;
		if (m_CurrentAnimTime >= m_AnimResetTime)
		{
			m_pAnimator->Reset();
			m_CurrentAnimTime = 0.0f;
		}
	}
}
