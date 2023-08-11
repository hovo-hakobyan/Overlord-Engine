#include "stdafx.h"
#include "ScreenButton.h"

ScreenButton::ScreenButton(const XMFLOAT2& loc, const XMFLOAT2& dimensions, const std::string& text):
	m_Location{loc},
	m_Dimensions{dimensions},
	m_Text{text}
{
}

void ScreenButton::Hide()
{
	GetTransform()->Translate(m_Location.x, m_Location.y, 1.9f);
	m_IsHidden = true;
}

void ScreenButton::Show()
{
	GetTransform()->Translate(m_Location.x,m_Location.y,.9f);
	m_IsHidden = false;
}

void ScreenButton::Select()
{
	m_IsSelected = true;
	m_pSpriteComponent->SetTexture(L"Textures/Menu/buttonSelected.png");
}

void ScreenButton::Deselect()
{
	m_IsSelected = false;
	m_pSpriteComponent->SetTexture(L"Textures/Menu/buttonUnselected.png");
}

bool ScreenButton::IsSelected()
{
	return m_IsSelected;
}


void ScreenButton::Initialize(const SceneContext&)
{
	m_pSpriteComponent = AddComponent(new SpriteComponent(L"Textures/Menu/buttonUnselected.png"));
	auto pTransform = GetTransform();
	pTransform->Translate(m_Location.x, m_Location.y, .9f);
	pTransform->Scale(m_Dimensions.x, m_Dimensions.y, 1.0f);

	//text
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Arial_32.fnt");
	m_TextColor = XMFLOAT4{ Colors::Red };
	m_TextPos = XMFLOAT2{m_Location.x + 15.f,m_Location.y + 10.0f};
}

void ScreenButton::Update(const SceneContext&)
{
	if (!m_IsHidden)
	{
		TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), m_TextPos, m_TextColor);
	}
	

}

