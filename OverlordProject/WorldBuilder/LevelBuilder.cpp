#include "stdafx.h"
#include "LevelBuilder.h"
#include "Materials/GrassMaterial.h"
#include "Materials/SimpleDiffuse.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/WaterMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Hatch.h"
#include "Prefabs/BrickWall.h"
#include "Prefabs/Nest.h"

DiffuseMaterial* LevelBuilder::m_pSandboxMat{ nullptr };
LevelBuilder::LevelBuilder(GameScene* gameScene,float tileSize):
	m_pGameScene{gameScene},
	m_TileSize{tileSize}
{
	m_LevelParser = new BMPLevelParser();
	m_pGrassMaterial = MaterialManager::Get()->CreateMaterial<GrassMaterial>();
	m_pWaterMaterial = MaterialManager::Get()->CreateMaterial<WaterMaterial>();

	m_pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pGroundMaterial->SetDiffuseTexture(L"Textures/ground/soilAlbedo.tif");
	
	m_pSolidWallMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pSolidWallMaterial->SetDiffuseTexture(L"Textures/wall/solidWallAlbedo.tif");

	m_pBorderWallMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pBorderWallMaterial->SetDiffuseTexture(L"Textures/wall/borderAlbedo.tif");

	m_pBrickWallMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pBrickWallMaterial->SetDiffuseTexture(L"Textures/wall/brickAlbedo.tif");
}


LevelBuilder::~LevelBuilder()
{
	SafeDelete(m_LevelParser);

	if (!m_LevelInfo.empty())
	{
		for (size_t i = 0; i < m_LevelInfo.size(); ++i)
		{
			SafeDelete(m_LevelInfo[i]);
		}
	}
	
	if (!m_Levels.empty())
	{
		for (size_t i = 0; i < m_Levels.size(); i++)
		{
			TileCollection tileCol = m_Levels[i];
			for (size_t j = 0; j < tileCol.size(); j++)
			{
				SafeDelete(tileCol[j]);
			}
		}
	}
	
	if (!m_MenuLevel.empty())
	{
		for (size_t i = 0; i < m_MenuLevel.size(); i++)
		{
			SafeDelete(m_MenuLevel[i]);
		}

		SafeDelete(m_MenuLevelInfo);
	}

}

void LevelBuilder::AddLevel(const std::string& filePath, uint8_t rows, uint8_t cols)
{
	auto levelData = m_LevelParser->GetLevelData(filePath, rows, cols);
	m_Levels.push_back(levelData );
	m_LevelInfo.push_back(new LevelInfo{ rows,cols });
}

void LevelBuilder::AddMainMenuLevel(const std::string& filePath, uint8_t rows, uint8_t cols)
{
	auto levelData = m_LevelParser->GetLevelData(filePath, rows, cols);
	m_MenuLevel = levelData;
	m_MenuLevelInfo = new LevelInfo{ rows,cols };
}

void LevelBuilder::BuildMainMenu()
{
	if (m_MenuLevel.empty() || !m_MenuLevelInfo)
	{
		return;
	}

	const int nrCols = m_MenuLevelInfo->cols;
	const int nrRows = m_MenuLevelInfo->rows;
	XMFLOAT3 currentPos{};
	int middleIdx = nrCols * nrRows / 2;
	auto pMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);

	for (int row = 0; row < nrRows; ++row)
	{
		for (int col = 0; col < nrCols; ++col)
		{
			int idx = row * nrCols + col;
			auto currentTileType = *m_MenuLevel[idx];
			if (idx == middleIdx)
			{
				m_LevelCenter = currentPos;
			}
			//Create the ground
			if (currentTileType != TileTypes::BorderWall && currentTileType != TileTypes::PlayerSpawn)
			{
				const auto pGroundModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
				pGroundModel->SetMaterial(m_pGroundMaterial);
				auto pGround = new GameObject();
				m_pGameScene->AddChild(pGround);
				pGround->AddComponent(pGroundModel);
				pGround->GetTransform()->Scale(m_TileSize);
				pGround->GetTransform()->Translate(currentPos);

			}

			//Create the terrain type on top of the ground

			if (currentTileType != TileTypes::Ground)
			{
				ModelComponent* pTerrainModel{};

				XMFLOAT3 borderWallSize{ m_TileSize,2.5f,m_TileSize };

				PxBoxGeometry borderWallGeo{ borderWallSize.x / 2.0f,borderWallSize.y / 2.0f ,borderWallSize.z / 2.0f };

				switch (currentTileType)
				{

				break;
				case TileTypes::BorderWall:
				{
					auto pTerrainObj = new GameObject();
					m_pGameScene->AddChild(pTerrainObj);
					pTerrainModel = new ModelComponent(L"Meshes/SolidWall.ovm");
					pTerrainModel->SetMaterial(m_pBorderWallMaterial);
					pTerrainObj->GetTransform()->Translate(currentPos.x, currentPos.y + borderWallSize.y / 2.0f, currentPos.z);
					pTerrainObj->GetTransform()->Scale(m_TileSize, borderWallSize.y / 2.0f, m_TileSize);
					auto pRigidBody = pTerrainObj->AddComponent(new RigidBodyComponent(true));
					pRigidBody->AddCollider(borderWallGeo, *pMat);
					pTerrainObj->AddComponent(pTerrainModel);
				}
				break;
				case TileTypes::Grass:
				{
					auto pTerrainObj = new GameObject();
					m_pGameScene->AddChild(pTerrainObj);
					pTerrainModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
					pTerrainModel->SetMaterial(m_pGrassMaterial);
					pTerrainObj->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z);
					pTerrainObj->GetTransform()->Scale(m_TileSize, 1.0f, m_TileSize);
					pTerrainObj->AddComponent(pTerrainModel);
				}
				break;

				case TileTypes::PlayerSpawn:
				{
					auto hatch = new Hatch(currentPos, XMFLOAT3{ 0.0f,0.0f,0.0f }, m_TileSize, L"Meshes/Door.ovm");
					hatch->CreateMaterial(L"Textures/door/doorDiffusePlayer.png", L"Textures/door/doorNormal.png");
					m_pGameScene->AddChild(hatch);
					m_pPlayerSpawnHatch = hatch;
				}
				break;
				case TileTypes::Base:
					auto nest = new Nest(currentPos);
					m_pGameScene->AddChild(nest);
					m_NestLocation = currentPos;
					break;
				}

			}
			currentPos.x += m_TileSize;
		}
		currentPos.x = 0.0f;
		currentPos.z += m_TileSize;
	}
}

void LevelBuilder::BuildNextLevel()
{
	++m_CurrentLevelIdx;

	if (m_Levels[m_CurrentLevelIdx].empty() || !m_LevelInfo[m_CurrentLevelIdx])
	{
		return;
	}

	const int nrCols = m_LevelInfo[m_CurrentLevelIdx]->cols;
	const int nrRows = m_LevelInfo[m_CurrentLevelIdx]->rows;
	XMFLOAT3 currentPos{};
	int middleIdx = nrCols * nrRows / 2;
	auto pMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	for (int row = 0; row < nrRows; ++row)
	{
		for (int col = 0; col < nrCols; ++col)
		{
			int idx = row * nrCols + col;
			auto currentTileType = *m_Levels[m_CurrentLevelIdx][idx];
			if (idx == middleIdx)
			{
				m_LevelCenter = currentPos;
			}
			//Create the ground, except if the tile is water tile, solid wall tile, border tile or spawn tile
			if (currentTileType != TileTypes::Water && currentTileType != TileTypes::SolidWall && currentTileType != TileTypes::BorderWall
				&& currentTileType != TileTypes::PlayerSpawn && currentTileType != TileTypes::EnemySpawn)
			{
				const auto pGroundModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
				pGroundModel->SetMaterial(m_pGroundMaterial);
				auto pGround = new GameObject();
				m_pGameScene->AddChild(pGround);
				pGround->AddComponent(pGroundModel);
				pGround->GetTransform()->Scale(m_TileSize);
				pGround->GetTransform()->Translate(currentPos);

			}

			//Create the terrain type on top of the ground
			
			if (currentTileType != TileTypes::Ground)
			{
				ModelComponent* pTerrainModel{};

				XMFLOAT3 wallSize{ m_TileSize,1.5f,m_TileSize };
				XMFLOAT3 borderWallSize{ m_TileSize,2.5f,m_TileSize };

				PxBoxGeometry borderWallGeo{ borderWallSize.x / 2.0f,borderWallSize.y / 2.0f ,borderWallSize.z / 2.0f };
				PxBoxGeometry wallGeo{ wallSize.x / 2.f,wallSize.y / 2.f, wallSize.z / 2.f };
				
				switch (currentTileType)
				{
				case TileTypes::SolidWall:
					{
						auto pTerrainObj = new GameObject();
						m_pGameScene->AddChild(pTerrainObj);
						pTerrainModel = new ModelComponent(L"Meshes/SolidWall.ovm");
						pTerrainModel->SetMaterial(m_pSolidWallMaterial);
						pTerrainObj->GetTransform()->Translate(currentPos.x,currentPos.y + wallSize.y / 2.0f,currentPos.z);
						pTerrainObj->GetTransform()->Scale(m_TileSize, wallSize.y / 2.0f, m_TileSize);
						auto pRigidBody = pTerrainObj->AddComponent(new RigidBodyComponent(true));
						pRigidBody->AddCollider(wallGeo, *pMat);
						pTerrainObj->AddComponent(pTerrainModel);
					}
					break;
				case TileTypes::BrickWall:
					{
						BuildBrickWalls(currentPos);
					}			
					break;
					case TileTypes::BorderWall:
					{
						auto pTerrainObj = new GameObject();
						m_pGameScene->AddChild(pTerrainObj);
						pTerrainModel = new ModelComponent(L"Meshes/SolidWall.ovm");
						pTerrainModel->SetMaterial(m_pBorderWallMaterial);
						pTerrainObj->GetTransform()->Translate(currentPos.x, currentPos.y + borderWallSize.y / 2.0f, currentPos.z);
						pTerrainObj->GetTransform()->Scale(m_TileSize, borderWallSize.y / 2.0f, m_TileSize);
						auto pRigidBody = pTerrainObj->AddComponent(new RigidBodyComponent(true));
						pRigidBody->AddCollider(borderWallGeo, *pMat);
						pTerrainObj->AddComponent(pTerrainModel);
					}
					break;
				case TileTypes::Grass:
					{
						auto pTerrainObj = new GameObject();
						m_pGameScene->AddChild(pTerrainObj);
						pTerrainModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
						pTerrainModel->SetMaterial(m_pGrassMaterial);
						pTerrainObj->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z);
						pTerrainObj->GetTransform()->Scale(m_TileSize, 1.0f, m_TileSize);
						pTerrainObj->AddComponent(pTerrainModel);
					}					
					break;
				case TileTypes::Water:
					{
						auto pTerrainObj = new GameObject();
						m_pGameScene->AddChild(pTerrainObj);
						pTerrainModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
						pTerrainObj->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z);
						pTerrainObj->GetTransform()->Scale(m_TileSize, 1.0f, m_TileSize);
						pTerrainModel->SetMaterial(m_pWaterMaterial);
						pTerrainObj->AddComponent(pTerrainModel);
					}
						
					break;
				case TileTypes::EnemySpawn:
					{
						auto hatch = new Hatch(currentPos, XMFLOAT3{ 0.0f,0.0f,0.0f },m_TileSize, L"Meshes/Door.ovm");
						hatch->CreateMaterial(L"Textures/door/doorDiffuseEnemy.png", L"Textures/door/doorNormal.png");
						m_pGameScene->AddChild(hatch);
						m_pEnemySpawnHatches.push_back(hatch);
					}
					break;
				case TileTypes::PlayerSpawn:
				{
					auto hatch = new Hatch(currentPos, XMFLOAT3{ 0.0f,0.0f,0.0f },m_TileSize, L"Meshes/Door.ovm");
					hatch->CreateMaterial(L"Textures/door/doorDiffusePlayer.png", L"Textures/door/doorNormal.png");
					m_pGameScene->AddChild(hatch);
					m_pPlayerSpawnHatch = hatch;			
				}
				break;
				case TileTypes::Base:
					auto nest = new Nest(currentPos);
					m_pGameScene->AddChild(nest);
					m_NestLocation = currentPos;
					break;
				}
			
			}
			currentPos.x += m_TileSize;
		}
		currentPos.x = 0.0f;
		currentPos.z += m_TileSize;
	}
}



std::vector<Hatch*> LevelBuilder::GetEnemyStartHatches() const
{
	return m_pEnemySpawnHatches;
}

Hatch* LevelBuilder::GetPlayerStartHatch() const
{
	return m_pPlayerSpawnHatch;
}

void LevelBuilder::BuildSandbox(GameScene* pGameScene, uint32_t rows, uint32_t cols,float tileSize)
{
	if (!m_pSandboxMat)
	{
		m_pSandboxMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
		m_pSandboxMat->SetDiffuseTexture(L"Textures/ground/soilAlbedo.tif");
	}
	XMFLOAT3 currentPos{};

	for (uint32_t row = 0; row < rows; ++row)
	{
		for (uint32_t col = 0; col < cols; ++col)
		{
			const auto pGroundModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
			pGroundModel->SetMaterial(m_pSandboxMat);
			auto pGround = new GameObject();
			pGameScene->AddChild(pGround);
			pGround->AddComponent(pGroundModel);
			pGround->GetTransform()->Scale(tileSize);
			pGround->GetTransform()->Translate(currentPos);

			currentPos.x += tileSize;
		}
		currentPos.x = 0.0f;
		currentPos.z += tileSize;
	}
}


void LevelBuilder::BuildBrickWalls(const XMFLOAT3& currentPos)
{
	constexpr int rows = 2;
	constexpr int cols = 2;
	float width = m_TileSize / rows;
	
	XMFLOAT3 pos = currentPos;
	constexpr float wallHeight = 1.25f;
	XMFLOAT3 size{ width, width * wallHeight * 2,width };

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{	
			auto brickWall = new BrickWall(pos, size, m_pBrickWallMaterial);
			m_pGameScene->AddChild(brickWall);
			pos.x += size.x;
		}
		pos.x = currentPos.x;
		pos.z += size.z;
	}

	
}

