#include "stdafx.h"
#include "MainMenu.h"
#include "Prefabs/WorldButton.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/Hatch.h"
#include "Prefabs/PlayerTank.h"
#include "Prefabs/CubePrefab.h"
#include "WorldBuilder/EnvironmentBuilder.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/FlagDAE.h"

MainMenu::MainMenu():
	GameScene(L"MainMenu")
{
}

MainMenu::~MainMenu()
{
	SafeDelete(m_pLevelBuilder);
}

void MainMenu::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this);

	LockCamera();

	m_pPlayButton = new WorldButton("Textures/Menu/play.png");
	AddChild(m_pPlayButton);

	m_pExitButton = new WorldButton("Textures/Menu/exit.png");
	AddChild(m_pExitButton);

	//Level
	m_pLevelBuilder = new LevelBuilder{ this,1.2f };
	m_pLevelBuilder->AddMainMenuLevel("Resources/Levels/Menu/MainMenu.bmp", 15, 15);
	m_pLevelBuilder->BuildMainMenu();

	auto center = m_pLevelBuilder->GetLevelCenter();
	center.y -= 0.1f;
	center.x -= 2.5f;

	auto transform = m_pPlayButton->GetTransform();
	transform->Translate(center);
	transform->Rotate(0, 90, 0);

	
	center.x += 4.5f;
	transform = m_pExitButton->GetTransform();
	transform->Translate(center);
	transform->Rotate(0, 90, 0);

	//Tank
	TankDesc tankDesc{ pDefaultMaterial };
	tankDesc.isCombat = false;
	tankDesc.maxMoveSpeed = 6.0f;
	m_PlayerStart = m_pLevelBuilder->GetPlayerStartHatch()->GetTransform()->GetPosition();
	m_pPlayerTank = new PlayerTank(m_PlayerStart, XMFLOAT3{ 0.0f,0.5f,0.0f }, tankDesc);
	AddChild(m_pPlayerTank);
	
	//Collision

	m_pPlayButton->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				//Logic when tank is on play button
				m_ShouldCountDown = true;
				m_CurrentButtonLoadTime = m_ButtonLoadMaxTime;
				m_MenuAction = MenuActions::Play;
			}
			if (triggerAction == PxTriggerAction::LEAVE)
			{
				m_ShouldCountDown = false;
				m_MenuAction = MenuActions::None;
			}
		}
	);


	m_pExitButton->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				//Logic when tank is on Exit button
				m_ShouldCountDown = true;
				m_CurrentButtonLoadTime = m_ButtonLoadMaxTime;
				m_MenuAction = MenuActions::Exit;
			}
			if (triggerAction == PxTriggerAction::LEAVE)
			{
				m_ShouldCountDown = false;
				m_MenuAction = MenuActions::None;
			}
		}
	);


	//HUD
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Arial_32.fnt");
	m_HudTextColor = XMFLOAT4{ Colors::Red };
	m_HudTextPos = XMFLOAT2{ m_SceneContext.windowWidth /2.f,m_SceneContext.windowHeight / 2.0f};

	EnvironmentBuilder::BuildLavaEnv(this,false);

	m_SceneContext.pLights->SetDirectionalLight(XMFLOAT3{ 15.897f,5.0f,10.793f }, XMFLOAT3{ -1.f,-0.8f,0.5f });
	
	//Credits
	auto creditsGameObj = new GameObject();
	AddChild(creditsGameObj);
	m_pCreditsModel = new ModelComponent(L"Meshes/credits.ovm");
	m_pCreditsMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pCreditsMat->SetDiffuseTexture(L"Textures/wall/solidWallAlbedo.tif");
	m_pCreditsModel->SetMaterial(m_pCreditsMat);
	creditsGameObj->AddComponent(m_pCreditsModel);
	auto pCreditsTransfer = creditsGameObj->GetTransform();
	pCreditsTransfer->Rotate(90.0f, 0.0f, 0.0);
	pCreditsTransfer->Scale(0.05f);
	pCreditsTransfer->Translate(center.x-2.f, center.y, center.z + 5.0f);

	m_pDAEFlag = new FlagDAE{};
	AddChild(m_pDAEFlag);
	m_pDAEFlag->GetTransform()->Translate(center.x + 2.0f, center.y, center.z + 7.8f);

	m_pDAEFlag1 = new FlagDAE{};
	AddChild(m_pDAEFlag1);
	m_pDAEFlag1->GetTransform()->Translate(center.x - 5.0f, center.y, center.z + 7.8f);
}


void MainMenu::Update()
{
	if (!m_ShouldCountDown)
	{
		return;
	}

	
	m_CurrentButtonLoadTime -= m_SceneContext.pGameTime->GetElapsed();
	if (m_CurrentButtonLoadTime > 0)
	{
		m_HudText = std::format("{:.0f}", m_CurrentButtonLoadTime);
		TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_HudText), m_HudTextPos, m_HudTextColor);
		return;
	}

	switch (m_MenuAction)
	{
	case MainMenu::MenuActions::Play:
		Logger::LogInfo(L"Opening Play");
		m_pPlayerTank->Reset();
		SceneManager::Get()->SetActiveGameScene(L"LevelSelectScene");
		break;
	case MainMenu::MenuActions::Exit:
		OverlordGame::SHOULD_EXIT = true;
		break;
	}
	m_CurrentButtonLoadTime = m_ButtonLoadMaxTime;
	m_ShouldCountDown = false;
	m_MenuAction = MenuActions::None;
}

void MainMenu::Draw()
{
}

void MainMenu::OnGUI()
{
}

void MainMenu::PostDraw()
{
}

void MainMenu::OnSceneDeactivated()
{
	if (m_pPlayerTank)
	{
		m_pPlayerTank->DisableMovement();
	}
}

void MainMenu::OnSceneActivated()
{
	if (m_pPlayerTank)
	{
		m_pPlayerTank->EnableMovement();
	}
}



void MainMenu::LockCamera()
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
