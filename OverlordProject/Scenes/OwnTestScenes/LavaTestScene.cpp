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


	float size = 100.0f;

	m_pLavaMat = MaterialManager::Get()->CreateMaterial<LavaMaterial>();

	const auto pModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
	pModel->SetMaterial(m_pLavaMat);
	auto pGround = new GameObject();
	AddChild(pGround);
	pGround->AddComponent(pModel);
	pGround->GetTransform()->Scale(size);

}

void LavaTestScene::Update()
{

}

void LavaTestScene::Draw()
{
}

void LavaTestScene::OnGUI()
{
	m_pLavaMat->DrawImGui();
}
