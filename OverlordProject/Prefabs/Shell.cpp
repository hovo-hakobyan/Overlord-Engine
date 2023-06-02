#include "stdafx.h"
#include "Shell.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

Shell::Shell(const XMFLOAT3& loc, const XMFLOAT3& rot, const XMFLOAT3& dir, GameScene* parent):
	m_Location{loc},
	m_Rotation{rot},
	m_Direction{dir},
	m_pParent{parent}
{
}

void Shell::Initialize(const SceneContext&)
{
	
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
					m_pOther = other;
				}

				m_IsEnabled = false;
			}
		});
}

void Shell::Update(const SceneContext& sceneContext)
{
	if (!m_IsEnabled)
	{
		if (m_pOther)
		{
			m_pParent->RemoveChild(m_pOther, true);
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


