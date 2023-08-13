#include "stdafx.h"
#include "hud.h"
#include "Prefabs/TankIcon.h"
#include "Prefabs/BaseTank.h"
#include "Misc/LevelExtractor.h"

Hud::Hud()
{
}

void Hud::Show()
{
	for (size_t i = 0; i < m_pIcons.size(); i++)
	{
		m_pIcons[i]->Show();
	}
}

void Hud::Initialize(const SceneContext& sceneContext)
{
	m_pBackground = AddComponent(new SpriteComponent(L"Textures/Menu/hud.png"));
	GetTransform()->Translate(sceneContext.windowWidth - 200, 0, 0.9f);
	//Icons init
	XMFLOAT2 dim{ 0.7f,0.7f };
	float gap{ 15.f };
	float size{ 65.f };
	XMFLOAT2 startPos{ 25.f, 130.f };
	XMFLOAT2 loc{ startPos };

	for (int i = 0; i < 20; i++)
	{

		if (i % 3 == 0)
		{
			if (i != 0)
			{
				loc.y += gap + size * dim.y;
				loc.x = startPos.x;
			}
			
		}
		else
		{
			loc.x += gap + size * dim.x;
		}

		auto tankIcon = new TankIcon(loc, dim);
		m_pIcons[i] = tankIcon;
		AddChild(m_pIcons[i]);

	}

	Show();
	
	//text
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Arial_32.fnt");
	m_TextColor = XMFLOAT4{ 0.51f,0.09f,0.04f,1.0f };
	m_TextPos = XMFLOAT2{sceneContext.windowWidth - 140.f, 25.0f };
	m_Text = LevelExtractor::GetSelectedLevelName();
}

void Hud::Update(const SceneContext&)
{
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), m_TextPos, m_TextColor);

	auto currentEnemies = BaseTank::EnemyCounter;
	if (currentEnemies == m_CurrentIcons)
	{
		return;
	}
	else
	{
		if (m_pIcons.empty())
		{
			return;
		}
		RemoveChild(m_pIcons[m_CurrentIcons - 1], true);
		m_pIcons.pop_back();
		m_CurrentIcons = currentEnemies;
		
	}
}
