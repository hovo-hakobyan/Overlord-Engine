#include "stdafx.h"
#include "EnemySpawner.h"
#include "Prefabs/Hatch.h"
#include "Prefabs/EnemyTank.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"


EnemySpawner::EnemySpawner(std::vector<Hatch*> pHatches, int nrEnemiesToSpawn):
	m_MaxEnemiesToSpawn{nrEnemiesToSpawn}
{
	m_DecreaseCooldownAfterSpawns = nrEnemiesToSpawn / 5;
	for (size_t i = 0; i < pHatches.size(); i++)
	{
		m_SpawnLocations.push_back(pHatches[i]->GetTransform()->GetPosition());
	}

	std::random_device rd;
	m_Generator.seed(rd());
}

void EnemySpawner::Initialize(const SceneContext&)
{
	m_pGameScene = static_cast<BattleCityScene*>(GetScene());
	m_pPxMat = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
}

void EnemySpawner::Update(const SceneContext& sceneContext)
{
	if (m_pGameScene->GetGameState() != CurrentGameState::Gameplay || m_pGameScene->IsGamePaused())
	{
		return;
	}

	float deltaTime = sceneContext.pGameTime->GetElapsed();
	m_CurrentSpawnCooldown += deltaTime;
	if (m_CurrentSpawnCooldown >= m_MaxSpawnCooldown)
	{
		if (m_CurrentEnemiesSpawned < m_MaxEnemiesToSpawn)
		{
			SpawnEnemy();
			++m_CurrentEnemiesSpawned;
			m_CurrentSpawnCooldown = 0.0f;

			if (m_CurrentEnemiesSpawned % m_DecreaseCooldownAfterSpawns == 0)
			{
				//make the spawn faster
				m_MaxSpawnCooldown -= m_CooldownDecrease;
			}
		}
		else
		{
			CheckForVictory();
		}
	}
}

void EnemySpawner::SpawnEnemy()
{
	std::uniform_int_distribution<int> distribution(0, static_cast<int>(m_SpawnLocations.size()) - 1);
	int randomIndex = distribution(m_Generator);
	auto spawnLocation = m_SpawnLocations[randomIndex];
	
	//Enemy test
	TankDesc enemyDesc{ m_pPxMat };
	auto pEnemyTank = new EnemyTank(spawnLocation, XMFLOAT3{ 0.0f,0.0f,0.0f }, enemyDesc);
	m_pGameScene->AddChild(pEnemyTank);
	
}

void EnemySpawner::CheckForVictory()
{
	if (BaseTank::EnemyCounter == 0)
	{
		m_pGameScene->SetGameState(CurrentGameState::Victory);
	}
}
