#include "stdafx.h"
#include "WaterTestScene.h"
#include "Prefabs/CubePrefab.h"
#include "Materials/WaterMaterial.h"

WaterTestScene::WaterTestScene():
	GameScene(L"WaterTestScene")
{
}

void WaterTestScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;


	//int nrCols = 10;
	//int nrRows = 10;
	float size = 100.0f;

	m_pWaterMat = MaterialManager::Get()->CreateMaterial<WaterMaterial>();

	//Init paddles

	const auto pModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
	pModel->SetMaterial(m_pWaterMat);
	auto pGround = new GameObject();
	AddChild(pGround);
	pGround->AddComponent(pModel);
	pGround->GetTransform()->Scale(size);

	/*for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			const auto pModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
			pModel->SetMaterial(m_pWaterMat);
			auto pGround = new GameObject();
			AddChild(pGround);
			pGround->AddComponent(pModel);
			pGround->GetTransform()->Scale(size);
			pGround->GetTransform()->Translate(col * size, 0.0f, row * size);
		}
	}*/



}

void WaterTestScene::Update()
{
	m_pWaterMat->UpdateTime(m_SceneContext.pGameTime->GetTotal());
}

void WaterTestScene::Draw()
{
}

void WaterTestScene::OnGUI()
{
	m_pWaterMat->DrawImGui();
}
