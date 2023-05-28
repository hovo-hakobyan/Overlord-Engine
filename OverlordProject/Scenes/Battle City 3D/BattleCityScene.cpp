#include "stdafx.h"
#include "BattleCityScene.h"
#include "WorldBuilder/LevelBuilder.h"

BattleCityScene::BattleCityScene():
	GameScene(L"Battle City")
{

}

BattleCityScene::~BattleCityScene()
{
	SafeDelete(m_pLevelBuilder);
}

void BattleCityScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;

	m_pLevelBuilder = new LevelBuilder{this,1};
	m_pLevelBuilder->AddLevel("Resources/Levels/Level1.bmp", 13, 13);
	m_pLevelBuilder->BuildNextLevel();
}

void BattleCityScene::Update()
{

}

void BattleCityScene::Draw()
{
}

void BattleCityScene::OnGUI()
{

}
