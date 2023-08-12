#include "stdafx.h"
#include "EndScreen.h"
#include "Prefabs/ScreenButton.h"

EndScreen::EndScreen()
{
}

void EndScreen::Initialize(const SceneContext& sceneContext)
{
	XMFLOAT2 offset{ 120.f,80.f };
	m_Location = { sceneContext.windowWidth / 2 - offset.x / 2.f,sceneContext.windowHeight / 2.f - offset.y};
	auto pTransform = GetTransform();
	pTransform->Translate(m_Location.x, m_Location.y, 0.9f);
	pTransform->Scale(0.2f, 0.2f, 1.0f);
}

void EndScreen::ShowVictory()
{
	m_pSpriteComp = AddComponent(new SpriteComponent(L"Textures/Menu/victory.png"));	
	ShowEndScreen();
}

void EndScreen::ShowDefeat()
{
	m_pSpriteComp = AddComponent(new SpriteComponent(L"Textures/Menu/defeat.png"));
	ShowEndScreen();
}

void EndScreen::Update(const SceneContext& sceneContext)
{
	if (!m_ShouldCountDown)
	{
		return;
	}

	if (m_CurrentTime >= m_MaxTime)
	{
		SceneManager::Get()->SetActiveGameScene(L"MainMenu");
		m_ShouldCountDown = false;
		return;
	}

	float deltaTime = sceneContext.pGameTime->GetElapsed();
	m_CurrentTime += deltaTime;

}

void EndScreen::ShowEndScreen()
{
	
	m_ShouldCountDown = true;
}
