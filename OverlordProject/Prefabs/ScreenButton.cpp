#include "stdafx.h"
#include "ScreenButton.h"

ScreenButton::ScreenButton(const XMFLOAT2& loc, const XMFLOAT2& dimensions):
	m_Location{loc},
	m_Dimensions{dimensions}
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

void ScreenButton::Initialize(const SceneContext&)
{
	AddComponent(new SpriteComponent(L"Textures/Menu/buttonUnselected.png"));
	auto pTransform = GetTransform();
	pTransform->Translate(m_Location.x, m_Location.y, .9f);
	pTransform->Scale(m_Dimensions.x, m_Dimensions.y, 1.0f);
}

