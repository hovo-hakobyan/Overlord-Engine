#include "stdafx.h"
#include "BattleCityScene.h"
#include "WorldBuilder/BMPLevelParser.h"

BattleCityScene::BattleCityScene():
	GameScene(L"Battle City")
{

}

void BattleCityScene::Initialize()
{
	auto pLevelParser = new BMPLevelParser();
	auto levelData = pLevelParser->GetLevelData("Resources/Levels/Level1.bmp", 13, 13);

	delete pLevelParser;
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
