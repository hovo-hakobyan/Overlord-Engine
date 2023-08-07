#include "stdafx.h"
#include "MainMenu.h"
#include "Prefabs/WorldButton.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/Hatch.h"
#include "Prefabs/PlayerTank.h"
#include "Prefabs/CubePrefab.h"

MainMenu::MainMenu():
	GameScene(L"Main Menu")
{
}

MainMenu::~MainMenu()
{
	SafeDelete(m_pLevelBuilder);
}

void MainMenu::Initialize()
{

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this);

	LockCamera();

	m_pPlayButton = new WorldButton(L"Textures/Menu/play.png");
	AddChild(m_pPlayButton);


	m_pWorldBuilderButton = new WorldButton(L"Textures/Menu/LevelBuilder.png");
	AddChild(m_pWorldBuilderButton);

	m_pExitButton = new WorldButton(L"Textures/Menu/exit.png");
	AddChild(m_pExitButton);

	//Level
	m_pLevelBuilder = new LevelBuilder{ this,1.2f };
	m_pLevelBuilder->AddMainMenuLevel("Resources/Levels/MainMenu.bmp", 15, 15);
	m_pLevelBuilder->BuildMainMenu();

	auto center = m_pLevelBuilder->GetLevelCenter();
	center.y -= 0.1f;
	center.x -= 4.5f;

	auto transform = m_pPlayButton->GetTransform();
	transform->Translate(center);
	transform->Rotate(0, 90, 0);

	center.x += 4.5f;
	transform = m_pWorldBuilderButton->GetTransform();
	transform->Translate(center);
	transform->Rotate(0, 90, 0);

	center.x += 4.5f;
	transform = m_pExitButton->GetTransform();
	transform->Translate(center);
	transform->Rotate(0, 90, 0);

	//Tank
	TankDesc tankDesc{ pDefaultMaterial };
	tankDesc.isCombat = false;
	auto playerLoc = m_pLevelBuilder->GetPlayerStartHatch()->GetTransform()->GetPosition();
	m_pPlayerTank = new PlayerTank(playerLoc, XMFLOAT3{ 0.0f,0.5f,0.0f }, tankDesc);
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

	m_pWorldBuilderButton->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				//Logic when tank is on WorldBuilder button
				m_ShouldCountDown = true;
				m_CurrentButtonLoadTime = m_ButtonLoadMaxTime;
				m_MenuAction = MenuActions::WorldBuilder;
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
		return;
	}

	switch (m_MenuAction)
	{
	case MainMenu::MenuActions::Play:
		Logger::LogInfo(L"Opening Play");
		break;
	case MainMenu::MenuActions::WorldBuilder:
		Logger::LogInfo(L"Opening Builder");
		break;
	case MainMenu::MenuActions::Exit:
		
		break;
	}
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
