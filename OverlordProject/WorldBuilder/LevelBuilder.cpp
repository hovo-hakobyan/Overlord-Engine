#include "stdafx.h"
#include "LevelBuilder.h"
#include "Materials/GrassMaterial.h"
#include "Materials/SimpleDiffuse.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/WaterMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Hatch.h"



LevelBuilder::LevelBuilder(GameScene* gameScene,float tileSize):
	m_pGameScene{gameScene},
	m_TileSize{tileSize}
{
	m_LevelParser = new BMPLevelParser();
	m_pGrassMaterial = MaterialManager::Get()->CreateMaterial<GrassMaterial>();
	m_pWaterMaterial = MaterialManager::Get()->CreateMaterial<WaterMaterial>();

	m_pGroundMaterial = MaterialManager::Get()->CreateMaterial<SimpleDiffuseMaterial>();
	m_pGroundMaterial->SetDiffuseTexture(L"Textures/ground/soilAlbedo.tif");
	m_pGroundMaterial->SetNormalMapTexture(L"Textures/ground/soilNormal.tif");

	m_pSolidWallMaterial = MaterialManager::Get()->CreateMaterial<SimpleDiffuseMaterial>();
	m_pSolidWallMaterial->SetDiffuseTexture(L"Textures/wall/solidWallAlbedo.tif");
	m_pSolidWallMaterial->SetNormalMapTexture(L"Textures/wall/solidWallNormal.tif");

	m_pBrickWallMaterial = MaterialManager::Get()->CreateMaterial<SimpleDiffuseMaterial>();
	m_pBrickWallMaterial->SetDiffuseTexture(L"Textures/wall/brickAlbedo.tif");
	m_pBrickWallMaterial->SetNormalMapTexture(L"Textures/wall/brickNormal.tif");
	
	m_pBorderWallMaterial = MaterialManager::Get()->CreateMaterial<SimpleDiffuseMaterial>();
	m_pBorderWallMaterial->SetDiffuseTexture(L"Textures/wall/borderAlbedo.tif");
	m_pBorderWallMaterial->SetNormalMapTexture(L"Textures/wall/borderNormal.tif");

}

LevelBuilder::~LevelBuilder()
{
	SafeDelete(m_LevelParser);

	for (size_t i = 0; i < m_LevelInfo.size(); ++i)
	{
		SafeDelete(m_LevelInfo[i]);
	}

	for (size_t i = 0; i < m_Levels.size(); i++)
	{
		TileCollection tileCol = m_Levels[i];
		for (size_t j = 0; j < tileCol.size(); j++)
		{
			SafeDelete(tileCol[j]);
		}
	}
}

void LevelBuilder::AddLevel(const std::string& filePath, uint8_t rows, uint8_t cols)
{
	auto levelData = m_LevelParser->GetLevelData(filePath, rows, cols);
	m_Levels.push_back(levelData );
	m_LevelInfo.push_back(new LevelInfo{ rows,cols });
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
	auto pMat = PxGetPhysics().createMaterial(1.0f, 1.0f, 0.f);
	for (int row = 0; row < nrRows; ++row)
	{
		for (int col = 0; col < nrCols; ++col)
		{
			int idx = row * nrCols + col;
			auto currentTileType = *m_Levels[m_CurrentLevelIdx][idx];

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
						auto pTerrainObj = new GameObject();
						m_pGameScene->AddChild(pTerrainObj);
						pTerrainModel = new ModelComponent(L"Meshes/SolidWall.ovm");
						pTerrainModel->SetMaterial(m_pBrickWallMaterial);
						pTerrainObj->GetTransform()->Translate(currentPos.x, currentPos.y + wallSize.y / 2.0f, currentPos.z);	
						pTerrainObj->GetTransform()->Scale(m_TileSize, wallSize.y / 2.0f, m_TileSize);
						auto pRigidBody = pTerrainObj->AddComponent(new RigidBodyComponent(true));
						pRigidBody->AddCollider(wallGeo, *pMat);
						pTerrainObj->AddComponent(pTerrainModel);
						
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
						m_EnemySpawnLocations.push_back(currentPos);
					}
					break;
				case TileTypes::PlayerSpawn:
				{
					auto hatch = new Hatch(currentPos, XMFLOAT3{ 0.0f,0.0f,0.0f },m_TileSize, L"Meshes/Door.ovm");
					hatch->CreateMaterial(L"Textures/door/doorDiffusePlayer.png", L"Textures/door/doorNormal.png");
					m_pGameScene->AddChild(hatch);
					m_PlayerSpawnLocation = currentPos;			
				}
				break;
				}
			
			}
			currentPos.x += m_TileSize;
		}
		currentPos.x = 0.0f;
		currentPos.z += m_TileSize;
	}
}

std::vector<XMFLOAT3> LevelBuilder::GetEnemyStartLocations() const
{
	return m_EnemySpawnLocations;
}

XMFLOAT3 LevelBuilder::GetPlayerStartLocation() const
{
	return m_PlayerSpawnLocation;
}

void LevelBuilder::temp()
{
	m_pBrickWallMaterial->DrawImGui();
}

