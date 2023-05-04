#include "stdafx.h"
#include "GrassTestScene.h"
#include "Materials/GrassMaterial.h"
#include "Prefabs/CubePrefab.h"

GrassTestScene::GrassTestScene():
	GameScene(L"GrassTestScene")
{
}

void GrassTestScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;


	XMFLOAT3 size = { 2.0f,0.1f,2.0f };

	m_pGround = new GameObject();
	AddChild(m_pGround);

	m_pGrassMat = MaterialManager::Get()->CreateMaterial<GrassMaterial>();
	const auto pModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
	pModel->SetMaterial(m_pGrassMat);

	m_pGround->AddComponent(pModel);
	m_pGround->GetTransform()->Scale(15.0f);
}

void GrassTestScene::Update()
{
	float degPerSec = 20.f;
	float deltaTime = m_SceneContext.pGameTime->GetElapsed();

	m_pGround->GetTransform()->Rotate(0.f, degPerSec * deltaTime, 0.f);
}

void GrassTestScene::Draw()
{
}

void GrassTestScene::OnGUI()
{
	m_pGrassMat->DrawImGui();
}
