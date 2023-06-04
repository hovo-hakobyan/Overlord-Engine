#include "stdafx.h"
#include "EnemyTank.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Prefabs/Shell.h"
#include "Components/BoxControllerComponent.h"
#include "Hatch.h"
#include <random>
#include "Scenes/Battle City 3D/BattleCityScene.h"
#include "WorldBuilder/ShellManager.h"

EnemyTank::EnemyTank(const XMFLOAT3& loc, const XMFLOAT3& startRot, const TankDesc& tankDesc):
	BaseTank(loc, startRot, tankDesc)
{
	++EnemyCounter;
}



void EnemyTank::Initialize(const SceneContext&)
{
	m_pGameScene = static_cast<BattleCityScene*>(GetScene());
	//Model
	m_pModelComponent = new ModelComponent(L"Meshes/Tank2.ovm");
	AddComponent(m_pModelComponent);

	//Material
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/tank/tank1Diffuse.png");
	m_pModelComponent->SetMaterial(m_pMaterial);

	auto pTransform = GetTransform();
	XMFLOAT3 tankSize{ 0.2f,0.2f,0.2f };
	pTransform->Rotate(m_StartRotation);
	pTransform->Scale(tankSize);
	pTransform->Translate(m_StartLocation);

	m_pAnimator = m_pModelComponent->GetAnimator();

	//Controller
	m_pBoxControllerComponent = AddComponent(new BoxControllerComponent(m_TankDesc.controller));
	m_pBoxControllerComponent->Translate(XMFLOAT3{ m_StartLocation.x,m_StartLocation.y + m_TankDesc.controller.halfHeight ,m_StartLocation.z });
	m_pBoxShape = m_pBoxControllerComponent->GetBoxShape();
	m_pBoxControllerComponent->SetCollisionGroup(CollisionGroup::Group7);
	m_pBoxControllerComponent->SetCollisionIgnoreGroup(CollisionGroup::Group5);
	m_pRigidBody= m_pBoxControllerComponent->GetBoxShape()->getActor()->is<PxRigidBody>();
	
	m_pColliderGameObj = new GameObject();
	AddChild(m_pColliderGameObj);

	auto pxMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	PxBoxGeometry Geo{0.5f,0.3f,0.5f};
	m_pColliderGameObj->GetTransform()->Translate(m_StartLocation.x, 0.8f, m_StartLocation.z);


	m_pCollider = m_pColliderGameObj->AddComponent(new RigidBodyComponent(true));
	m_pCollider->AddCollider(Geo, *pxMat);

	m_pColliderGameObj->SetTag(L"Enemy");
	m_IsDead = false;
}

void EnemyTank::Update(const SceneContext& sceneContext)
{
	if (m_pGameScene->GetGameState() != CurrentGameState::Gameplay)
	{
		return;
	}
	if (m_IsDead)
	{
		//the controllers shape doesn't delete when controller is deleted
		//so i translate it out of the view
		m_pBoxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		m_pBoxControllerComponent->Translate(XMFLOAT3{ 0.0f,m_pBoxControllerComponent->GetTransform()->GetPosition().y + 100.0f,0.0f});
		m_pGameScene->RemoveChild(this, true);
		--EnemyCounter;
		return;
	}
	

	float deltaTime = sceneContext.pGameTime->GetElapsed();
	XMFLOAT2 move = { 0.0f,0.0f };

	switch (m_Direction)
	{
	case EnemyTank::Direction::Left:
		move.x = -1;
		
		break;
	case EnemyTank::Direction::Right:
		move.x = 1;
		
		break;
	case EnemyTank::Direction::Up:
		move.y = 1;
		
		break;
	case EnemyTank::Direction::Down:
		move.y = -1;
		break;
	}
	Move(move, deltaTime);

	m_VelocityMagnitudeSqr = m_pRigidBody->getLinearVelocity().magnitudeSquared();
	if (m_VelocityMagnitudeSqr <= m_VelocityThreshold)
	{
		m_TimeSinceZeroVelocity += deltaTime;
		if (m_TimeSinceZeroVelocity > m_TimeThreshold)
		{	
			ChangeDirection();
			m_TimeSinceZeroVelocity = 0.0f;
			m_CurrentShootCooldown = 0.0f;
		}
		
	}

	m_CurrentShootCooldown += deltaTime;
	if (m_CurrentShootCooldown >= m_NormalShootCooldown)
	{
		Shoot();
		m_CurrentShootCooldown = 0.0f;
	}
}

void EnemyTank::Move(const XMFLOAT2& dir, float deltaTime)
{
	auto pTransform = GetTransform();
	float currentAcceleration = m_MoveAcceleration * deltaTime;
	m_MoveSpeed += currentAcceleration;
	m_MoveSpeed = m_MoveSpeed >= m_TankDesc.maxMoveSpeed ? m_TankDesc.maxMoveSpeed : m_MoveSpeed;
	XMFLOAT3 displacement{ dir.x * m_MoveSpeed * deltaTime,0.0f,dir.y * m_MoveSpeed * deltaTime };
	m_pBoxControllerComponent->Move(displacement);
	auto tankPos = GetTransform()->GetPosition();
	m_pCollider->GetTransform()->Translate(tankPos.x, 1.f, tankPos.z);


	switch (m_Direction)
	{
	case EnemyTank::Direction::Left:
		pTransform->Rotate(0.0f, 90.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
		break;
	case EnemyTank::Direction::Right:
		pTransform->Rotate(0.0f, 270.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
		break;
	case EnemyTank::Direction::Up:
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{0.0f,PxVec3{1,0,0}} });
		pTransform->Rotate(0.0f, 180.0f, 0.0f, true);
		break;
	case EnemyTank::Direction::Down:
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{3.14159f,PxVec3{1,0,0}} });
		pTransform->Rotate(0.0f, 0.0f, 0.0f, true);
		break;

	}

}

void EnemyTank::ChangeDirection()
{
	//Some randomess to left / right if down is not possible
	std::deque<Direction> directions = { Direction::Left, Direction::Right};
	std::shuffle(directions.begin(), directions.end(), std::random_device());
	
	//We prioritize going down
	directions.push_front(Direction::Down);

	//We only want to go up if there is nowhere else to go
	directions.push_back(Direction::Up);

	PxVec3 unitDirections[] = {
	   PxVec3(-1.0f, 0.0f, 0.0f),  // Left
	   PxVec3(1.0f, 0.0f, 0.0f),   // Right
	   PxVec3(0.0f, 0.0f, 1.0f),   // Forward
	   PxVec3(0.0f, 0.0f, -1.0f)   // Backward
	};

	PxScene* pxScene = m_pGameScene->GetPhysxProxy()->GetPhysxScene();
	auto tankPos = GetTransform()->GetPosition();
	PxVec3 origin = PxVec3{ tankPos.x, 2.0f,tankPos.z };
	PxRaycastBuffer hit;
	PxReal maxDistance = 1.0f;

	for (int i = 0; i < directions.size(); i++)
	{
		if (directions[i] == m_Direction)
		{
			continue;
		}
		PxVec3 dir;
		switch (directions[i])
		{
		case Direction::Left:
			dir = unitDirections[0];
			break;
		case Direction::Right:
			dir = unitDirections[1];
			break;
		case Direction::Up:
			dir = unitDirections[2];
			break;
		case Direction::Down:
			dir = unitDirections[3];
			break;

		}

		bool hasHit = pxScene->raycast(origin, dir, maxDistance, hit);
		if (!hasHit) //this direction is free
		{
			m_Direction = directions[i];
			break;
		}
		
	}
}

void EnemyTank::Shoot()
{
	constexpr float spawnDistance = 1.0f;
	auto loc = GetTransform()->GetPosition();
	XMFLOAT3 dir = GetTransform()->GetForward();
	dir.x *= -1;
	dir.z *= -1;
	auto finalLoc = XMFLOAT3{ loc.x + dir.x * spawnDistance,1.0f,loc.z + dir.z * spawnDistance };
	auto finalRot = XMFLOAT3{ 0.0f,0.0f,0.0f };
	m_pGameScene->GetShellManager()->SpawnShell(finalLoc, finalRot, dir, m_pColliderGameObj->GetTag());
	
}

	
	
	
