#include "stdafx.h"
#include "ParticleAtLocation.h"

ParticleAtLocation::ParticleAtLocation(const XMFLOAT3& loc, float duration, const ParticleEmitterSettings& settings, const std::wstring& filePath):
	m_Duration{duration},
	m_Location{loc},
	m_Settings{settings},
	m_Path{filePath}
{
}


void ParticleAtLocation::Play()
{
	if (!m_HasBeenInitialized)
	{
		m_ShouldPlay = true;
	}
	
}



void ParticleAtLocation::Initialize(const SceneContext&)
{
	GetTransform()->Translate(m_Location);
}

void ParticleAtLocation::Update(const SceneContext& sceneContext)
{
	if (m_ShouldPlay)
	{
		if (!m_HasBeenInitialized)
		{
			m_pComponent = new ParticleEmitterComponent(m_Path, m_Settings, 100);
			AddComponent(m_pComponent);
			m_HasBeenInitialized = true;
		}

		float deltaTime = sceneContext.pGameTime->GetElapsed();

		m_CurrentDuration += deltaTime;
		if (m_CurrentDuration >= m_Duration)
		{
			GetScene()->RemoveChild(this, true);
			m_ShouldPlay = false;
			m_CurrentDuration = 0.0f;
		}
	}
}
