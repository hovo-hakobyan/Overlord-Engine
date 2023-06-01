#include "stdafx.h"
#include "BattleCityScene.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/PlayerTank.h"

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

	GameSceneExt::CreatePhysXGroundPlane(*this);

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	
	LockCamera();

	m_pLevelBuilder = new LevelBuilder{this,1};
	m_pLevelBuilder->AddLevel("Resources/Levels/Level1.bmp", 15, 15);
	m_pLevelBuilder->BuildNextLevel();

	auto pPlayerTank = new PlayerTank(m_pLevelBuilder->GetPlayerStartLocation(), XMFLOAT3{0.0f,180.0f,0.0f});
	AddChild(pPlayerTank);

}

void BattleCityScene::Update()
{
	
}

void BattleCityScene::Draw()
{
}

void BattleCityScene::OnGUI()
{
	//m_pLevelBuilder->temp();
}

void BattleCityScene::LockCamera()
{
	//Set a new fixed camera
	auto prevCamera = m_SceneContext.pCamera;
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(6.f, 19.f, 3.f);
	pFixedCamera->GetTransform()->Rotate(80.0f, 0.0f, 0.0f);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	//remove the old camera
	RemoveChild(prevCamera->GetGameObject(), true);
}
