#include "stdafx.h"
#include "EnemyTank.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Prefabs/Shell.h"
#include "Components/BoxControllerComponent.h"

EnemyTank::EnemyTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot, const TankDesc& tankDesc, GameScene* gameScene):
	BaseTank(startLoc, startRot, tankDesc, gameScene)
{
}

void EnemyTank::Initialize(const SceneContext&)
{
	//Model
	m_pModelComponent = new ModelComponent(L"Meshes/Tank2.ovm");
	AddComponent(m_pModelComponent);

	//Material
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/tank/tank2Diffuse.png");
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

	SetTag(L"Enemy");

	m_pRigidBody= m_pBoxControllerComponent->GetBoxShape()->getActor()->is<PxRigidBody>();
}

void EnemyTank::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	XMFLOAT2 move = { 0.0f,0.0f };
	auto pTransform = GetTransform();

	switch (m_Direction)
	{
	case EnemyTank::Direction::Left:
		move.x = -1;
		pTransform->Rotate(0.0f, 90.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
		break;
	case EnemyTank::Direction::Right:
		move.x = 1;
		pTransform->Rotate(0.0f, 270.0f, 0.0f, true);
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{1.5708f,PxVec3{1,0,0}} });
		break;
	case EnemyTank::Direction::Up:
		move.y = 1;
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{0.0f,PxVec3{1,0,0}} });
		pTransform->Rotate(0.0f, 180.0f, 0.0f, true);
		break;
	case EnemyTank::Direction::Down:
		move.y = -1;
		m_pBoxShape->setLocalPose(PxTransform{ PxQuat{3.14159f,PxVec3{1,0,0}} });
		pTransform->Rotate(0.0f, 0.0f, 0.0f, true);
		break;
	}

	Move(move, deltaTime);

	m_VelocityMagnitudeSqr = m_pRigidBody->getLinearVelocity().magnitudeSquared();

	if (m_VelocityMagnitudeSqr <= m_VelocityThreshold)
	{
		m_TimeSinceZeroVelocity += deltaTime;

		if (m_TimeSinceZeroVelocity > m_TimeThreshold)
		{
			m_TimeSinceZeroVelocity = 0.0f;
		}
	}
	else
	{
		m_TimeSinceZeroVelocity = 0.0f;
	}
}

void EnemyTank::Move(const XMFLOAT2& dir, float deltaTime)
{
	float currentAcceleration = m_MoveAcceleration * deltaTime;
	m_MoveSpeed += currentAcceleration;
	m_MoveSpeed = m_MoveSpeed >= m_TankDesc.maxMoveSpeed ? m_TankDesc.maxMoveSpeed : m_MoveSpeed;
	XMFLOAT3 displacement{ dir.x * m_MoveSpeed * deltaTime,0.0f,dir.y * m_MoveSpeed * deltaTime };
	m_pBoxControllerComponent->Move(displacement);
}
