#include "stdafx.h"
#include "Shell.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "BaseTank.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"
#include "WorldBuilder/ShellManager.h"


Shell::Shell(const XMFLOAT3& pos, const XMFLOAT3& rot, const std::wstring& parentTag, const XMFLOAT3& dir):
	m_Loc{pos},
	m_Rot{rot},
	m_Direction{dir},
	m_ParentTag{parentTag}
{

}

void Shell::Initialize(const SceneContext&)
{
	m_pGameScene =static_cast<BattleCityScene*>(GetScene());

	auto& physX = PxGetPhysics();
	auto pPhysicsMaterial = physX.createMaterial(1.0f, 1.0f, .0f);

	auto pTransform = GetTransform();
	pTransform->Translate(m_Loc);
	pTransform->Rotate(m_Rot);
	pTransform->Scale(0.1f, 0.05f, 0.1f);

	m_pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pMat->SetDiffuseTexture(L"Textures/tank/shell.png");

	m_pModelComponent = new ModelComponent(L"Meshes/Shell.ovm");
	m_pModelComponent->SetMaterial(m_pMat);
	AddComponent(m_pModelComponent);
	
	PxBoxGeometry shellGeo{ 0.2f,0.2f,0.2f };
	
	m_pRigidBody = new RigidBodyComponent();
	AddComponent(m_pRigidBody);
	m_pRigidBody->AddCollider(shellGeo, *pPhysicsMaterial,true);
	m_pRigidBody->SetConstraint(RigidBodyConstraint::AllRot | RigidBodyConstraint::TransY, false);
	
	//Particle System
	ParticleEmitterSettings settings{};

	settings.velocity = { 0.f,0.f,0.f };
	settings.minSize = 0.1f;
	settings.maxSize = 1.2f;
	settings.minEnergy = 0.1f;
	settings.maxEnergy = 0.5f;
	settings.minScale = 0.2f;
	settings.maxScale = 1.0f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f,1.f,1.f, .6f };

	m_pShotParticleEmitter = new ParticleEmitterComponent(L"Textures/fire.png", settings, 58);
	AddComponent(m_pShotParticleEmitter);

	ParticleEmitterSettings expSettings{};

	expSettings.velocity = { 0.f,0.f,0.f };
	expSettings.minSize = 0.5f;
	expSettings.maxSize = 1.5f;
	expSettings.minEnergy = .5f;
	expSettings.maxEnergy = 1.5f;
	expSettings.minScale = 3.f;
	expSettings.maxScale = 4.0f;
	expSettings.minEmitterRadius = .2f;
	expSettings.maxEmitterRadius = .5f;
	expSettings.color = { 1.f,1.f,1.f, .6f };
	m_pExplosionEmitter = new ParticleEmitterComponent(L"Textures/explosion.png", expSettings, 100);

	SetOnTriggerCallBack([=](GameObject*, GameObject* other, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				if (m_IsEnabled)
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
								AddComponent(m_pExplosionEmitter);
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
								AddComponent(m_pExplosionEmitter);
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

 				
			}
		});

	SetTag(L"Shell");


	
}

void Shell::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	if (!m_IsEnabled)
	{
		if (m_ShouldDisable)
		{
			Disable();
			m_ShouldDisable = false;
		}
		m_CurrentDisabledTime += deltaTime;
		if (m_CurrentDisabledTime >= m_DisabledLifetime)
		{
			if (!m_IsDead)
			{
				m_IsDead = true;
				auto parent = GetParent();
				parent->RemoveChild(this, true);			
			}
		}
		return;
	}

	m_CurrentLifeTime += deltaTime;

	if (m_CurrentLifeTime >= m_Lifetime)
	{
		if (!m_IsDead)
		{
			GetParent()->RemoveChild(this, true);
			m_IsDead = true;
		}
		
	}
	else
	{
		m_pRigidBody->AddForce(XMFLOAT3{ m_Speed * deltaTime * m_Direction.x,0.0f,m_Speed * deltaTime * m_Direction.z }, PxForceMode::eIMPULSE);
	}

	if (m_CurrentLifeTime >=m_ParticleLifeTime)
	{
		RemoveComponent(m_pShotParticleEmitter);
		m_ParticleLifeTime = m_Lifetime * 2;
	}
}

void Shell::Disable()
{
	m_pRigidBody->PutToSleep();
	m_pModelComponent->GetTransform()->Scale(0.0f, 0.0f, 0.0f);

	if (m_pHitObject)
	{
		GetScene()->RemoveChild(m_pHitObject, true);
	}
}


