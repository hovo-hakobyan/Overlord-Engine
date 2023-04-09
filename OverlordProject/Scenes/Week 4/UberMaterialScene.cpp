#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

UberMaterialScene::UberMaterialScene():
	GameScene(L"UberMaterialScene")
{
}

void UberMaterialScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;

	m_pSphere = new GameObject();
	AddChild(m_pSphere);

	m_pUberMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	
	const auto pModel = new ModelComponent(L"Meshes/Sphere.ovm");
	pModel->SetMaterial(m_pUberMat);

	m_pSphere->AddComponent(pModel);
	m_pSphere->GetTransform()->Translate(0.f, 10.f, 0.f);

}

void UberMaterialScene::Update()
{
}

void UberMaterialScene::Draw()
{
}

void UberMaterialScene::OnGUI()
{
	m_pUberMat->DrawImGui();
}
