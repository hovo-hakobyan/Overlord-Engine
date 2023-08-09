#pragma once
#include <random>
class Hatch;
class BattleCityScene;
class EnemySpawner final : public GameObject
{
public:
	EnemySpawner(std::vector<Hatch*> pHatches, int nrEnemiesToSpawn);
	~EnemySpawner() override = default;
	EnemySpawner(const EnemySpawner& other) = delete;
	EnemySpawner(EnemySpawner&& other) noexcept = delete;
	EnemySpawner& operator=(const EnemySpawner& other) = delete;
	EnemySpawner& operator=(EnemySpawner&& other) noexcept = delete;
protected:
	virtual void Initialize(const SceneContext& /*sceneContext*/) override;
	virtual void Update(const SceneContext&) override;
private:
	int m_MaxEnemiesToSpawn{};
	int m_CurrentEnemiesSpawned{};
	std::vector<XMFLOAT3> m_SpawnLocations;
	BattleCityScene* m_pGameScene{};
	float m_MaxSpawnCooldown{3.0f};
	float m_CurrentSpawnCooldown{0.0f};

	float m_CooldownDecrease{0.13f};
	int m_DecreaseCooldownAfterSpawns{};

	void SpawnEnemy();

	std::mt19937 m_Generator{};
	PxMaterial* m_pPxMat{};

	void CheckForVictory();
};

