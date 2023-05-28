#include "stdafx.h"
#include "LevelBuilder.h"
#include "Materials/GrassMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/WaterMaterial.h"



LevelBuilder::LevelBuilder(GameScene* gameScene,float tileSize):
	m_pGameScene{gameScene},
	m_TileSize{tileSize}
{
	m_LevelParser = new BMPLevelParser();
	m_pGrassMaterial = MaterialManager::Get()->CreateMaterial<GrassMaterial>();
	m_pWaterMaterial = MaterialManager::Get()->CreateMaterial<WaterMaterial>();
	m_pDefaultMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pDefaultMaterial->SetDiffuseTexture(L"Textures/GroundBrick.jpg");
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
	for (int row = 0; row < nrRows; ++row)
	{
		for (int col = 0; col < nrCols; ++col)
		{
			int idx = row * nrCols + col;
			auto currentTileType = *m_Levels[m_CurrentLevelIdx][idx];

			const auto pModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
			
			switch (currentTileType)
			{
			case TileTypes::Ground:
				pModel->SetMaterial(m_pDefaultMaterial);
				break;
			case TileTypes::SolidWall:
				pModel->SetMaterial(m_pDefaultMaterial);
				break;
			case TileTypes::BrickWall:
				pModel->SetMaterial(m_pDefaultMaterial);
				break;
			case TileTypes::Grass:
				pModel->SetMaterial(m_pGrassMaterial);
				break;
			case TileTypes::Water:
				pModel->SetMaterial(m_pWaterMaterial);
				break;
			}

			auto pGround = new GameObject();
			m_pGameScene->AddChild(pGround);
			pGround->AddComponent(pModel);
			pGround->GetTransform()->Scale(m_TileSize);
			pGround->GetTransform()->Translate(currentPos);
			
			currentPos.x += m_TileSize;
		}
		currentPos.x = 0.0f;
		currentPos.z += m_TileSize;
	}
}

