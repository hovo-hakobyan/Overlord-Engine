#include "stdafx.h"
#include "PauseMenu.h"
#include "Prefabs/ScreenButton.h"

PauseMenu::PauseMenu()
{
}

void PauseMenu::Hide()
{
	m_IsHidden = true;
	m_pExit->Hide();
	m_pMainMenu->Hide();
	m_pRestart->Hide();
}

void PauseMenu::Show()
{
	if (m_IsHidden)
	{
		m_pExit->Show();
		m_pMainMenu->Show();
		m_pRestart->Show();
		m_IsHidden = false;
	}
	
}

void PauseMenu::Initialize(const SceneContext& sceneContext)
{
	XMFLOAT2 dim{ 0.1f,0.1f };
	const float gap{ 80.0f };
	XMFLOAT2 loc{ sceneContext.windowWidth / 2 - dim.x / 2.f,sceneContext.windowHeight / 2.f - 100.f };
	m_pRestart = new ScreenButton(loc, dim);
	AddChild(m_pRestart);

	loc.y += gap;
	m_pExit = new ScreenButton(loc, dim);
	AddChild(m_pExit);

	loc.y += gap;
	m_pMainMenu = new ScreenButton(loc, dim);
	AddChild(m_pMainMenu);

	Hide();
}

void PauseMenu::Update(const SceneContext&)
{
}
