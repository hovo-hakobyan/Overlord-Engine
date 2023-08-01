#include "stdafx.h"
#include "EnvironmentBuilder.h"
#include "Materials/LavaMaterial.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/DiffuseMaterial.h"


EnvironmentBuilder::EnvironmentBuilder(BattleCityScene* gameScene):
	m_pGameScene{gameScene}
{
	
	
}


void EnvironmentBuilder::BuildLavaEnv()
{

	//Materials
	m_pLavaMat = MaterialManager::Get()->CreateMaterial<LavaMaterial>();

	auto pIslandGrassMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pIslandGrassMat->SetDiffuseTexture(L"Textures/Environment/Island_Grass.png");

	auto pPyramidMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pPyramidMat->SetDiffuseTexture(L"Textures/Environment/Pyramid.png");
	
	auto pShrubberyMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pShrubberyMat->SetDiffuseTexture(L"Textures/Environment/shrubbery.png");

	auto pLavaBubble = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pLavaBubble->SetDiffuseTexture(L"Textures/Environment/Lava_bubble.png");

	auto pPalmMat1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pPalmMat1->SetDiffuseTexture(L"Textures/Environment/Palm_tree_1.png");

	auto pPalmMat2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pPalmMat2->SetDiffuseTexture(L"Textures/Environment/Palm_tree_2.png");

	auto pSandMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pSandMat->SetDiffuseTexture(L"Textures/Environment/Volacano_Sand.png");

	auto pVolcGrassMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pVolcGrassMat->SetDiffuseTexture(L"Textures/Environment/Volcano_Grass.png");
	
	auto pVolcBaseMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pVolcBaseMat->SetDiffuseTexture(L"Textures/Environment/Volcano_Base.png");

	//Object
	auto pEnv = new GameObject();
	m_pGameScene->AddChild(pEnv);
	const auto pLavaEnvModel = new ModelComponent(L"Meshes/Environment.ovm");
	pEnv->AddComponent(pLavaEnvModel);

	//Setting materials
	pLavaEnvModel->SetMaterial(pPyramidMat, 0);
	pLavaEnvModel->SetMaterial(pIslandGrassMat, 1);
	pLavaEnvModel->SetMaterial(pShrubberyMat, 2);
	pLavaEnvModel->SetMaterial(pLavaBubble, 3);
	pLavaEnvModel->SetMaterial(m_pLavaMat, 4);
	pLavaEnvModel->SetMaterial(pPalmMat2, 5);
	pLavaEnvModel->SetMaterial(pPalmMat1, 6);
	pLavaEnvModel->SetMaterial(pSandMat, 7);
	pLavaEnvModel->SetMaterial(m_pLavaMat, 8);
	pLavaEnvModel->SetMaterial(pVolcGrassMat, 9);
	pLavaEnvModel->SetMaterial(pVolcBaseMat, 10);

	auto pTransform =pEnv->GetTransform();

	pTransform->Scale(0.08f);
	pTransform->Rotate(0, -110, 0);
	pTransform->Translate(-8.0f, -2.f, 8.0f);

}

void EnvironmentBuilder::test()
{
	m_pLavaMat->DrawImGui();
}
