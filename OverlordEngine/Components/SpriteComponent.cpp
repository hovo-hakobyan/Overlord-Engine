#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext&)
{
	if (!m_pTexture)
		return;

	auto pTransformComp = GetGameObject()->GetTransform();

	XMFLOAT3 worldPos{ pTransformComp->GetWorldPosition() };
	XMFLOAT3 worldScale{ pTransformComp->GetWorldScale() };
	XMFLOAT4 worldRot{ pTransformComp->GetWorldRotation() };
	float zRot{ MathHelper::QuaternionToEuler(worldRot).z };

	SpriteRenderer::Get()->AppendSprite(m_pTexture, XMFLOAT2{worldPos.x,worldPos.y}, m_Color, m_Pivot, XMFLOAT2{worldScale.x,worldScale.y},zRot,pTransformComp->GetPosition().z);
	
		
}
