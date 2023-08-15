#include "stdafx.h"
#include "BattleCityScene.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/PlayerTank.h"
#include "Prefabs/EnemyTank.h"
#include "Misc/EnemySpawner.h"
#include "Prefabs/Hatch.h"
#include "Misc/ShellManager.h"
#include "WorldBuilder/EnvironmentBuilder.h"
#include "Materials/LavaMaterial.h"
#include "Misc/PauseMenu.h"
#include "Misc/EndScreen.h"
#include "Misc/hud.h"
#include "Materials/Post/PostVignette.h"


BattleCityScene::BattleCityScene(const std::string& path):
	GameScene(L"BattleCity"),
	m_LevelPath{path}
{
	m_LevelPath.insert(0, "Resources/");
}

BattleCityScene::~BattleCityScene()
{
	SafeDelete(m_pLevelBuilder);
}

bool BattleCityScene::IsGamePaused() const
{
	return m_IsPaused;
}

void BattleCityScene::Initialize()
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this);
	m_SceneContext.pLights->SetDirectionalLight(XMFLOAT3{ 6.897f,82.759f,13.793f }, XMFLOAT3{ -0.241f,-1.0f,-0.069f });

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;

	m_SceneContext.settings.enableOnGUI = true;

	LockCamera();
	
	//Level
	m_pLevelBuilder = new LevelBuilder{this,1.2f};
	m_pLevelBuilder->AddLevel(m_LevelPath, 15, 15);
	m_pLevelBuilder->BuildNextLevel();

	EnvironmentBuilder::BuildLavaEnv(this,true);

	auto lavaMat = MaterialManager::Get()->CreateMaterial<LavaMaterial>();
	

	m_pGrowingLava = new ModelComponent(L"Meshes/GrowingLava.ovm");
	m_pGrowingLava->SetMaterial(lavaMat);
	auto pGround = new GameObject();
	AddChild(pGround);
	pGround->AddComponent(m_pGrowingLava);
	
	auto pTransform = m_pGrowingLava->GetTransform();
	pTransform->Scale(0.15f);
	pTransform->Rotate(0, -110, 0);
	pTransform->Translate(-8.0f, -2.5f, 8.0f);


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

	//input
	auto inputAction = InputAction(InputIds::Pause, InputState::pressed, VK_ESCAPE);
	m_SceneContext.pInput->AddInputAction(inputAction);

	//Pause menu
	m_pPauseMenu = new PauseMenu();
	AddChild(m_pPauseMenu);

	m_pEndScreen = new EndScreen();
	AddChild(m_pEndScreen);

	

	//Sound
	const auto pSoundManager = SoundManager::Get()->GetSystem();
	pSoundManager->createStream("Resources/Sounds/TankIdle.wav", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &m_pAmbient);
	m_pChannel->setVolume(0.2f);
	pSoundManager->playSound(m_pAmbient, nullptr, false, &m_pChannel);

	
	//Post processing
	m_pPostVignette = MaterialManager::Get()->CreateMaterial<PostVignette>();
	AddPostProcessingEffect(m_pPostVignette);

	//Hud
	m_pHud = new Hud();
	AddChild(m_pHud);
}

void BattleCityScene::Update()
{
	const auto input = m_SceneContext.pInput;
	if (input->IsActionTriggered(InputIds::Pause))
	{
		m_IsPaused = !m_IsPaused;

		if (m_IsPaused)
		{
			m_pPauseMenu->Show();
			SoundManager::Get()->GetSystem()->playSound(m_pAmbient, nullptr, true, &m_pChannel);
			
		}
		else
		{
			m_pPauseMenu->Hide();
			SoundManager::Get()->GetSystem()->playSound(m_pAmbient, nullptr, false, &m_pChannel);
		}
	}

	if (m_IsPaused)
	{
		return;
	}

	switch (m_GameState)
	{
	case CurrentGameState::Gameplay:
		break;
	case CurrentGameState::Victory:
		m_pEndScreen->ShowVictory();
		SoundManager::Get()->GetSystem()->playSound(m_pAmbient, nullptr, true, &m_pChannel);
		break;
	case CurrentGameState::Defeat:
		m_pEndScreen->ShowDefeat();
		SoundManager::Get()->GetSystem()->playSound(m_pAmbient, nullptr, true, &m_pChannel);
		break;
	default:
		break;
	}
	if (m_HasLavaFinishedGrowing)
	{
		return;
	}

	float deltaTime = m_SceneContext.pGameTime->GetElapsed();
	float frameGrowth = deltaTime * m_LavaGrowSpeed;

	auto pTransform = m_pGrowingLava->GetTransform();
	auto currentScale = pTransform->GetScale();
	if (currentScale.x > 1.2f)
	{
		m_HasLavaFinishedGrowing = true;
		return;
	}
	pTransform->Scale(currentScale.x + frameGrowth, currentScale.y, currentScale.z + frameGrowth);
	
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

void BattleCityScene::OnSceneDeactivated()
{
	m_HasLavaFinishedGrowing = false;

	auto pTransform = m_pGrowingLava->GetTransform();
	pTransform->Scale(0.08f,pTransform->GetScale().y,0.08f);

	SceneManager::Get()->RemoveGameScene(this,true);
}

void BattleCityScene::LockCamera()
{
	//Set a new fixed camera
	auto prevCamera = m_SceneContext.pCamera;
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(6.f, 26.f, 4.f);
	pFixedCamera->GetTransform()->Rotate(80.0f, 0.0f, 0.0f);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	//remove the old camera
	RemoveChild(prevCamera->GetGameObject(), true);
}
