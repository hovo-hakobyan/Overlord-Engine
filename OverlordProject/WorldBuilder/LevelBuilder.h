#pragma once
#include "BMPLevelParser.h"


struct LevelInfo
{
	uint8_t rows;
	uint8_t cols;
};
class DiffuseMaterial_Shadow;
class DiffuseMaterial;
class Hatch;
class LevelBuilder final
{
public:
	LevelBuilder(GameScene* gameScene, float tileSize);

	~LevelBuilder();
	LevelBuilder(const LevelBuilder& other) = delete;
	LevelBuilder(LevelBuilder&& other) noexcept = delete;
	LevelBuilder& operator=(const LevelBuilder& other) = delete;
	LevelBuilder& operator=(LevelBuilder&& other) noexcept = delete;

	void AddLevel(const std::string& filePath, uint8_t rows, uint8_t cols);
	void BuildNextLevel();
	void AddMainMenuLevel(const std::string& filePath, uint8_t rows, uint8_t cols);
	void BuildMainMenu();

	std::vector<class Hatch*> GetEnemyStartHatches() const;
	Hatch* GetPlayerStartHatch() const;
	const XMFLOAT3& GetNestLocation() const { return m_NestLocation; }
	const XMFLOAT3& GetLevelCenter() const { return m_LevelCenter; }

	static void BuildSandbox(GameScene* pGameScene, uint32_t rows, uint32_t cols,float tileSize);
private:
	std::vector<TileCollection> m_Levels;
	std::vector<LevelInfo*> m_LevelInfo;
	BMPLevelParser* m_LevelParser{};

	TileCollection m_MenuLevel;
	LevelInfo* m_MenuLevelInfo;

	int m_CurrentLevelIdx = -1;
	float m_TileSize{};
	GameScene* m_pGameScene;

	class GrassMaterial* m_pGrassMaterial{};
	class WaterMaterial* m_pWaterMaterial{};
	DiffuseMaterial_Shadow* m_pGroundMaterial{};
	DiffuseMaterial_Shadow* m_pSolidWallMaterial{};
	DiffuseMaterial_Shadow* m_pBorderWallMaterial{};
	DiffuseMaterial_Shadow* m_pBrickWallMaterial{};
	static DiffuseMaterial* m_pSandboxMat;

	std::vector<class Hatch*> m_pEnemySpawnHatches;
	Hatch* m_pPlayerSpawnHatch;

	void BuildBrickWalls(const XMFLOAT3& currentPos);
	
	XMFLOAT3 m_NestLocation{};
	XMFLOAT3 m_LevelCenter{};
};

