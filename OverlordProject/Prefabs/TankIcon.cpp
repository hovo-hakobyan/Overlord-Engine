#include "stdafx.h"
#include "TankIcon.h"

TankIcon::TankIcon(const XMFLOAT2& loc, const XMFLOAT2& dimensions):
	m_Location{loc},
	m_Dimensions{dimensions}
{
}

void TankIcon::Show()
{
	GetTransform()->Translate(m_Location.x, m_Location.y, 0.9f);
}

void TankIcon::Initialize(const SceneContext&)
{
	m_pSpriteComponent = AddComponent(new SpriteComponent(L"Textures/Menu/tankicon.png"));
	auto pTransform = GetTransform();
	pTransform->Scale(m_Dimensions.x, m_Dimensions.y, 1.f);
}
