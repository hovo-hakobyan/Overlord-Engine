#include "stdafx.h"
#include "BoxControllerComponent.h"

BoxControllerComponent::BoxControllerComponent(const PxBoxControllerDesc& controllerDesc):
	m_ControllerDesc{controllerDesc}
{
}

void BoxControllerComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	if (!m_IsInitialized)
	{
		XMFLOAT3 pos = GetTransform()->GetWorldPosition();
		m_ControllerDesc.position = PxExtendedVec3{ pos.x,pos.y,pos.z };
		m_ControllerDesc.userData = static_cast<void*>(this);

		PxControllerManager* pManager = m_pScene->GetPhysxProxy()->GetControllerManager();
		PxController* c = pManager->createController(m_ControllerDesc);
		ASSERT_NULL(c, L"PxController returned nullptr");
		m_pController = c;
		m_pController->getActor()->userData = static_cast<void*>(m_pController);
		SetCollisionGroup(static_cast<CollisionGroup>(m_CollisionGroups.word0));
		SetCollisionIgnoreGroup(static_cast<CollisionGroup>(m_CollisionGroups.word1));

		auto actor = c->getActor();
		PxShape* shapes[1];
		actor->getShapes(shapes, 1, 0);
		m_pShape = shapes[0];
	}
}

void BoxControllerComponent::OnSceneDetach(GameScene*)
{
	if (m_pController)
	{
		m_pController->getActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	}
}

void BoxControllerComponent::OnSceneAttach(GameScene*)
{
	if (m_pController)
	{
		m_pController->getActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
	}
}

void BoxControllerComponent::OnOwnerAttach(GameObject* pOwner)
{
	pOwner->GetTransform()->SetBoxControllerComponent(this);
}

void BoxControllerComponent::OnOwnerDetach(GameObject* pOwner)
{
	pOwner->GetTransform()->SetBoxControllerComponent(nullptr);
}

void BoxControllerComponent::ApplyFilterData() const
{
	if (m_pController != nullptr)
	{
		const auto actor = m_pController->getActor();
		const auto numShapes = actor->getNbShapes();
		const auto shapes = new PxShape * [numShapes];

		const auto numPointers = actor->getShapes(shapes, numShapes);
		for (PxU32 i = 0; i < numPointers; i++)
		{
#pragma warning (push)
#pragma warning (disable: 6385)
			const auto shape = shapes[i];
#pragma warning (pop)
			shape->setSimulationFilterData(m_CollisionGroups);
			shape->setQueryFilterData(m_CollisionGroups);
		}
		delete[] shapes;
	}
}


void BoxControllerComponent::Translate(const XMFLOAT3& pos) const
{
	ASSERT_NULL_(m_pController);
	m_pController->setPosition(PhysxHelper::ToPxExtendedVec3(pos));
}



void BoxControllerComponent::Move(const XMFLOAT3& displacement, float minDistance)
{
	ASSERT_NULL_(m_pController);
	m_CollisionFlag = m_pController->move(PhysxHelper::ToPxVec3(displacement), minDistance, 0, nullptr, nullptr);
}

XMFLOAT3 BoxControllerComponent::GetPosition() const
{
	ASSERT_NULL_(m_pController);
	return PhysxHelper::ToXMFLOAT3(m_pController->getPosition());
}

XMFLOAT3 BoxControllerComponent::GetFootPosition() const
{
	ASSERT_NULL_(m_pController);
	return PhysxHelper::ToXMFLOAT3(m_pController->getFootPosition());
}

void BoxControllerComponent::SetCollisionGroup(CollisionGroup groups)
{
	m_CollisionGroups.word0 = PxU32(groups);
	ApplyFilterData();
}

void BoxControllerComponent::SetCollisionIgnoreGroup(CollisionGroup ignoreGroups)
{
	m_CollisionGroups.word1 = PxU32(ignoreGroups);
	ApplyFilterData();
}





