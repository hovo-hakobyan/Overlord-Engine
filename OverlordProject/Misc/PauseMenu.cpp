#include "stdafx.h"
#include "PauseMenu.h"
#include "Prefabs/ScreenButton.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"
#include "LevelExtractor.h"

PauseMenu::PauseMenu()
{
}

void PauseMenu::Hide()
{
	m_IsHidden = true;
	m_pExit->Hide();
	m_pMainMenu->Hide();
	m_pRestart->Hide();
	m_pExit->Deselect();
	m_pRestart->Deselect();
	m_pMainMenu->Deselect();
	
}

void PauseMenu::Show()
{
	if (m_IsHidden)
	{
		m_pExit->Show();
		m_pMainMenu->Show();
		m_pRestart->Show();
		m_IsHidden = false;

		m_pMainMenu->Select();
		SoundManager::Get()->GetSystem()->playSound(m_pMenuOpenSound, nullptr, false, &m_pChannelOpen);
	}
	
}

void PauseMenu::Initialize(const SceneContext& sceneContext)
{
	XMFLOAT2 dim{ 0.1f,0.1f };
	const float gap{ 80.0f };
	XMFLOAT2 loc{ sceneContext.windowWidth / 2 - dim.x / 2.f,sceneContext.windowHeight / 2.f - 100.f };

	m_pMainMenu = new ScreenButton(loc, dim, "Main Menu");
	AddChild(m_pMainMenu);

	loc.y += gap;
	m_pRestart = new ScreenButton(loc, dim,"Restart");
	AddChild(m_pRestart);

	loc.y += gap;
	m_pExit = new ScreenButton(loc, dim,"Exit");
	AddChild(m_pExit);


	Hide();

	//Input
	auto input = sceneContext.pInput;

	input->AddInputAction(InputAction{ InputIds::Down,InputState::pressed,VK_DOWN });
	input->AddInputAction(InputAction{ InputIds::Select,InputState::pressed,VK_RETURN });
	input->AddInputAction(InputAction{ InputIds::Up,InputState::pressed,VK_UP });

	// Sound
	const auto pSoundManager = SoundManager::Get()->GetSystem();
	pSoundManager->createStream("Resources/Sounds/ButtonHover.wav", FMOD_DEFAULT, nullptr, &m_pButtonSound);
	pSoundManager->createStream("Resources/Sounds/MenuOpen.wav", FMOD_DEFAULT, nullptr, &m_pMenuOpenSound);
	m_pChannelHover->setVolume(1.f);
	m_pChannelOpen->setVolume(1.f);
}

void PauseMenu::Update(const SceneContext& sceneContext)
{
	if (m_IsHidden)
	{
		return;
	}

	UpdateButtonNavigation(sceneContext);

	
}

void PauseMenu::UpdateButtonNavigation(const SceneContext& sceneContext)
{
	const auto input = sceneContext.pInput;
	const auto soundSystem = SoundManager::Get()->GetSystem();
	if (input->IsActionTriggered(InputIds::Down))
	{
		SelectNextButton();
		soundSystem->playSound(m_pButtonSound, nullptr, false, &m_pChannelHover);
	}

	if (input->IsActionTriggered(InputIds::Up))
	{
		SelectPreviousButton();
		soundSystem->playSound(m_pButtonSound, nullptr, false, &m_pChannelHover);
	}

	if (input->IsActionTriggered(InputIds::Select))
	{
		TriggerButton();
	}

}

void PauseMenu::SelectPreviousButton()
{
	if (m_pRestart->IsSelected())
	{
		m_pRestart->Deselect();
		m_pMainMenu->Select();
		return;
	}

	if (m_pExit->IsSelected())
	{
		m_pExit->Deselect();
		m_pRestart->Select();
		return;
	}
}

void PauseMenu::SelectNextButton()
{
	if (m_pMainMenu->IsSelected())
	{
		m_pMainMenu->Deselect();
		m_pRestart->Select();
		return;
	}

	if (m_pRestart->IsSelected())
	{
		m_pRestart->Deselect();
		m_pExit->Select();
		return;
	}

}

void PauseMenu::TriggerButton()
{
	if (m_pMainMenu->IsSelected())
	{
		SceneManager::Get()->SetActiveGameScene(L"MainMenu");
		return;
	}
	
	if (m_pRestart->IsSelected())
	{
		
		SceneManager::Get()->AddGameScene(new BattleCityScene(LevelExtractor::GetLevelPath(LevelExtractor::GetSelectedLevelIdx())));
		SceneManager::Get()->NextScene();
		return;
	}

	if (m_pExit->IsSelected())
	{
		OverlordGame::SHOULD_EXIT = true;
		return;
	}
}
