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


	int nrCols = 10;
	int nrRows = 10;
	float size = 10.0f;

	m_pGrassMat = MaterialManager::Get()->CreateMaterial<GrassMaterial>();
	

	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			const auto pModel = new ModelComponent(L"Meshes/GroundPlane.ovm");
			pModel->SetMaterial(m_pGrassMat);
			auto pGround = new GameObject();
			AddChild(pGround);
			pGround->AddComponent(pModel);
			pGround->GetTransform()->Scale(size);
			pGround->GetTransform()->Translate(col * size, 0.0f, row * size);
		}
	}

	

}

void GrassTestScene::Update()
{
	
}

void GrassTestScene::Draw()
{
}

void GrassTestScene::OnGUI()
{
	m_pGrassMat->DrawImGui();
}
