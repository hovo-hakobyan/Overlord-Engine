#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

SpikyScene::SpikyScene():
	GameScene(L"Spiky Scene")
{
}

void SpikyScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	

	m_pOctaSphere = new GameObject();
	AddChild(m_pOctaSphere);

	 m_pSpikyMat = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();

	const auto pModel = new ModelComponent(L"Meshes/OctaSphere.ovm");
	pModel->SetMaterial(m_pSpikyMat);

	m_pOctaSphere->AddComponent(pModel);
	m_pOctaSphere->GetTransform()->Scale(15.f);
}

void SpikyScene::Update()
{

	float degPerSec = 20.f;
	float deltaTime = m_SceneContext.pGameTime->GetElapsed();

	m_pOctaSphere->GetTransform()->Rotate(0.f, degPerSec * deltaTime, 0.f);
}

void SpikyScene::Draw()
{
}

void SpikyScene::OnGUI()
{
	m_pSpikyMat->DrawImGui();
}
