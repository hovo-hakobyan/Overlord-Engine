#include "stdafx.h"
#include "Nest.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"

Nest::Nest(const XMFLOAT3& loc):
	m_Location{loc}
{
	
}

void Nest::Initialize(const SceneContext&)
{
	m_pGameScene = static_cast<BattleCityScene*>(GetScene());
	auto pTransform = GetTransform();
	pTransform->Translate(m_Location);
	pTransform->Rotate(0.0f, 180.f, 0.0f);
	pTransform->Scale(0.07f, 0.1f, 0.07f);

	auto pxMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	m_pModelComponent = new ModelComponent(L"Meshes/king.ovm");
	AddComponent(m_pModelComponent);

	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/King.png");
	m_pModelComponent->SetMaterial(m_pMaterial);
	m_pModelComponent->GetTransform()->Translate(m_Location.x, m_Location.y, m_Location.z - 0.3f);

	
	m_pAnimator = m_pModelComponent->GetAnimator();
	m_pAnimator->SetAnimation(0);
	m_pAnimator->Play();
	m_MaxAnimTimer = m_SaluteTimer;

	

	m_pRigidBody = AddComponent(new RigidBodyComponent(true));
	m_pRigidBody->AddCollider(PxBoxGeometry{ 0.5f,1.5f,0.5f }, *pxMat);
	
	SetTag(L"Nest");
}

void Nest::Update(const SceneContext& sceneContext)
{
	if (m_FinishedFinalAnimation)
	{
		return;
	}

	float deltaTime = sceneContext.pGameTime->GetElapsed();

	if (m_pAnimator->IsPlaying())
	{
		if (m_pAnimator->GetClipName().compare(L"Scared") != 0)
		{
			m_CurrentAnimTimer += deltaTime;
			if (m_CurrentAnimTimer >= m_MaxAnimTimer)
			{
				if (m_pAnimator->GetClipName().compare(L"Death") == 0 || m_pAnimator->GetClipName().compare(L"Victory") == 0)
				{
					m_FinishedFinalAnimation = true;
					m_pAnimator->Pause();
					return;
				}

				m_pAnimator->SetAnimation(L"Scared");

				m_CurrentAnimTimer = 0.0f;

			}
		}
	
	}


	if (m_pGameScene->GetGameState() == CurrentGameState::Defeat)
	{
		if (m_pAnimator->GetClipName().compare(L"Death") != 0)
		{
			GetTransform()->Rotate(0.0f, -90.0f, 0.0f);
			m_CurrentAnimTimer = 0.0f;
			m_MaxAnimTimer = m_DeathTimer;
			m_pAnimator->SetAnimation(L"Death");
			m_pAnimator->Play();
		
		}	
	}
	else if (m_pGameScene->GetGameState() == CurrentGameState::Victory)
	{
		if (m_pAnimator->GetClipName().compare(L"Victory") != 0)
		{
			m_CurrentAnimTimer = 0.0f;
			m_MaxAnimTimer = m_VictoryTimer;
			m_pAnimator->SetAnimation(L"Victory");
			m_pAnimator->Play();

		}
	}

}
