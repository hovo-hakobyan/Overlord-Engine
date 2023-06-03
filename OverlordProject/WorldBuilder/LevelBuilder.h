#pragma once
#include "BMPLevelParser.h"


struct LevelInfo
{
	uint8_t rows;
	uint8_t cols;
};
class BattleCityScene;
class LevelBuilder final
{
public:
	LevelBuilder(BattleCityScene* gameScene, float tileSize);
	~LevelBuilder();
	LevelBuilder(const LevelBuilder& other) = delete;
	LevelBuilder(LevelBuilder&& other) noexcept = delete;
	LevelBuilder& operator=(const LevelBuilder& other) = delete;
	LevelBuilder& operator=(LevelBuilder&& other) noexcept = delete;

	void AddLevel(const std::string& filePath, uint8_t rows, uint8_t cols);
	void BuildNextLevel();

	std::vector<class Hatch*> GetEnemyStartHatches() const;
	Hatch* GetPlayerStartHatch() const;

private:
	std::vector<TileCollection> m_Levels;
	std::vector<LevelInfo*> m_LevelInfo;
	BMPLevelParser* m_LevelParser{};

	int m_CurrentLevelIdx = -1;
	float m_TileSize{};
	BattleCityScene* m_pGameScene;

	class GrassMaterial* m_pGrassMaterial{};
	class WaterMaterial* m_pWaterMaterial{};
	class SimpleDiffuseMaterial* m_pGroundMaterial{};
	class SimpleDiffuseMaterial* m_pSolidWallMaterial{};
	class SimpleDiffuseMaterial* m_pBorderWallMaterial{};

	std::vector<class Hatch*> m_pEnemySpawnHatches;
	Hatch* m_pPlayerSpawnHatch;

	void BuildBrickWalls(const XMFLOAT3& currentPos);
	
};

