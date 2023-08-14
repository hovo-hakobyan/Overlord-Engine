#include "stdafx.h"
#include "PlayerTank.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Components/BoxControllerComponent.h"
#include "Prefabs/Shell.h"
#include "Components/MuzzleComponent.h"
#include "Hatch.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"
#include "Misc/ShellManager.h"

PlayerTank::PlayerTank(const XMFLOAT3& loc, const XMFLOAT3& startRot,const TankDesc& tankDesc):
	BaseTank(loc,startRot,tankDesc)
{
	m_TankDesc.actionId_MoveForward = MoveForward;
	m_TankDesc.actionId_MoveBackward = MoveBackward;
	m_TankDesc.actionId_MoveLeft = MoveLeft;
	m_TankDesc.actionId_MoveRight = MoveRight;
	m_TankDesc.actionId_Shoot = Shoot;
}



void PlayerTank::Reset()
{
	m_pBoxControllerComponent->Translate(XMFLOAT3{ m_StartLocation.x,m_StartLocation.y + m_TankDesc.controller.halfHeight ,m_StartLocation.z });
	m_pColliderGameObj->GetTransform()->Translate(m_StartLocation.x, 1.f, m_StartLocation.z);

}

void PlayerTank::Initialize(const SceneContext& sceneContext)
{
	m_pGameScene = static_cast<BattleCityScene*>(GetScene());
	//Model
	m_pModelComponent = new ModelComponent(L"Meshes/Tank2.ovm");
	AddComponent(m_pModelComponent);

	//Material
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/tank/tank2Diffuse.png");
	m_pModelComponent->SetMaterial(m_pMaterial);

	

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
	SetTag(L"Friendly");
	m_IsDead = false;

	auto pTransform = GetTransform();
	XMFLOAT3 tankSize{ 0.2f,0.2f,0.2f };
	pTransform->Rotate(m_StartRotation);
	pTransform->Scale(tankSize);
	pTransform->Translate(m_StartLocation);

	//Sound
	const auto pSoundManager = SoundManager::Get()->GetSystem();
	pSoundManager->createStream("Resources/Sounds/Shooting.wav", FMOD_DEFAULT, nullptr, &m_pShootingSound);
	pSoundManager->createStream("Resources/Sounds/TankMoving.wav", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &m_pShootingSound);
	m_pShootingChannel->setVolume(1.f);
	m_pMovingChannel->setVolume(1.f);
}

void PlayerTank::Update(const SceneContext& sceneContext)
{

	if (m_TankDesc.isCombat)
	{
		if (m_pGameScene->IsGamePaused())
		{
			return;
		}

		if (m_pGameScene->GetGameState() != CurrentGameState::Gameplay)
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
	}
	

	
	constexpr float epsilon = 0.01f;
	XMFLOAT2 move = { 0.0f,0.0f };

	auto pTransform = GetTransform();
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

	if (move.x != 0 || move.y !=0)
	{
		if (!m_IsPlayingMovingSound)
		{
			SoundManager::Get()->GetSystem()->playSound(m_pMovingSound, nullptr, false, &m_pMovingChannel);
			m_IsPlayingMovingSound = true;
		}
		
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

		if (move.x > 0)
		{
			pTransform->Rotate(0.0f, 270.0f, 0.0f, true);
			m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
		}
		else if (move.x < 0)
		{
			pTransform->Rotate(0.0f, 90.0f, 0.0f, true);
			m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
		}

		
	}
	else if (fabs(move.y) > epsilon)
	{
		m_MoveSpeed += currentAcceleration;
		m_MoveSpeed = m_MoveSpeed >= m_TankDesc.maxMoveSpeed ? m_TankDesc.maxMoveSpeed : m_MoveSpeed;
		XMFLOAT3 displacement = {0.0f,0.0f, m_MoveSpeed * move.y * deltaTime };
		m_pBoxControllerComponent->Move(displacement);
		auto tankPos = pTransform->GetPosition();
		m_pCollider->GetTransform()->Translate(tankPos.x, 1.f, tankPos.z);

		if (move.y > 0)
		{
			pTransform->Rotate(0.0f, 180.0f, 0.0f, true);
			m_pBoxShape->setLocalPose(PxTransform{ PxQuat{0.0f,PxVec3{1,0,0}} });
		}
		else if (move.y < 0)
		{
			pTransform->Rotate(0.0f, 0.0f, 0.0f, true);
			m_pBoxShape->setLocalPose(PxTransform{ PxQuat{3.14159f,PxVec3{1,0,0}} });
		}
	}
	else
	{
		m_IsPlayingMovingSound = false;
		SoundManager::Get()->GetSystem()->playSound(m_pMovingSound, nullptr, true, &m_pMovingChannel);
	}
	
	m_CurrentShootCooldown += deltaTime;
	//Shooting
	if (m_TankDesc.isCombat)
	{
		if (sceneContext.pInput->IsActionTriggered(Shoot))
		{
			if (m_CurrentShootCooldown >= m_MaxShootCooldown)
			{
				constexpr float spawnDistance = 1.f;
				auto loc = pTransform->GetPosition();

				XMFLOAT3 dir = pTransform->GetForward();
				dir.x *= -1;
				dir.z *= -1;
				auto finalLoc = XMFLOAT3{ loc.x + dir.x * spawnDistance,1.0f,loc.z + dir.z * spawnDistance };
				auto finalRot = XMFLOAT3{ 0.0f,0.0f,0.0f };
				m_pGameScene->GetShellManager()->SpawnShell(finalLoc, finalRot, dir, m_pColliderGameObj->GetTag());

				m_CurrentShootCooldown = 0.0f;
				m_pAnimator->SetAnimation(0);
				m_pAnimator->SetAnimationSpeed(3.0f);
				m_pAnimator->Play();
				SoundManager::Get()->GetSystem()->playSound(m_pShootingSound, nullptr, false, &m_pShootingChannel);
			}



		}
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
