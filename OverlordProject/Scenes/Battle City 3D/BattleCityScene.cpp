#include "stdafx.h"
#include "BattleCityScene.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/PlayerTank.h"
#include "Prefabs/EnemyTank.h"
#include "WorldBuilder/EnemySpawner.h"
#include "Prefabs/Hatch.h"

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
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this);
	m_SceneContext.pLights->SetDirectionalLight(XMFLOAT3{ 6.897f,82.759f,13.793f }, XMFLOAT3{ -0.241f,-1.0f,-0.069f });

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	
	LockCamera();

	//Level
	m_pLevelBuilder = new LevelBuilder{this,1.2f};
	m_pLevelBuilder->AddLevel("Resources/Levels/Level1.bmp", 15, 15);
	m_pLevelBuilder->BuildNextLevel();

	//Tank
	TankDesc tankDesc{ pDefaultMaterial };
	auto playerLoc = m_pLevelBuilder->GetPlayerStartHatch()->GetTransform()->GetPosition();
	m_pPlayerTank = new PlayerTank(playerLoc, XMFLOAT3{0.0f,0.0f,0.0f}, tankDesc,this);
	AddChild(m_pPlayerTank);

	
	auto enemyHatches = m_pLevelBuilder->GetEnemyStartHatches();
	m_pEnemySpawner = new EnemySpawner(enemyHatches, 20,this);
	AddChild(m_pEnemySpawner);

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

void BattleCityScene::PostDraw()
{
}

void BattleCityScene::LockCamera()
{
	//Set a new fixed camera
	auto prevCamera = m_SceneContext.pCamera;
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(8.f, 21.f, 4.f);
	pFixedCamera->GetTransform()->Rotate(80.0f, 0.0f, 0.0f);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	//remove the old camera
	RemoveChild(prevCamera->GetGameObject(), true);
}
