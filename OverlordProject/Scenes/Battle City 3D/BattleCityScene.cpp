#include "stdafx.h"
#include "BattleCityScene.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/PlayerTank.h"
#include "Prefabs/EnemyTank.h"
#include "WorldBuilder/EnemySpawner.h"
#include "Prefabs/Hatch.h"
#include "Prefabs/ParticleAtLocation.h"
#include "WorldBuilder/ShellManager.h"
#include "WorldBuilder/EnvironmentBuilder.h"

BattleCityScene::BattleCityScene():
	GameScene(L"Battle City")
{

}

BattleCityScene::~BattleCityScene()
{
	SafeDelete(m_pLevelBuilder);
	SafeDelete(m_pEnvBuilder);
}

void BattleCityScene::Initialize()
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this);
	m_SceneContext.pLights->SetDirectionalLight(XMFLOAT3{ 6.897f,82.759f,13.793f }, XMFLOAT3{ -0.241f,-1.0f,-0.069f });

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	//m_SceneContext.settings.showInfoOverlay = false;

	m_SceneContext.settings.enableOnGUI = true;

	LockCamera();
	
	//Level
	m_pLevelBuilder = new LevelBuilder{this,1.2f};
	m_pLevelBuilder->AddLevel("Resources/Levels/Level1.bmp", 15, 15);
	m_pLevelBuilder->BuildNextLevel();

	m_pEnvBuilder = new EnvironmentBuilder{ this };
	m_pEnvBuilder->BuildLavaEnv();

	//Tank
	TankDesc tankDesc{ pDefaultMaterial };
	auto playerLoc = m_pLevelBuilder->GetPlayerStartHatch()->GetTransform()->GetPosition();
	m_pPlayerTank = new PlayerTank(playerLoc, XMFLOAT3{0.0f,0.0f,0.0f}, tankDesc);
	AddChild(m_pPlayerTank);

	
	auto enemyHatches = m_pLevelBuilder->GetEnemyStartHatches();
	m_pEnemySpawner = new EnemySpawner(enemyHatches, 20);
	AddChild(m_pEnemySpawner);

	m_pShellManager = new ShellManager();
	AddChild(m_pShellManager);


	ParticleEmitterSettings settings{};

	settings.velocity = { 0.f,0.f,0.f };
	settings.minSize = 0.1f;
	settings.maxSize = 2.2f;
	settings.minEnergy = 0.1f;
	settings.maxEnergy = 2.5f;
	settings.minScale = 1.2f;
	settings.maxScale = 2.0f;
	settings.minEmitterRadius = 0.5f;
	settings.maxEmitterRadius = 1.5f;
	settings.color = { 1.f,1.f,1.f, .6f };

	auto loc = m_pLevelBuilder->GetNestLocation();
	m_pDefeatParticle = new ParticleAtLocation{XMFLOAT3{loc.x,loc.y + 3.0f,loc.z},4.0f,settings,L"Textures/explosion.png"};
	AddChild(m_pDefeatParticle);


	settings.minSize = 1.f;
	settings.maxSize = 4.f;
	settings.minEnergy = 0.5f;
	settings.maxEnergy = 5.f;
	settings.minScale = 0.5f;
	settings.maxScale = 1.f;
	settings.minEmitterRadius = 0.5f;
	settings.maxEmitterRadius = 4.f;
	settings.color = { 1.f,1.f,1.f, .6f };

	loc = m_pLevelBuilder->GetLevelCenter();
	m_pVictoryParticle = new ParticleAtLocation{ XMFLOAT3{loc.x,loc.y + 3.0f,loc.z},100.0f,settings,L"Textures/fireworks.png" };
	AddChild(m_pVictoryParticle);
}

void BattleCityScene::Update()
{
	
	switch (m_GameState)
	{
	case CurrentGameState::Gameplay:
		break;
	case CurrentGameState::Victory:
		m_pVictoryParticle->Play();
		break;
	case CurrentGameState::Defeat:
		m_pDefeatParticle->Play();
		break;
	default:
		break;
	}

}

void BattleCityScene::Draw()
{
}

void BattleCityScene::OnGUI()
{
	m_pEnvBuilder->test();
}

void BattleCityScene::PostDraw()
{
}

void BattleCityScene::LockCamera()
{
	//Set a new fixed camera
	auto prevCamera = m_SceneContext.pCamera;
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(5.f, 26.f, 4.f);
	pFixedCamera->GetTransform()->Rotate(80.0f, 0.0f, 0.0f);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	//remove the old camera
	RemoveChild(prevCamera->GetGameObject(), true);
}
