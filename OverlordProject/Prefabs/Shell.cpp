#include "stdafx.h"
#include "Shell.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "BaseTank.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"



Shell::Shell(const XMFLOAT3& loc, const XMFLOAT3& rot, const XMFLOAT3& dir, BaseTank* parent, const std::wstring& parentTag):
	m_Location{loc},
	m_Rotation{rot},
	m_Direction{dir},
	m_pParent{parent},
	m_ParentTag{parentTag}
{

}

void Shell::Initialize(const SceneContext&)
{
	m_pGameScene =static_cast<BattleCityScene*>(GetScene());
	GetTransform()->Rotate(m_Rotation);
	GetTransform()->Scale(0.1f, 0.05f, 0.1f);
	GetTransform()->Translate(m_Location);

	auto& physX = PxGetPhysics();
	auto pPhysicsMaterial = physX.createMaterial(1.0f, 1.0f, .0f);

	m_pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pMat->SetDiffuseTexture(L"Textures/tank/shell.png");

	const auto pModel = new ModelComponent(L"Meshes/Shell.ovm");
	pModel->SetMaterial(m_pMat);
	AddComponent(pModel);

	
	PxBoxGeometry shellGeo{ 0.2f,0.2f,0.2f };
	
	m_pRigidBody = new RigidBodyComponent();
	AddComponent(m_pRigidBody);
	m_pRigidBody->AddCollider(shellGeo, *pPhysicsMaterial,true);
	m_pRigidBody->SetConstraint(RigidBodyConstraint::AllRot | RigidBodyConstraint::TransY, false);

	SetOnTriggerCallBack([=](GameObject*, GameObject* other, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
 				if (other->GetTag().compare(L"Destructible") == 0)
				{
					
					m_pHitObject = other;
				}
				else if (other->GetTag().compare(L"Enemy") == 0)
				{
					
					//to prevent friendly fire amongst enemies
					if (m_ParentTag.compare(L"Enemy") != 0)
					{
			
						m_pHitObject = other->GetParent();
						auto tank = static_cast<BaseTank*>(m_pHitObject);
						if (tank)
						{
							tank->SetIsDead(true);
							
						}

						m_pHitObject = nullptr;
					}
					
				}
				else if (other->GetTag().compare(L"Friendly") == 0)
				{
		
					//to not hit self
					if (m_ParentTag.compare(L"Friendly") != 0)
					{
						m_pHitObject = other->GetParent();
						auto tank = static_cast<BaseTank*>(m_pHitObject);
						if (tank)
						{
							tank->SetIsDead(true);
							m_pGameScene->SetGameState(CurrentGameState::Defeat);
						}

						m_pHitObject = nullptr;
					}
					
				}
				else if (other->GetTag().compare(L"Nest") == 0)
				{
					
					m_pGameScene->SetGameState(CurrentGameState::Defeat);
				}

				m_IsEnabled = false;
			}
		});

	SetTag(L"Shell");
}

void Shell::Update(const SceneContext& sceneContext)
{
	
	if (!m_IsEnabled)
	{
		if (m_pHitObject)
		{
			m_pGameScene->RemoveChild(m_pHitObject, true);
			m_pHitObject = nullptr;
		}
		m_pParent->RemoveChild(this, true);
		return;
	}

	float deltaTime = sceneContext.pGameTime->GetElapsed();

	m_CurrentLifeTime += deltaTime;

	if (m_CurrentLifeTime >= m_Lifetime)
	{
		m_pParent->RemoveChild(this, true);
	}
	else
	{
		m_pRigidBody->AddForce(XMFLOAT3{ m_Speed * deltaTime * m_Direction.x,0.0f,m_Speed * deltaTime * m_Direction.z }, PxForceMode::eIMPULSE);
	}

}


