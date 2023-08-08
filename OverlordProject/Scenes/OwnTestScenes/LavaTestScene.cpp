#include "stdafx.h"
#include "LavaTestScene.h"
#include "Prefabs/CubePrefab.h"
#include "Materials/LavaMaterial.h"

LavaTestScene::LavaTestScene() :
	GameScene(L"LavaTestScene")
{
}

void LavaTestScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;

	m_pLavaMat = MaterialManager::Get()->CreateMaterial<LavaMaterial>();

	m_pModel = new ModelComponent(L"Meshes/Environment.ovm");
	m_pModel->SetMaterial(m_pLavaMat);
	auto pGround = new GameObject();
	AddChild(pGround);
	pGround->AddComponent(m_pModel);

	auto pTransform = m_pModel->GetTransform();

	pTransform->Scale(0.08f);
	pTransform->Rotate(0, -110, 0);
	pTransform->Translate(-8.0f, -2.f, 8.0f);

	
}

void LavaTestScene::Update()
{
	float deltatime = m_SceneContext.pGameTime->GetElapsed() * 0.001f;

	auto transform = m_pModel->GetTransform();
	auto currentSize = transform->GetScale();

	transform->Scale(currentSize.x + deltatime, currentSize.y, currentSize.z + deltatime);
}

void LavaTestScene::Draw()
{
}

void LavaTestScene::OnGUI()
{
	m_pLavaMat->DrawImGui();
}
