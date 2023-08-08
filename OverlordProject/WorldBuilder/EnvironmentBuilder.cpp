#include "stdafx.h"
#include "EnvironmentBuilder.h"
#include "Materials/LavaMaterial.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/SimpleDiffuse.h"

bool EnvironmentBuilder::m_IsInitialized = false;
LavaMaterial* EnvironmentBuilder::m_pLavaMat = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pIslandGrassMat = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pPyramidMat = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pShrubberyMat = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pLavaBubble = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pPalmMat1 = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pPalmMat2 = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pVolcBaseMat = nullptr;
DiffuseMaterial_Shadow* EnvironmentBuilder::m_pVolcGrassMat = nullptr;
DiffuseMaterial* EnvironmentBuilder::m_pSandMat = nullptr;
SimpleDiffuseMaterial* EnvironmentBuilder::m_pCooledLavaMat = nullptr;

EnvironmentBuilder::EnvironmentBuilder()
{
}


void EnvironmentBuilder::Initialize()
{
	if (m_IsInitialized)
	{
		return;
	}

	//Materials
	m_pLavaMat = MaterialManager::Get()->CreateMaterial<LavaMaterial>();

	m_pIslandGrassMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pIslandGrassMat->SetDiffuseTexture(L"Textures/Environment/Island_Grass.png");

	m_pPyramidMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pPyramidMat->SetDiffuseTexture(L"Textures/Environment/Pyramid.png");

	m_pShrubberyMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pShrubberyMat->SetDiffuseTexture(L"Textures/Environment/shrubbery.png");

	m_pLavaBubble = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pLavaBubble->SetDiffuseTexture(L"Textures/Environment/Lava_bubble.png");

	m_pPalmMat1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pPalmMat1->SetDiffuseTexture(L"Textures/Environment/Palm_tree_1.png");

	m_pPalmMat2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pPalmMat2->SetDiffuseTexture(L"Textures/Environment/Palm_tree_2.png");

	m_pSandMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pSandMat->SetDiffuseTexture(L"Textures/Environment/Volacano_Sand.png");

	m_pVolcGrassMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pVolcGrassMat->SetDiffuseTexture(L"Textures/Environment/Volcano_Grass.png");

	m_pVolcBaseMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pVolcBaseMat->SetDiffuseTexture(L"Textures/Environment/Volcano_Base.png");

	m_pCooledLavaMat = MaterialManager::Get()->CreateMaterial<SimpleDiffuseMaterial>();
	m_pCooledLavaMat->SetNormalMapTexture(L"Textures/Environment/CooledLavaNormal.tif");
	m_pCooledLavaMat->SetDiffuseTexture(L"Textures/Environment/CooledLavaAlbedo.tif");
	m_IsInitialized = true;
}

void EnvironmentBuilder::BuildLavaEnv(GameScene* pScene)
{
	if (!m_IsInitialized)
	{
		Initialize();
	}

	//Object
	auto pEnv = new GameObject();
	pScene->AddChild(pEnv);
	const auto pLavaEnvModel = new ModelComponent(L"Meshes/Environment.ovm");
	pEnv->AddComponent(pLavaEnvModel);

	//Setting materials
	pLavaEnvModel->SetMaterial(m_pPyramidMat, 0);
	pLavaEnvModel->SetMaterial(m_pIslandGrassMat, 1);
	pLavaEnvModel->SetMaterial(m_pShrubberyMat, 2);
	pLavaEnvModel->SetMaterial(m_pLavaBubble, 3);
	pLavaEnvModel->SetMaterial(m_pLavaMat, 4);
	pLavaEnvModel->SetMaterial(m_pPalmMat2, 5);
	pLavaEnvModel->SetMaterial(m_pPalmMat1, 6);
	pLavaEnvModel->SetMaterial(m_pSandMat, 7);
	pLavaEnvModel->SetMaterial(m_pLavaMat, 8);
	pLavaEnvModel->SetMaterial(m_pVolcGrassMat, 9);
	pLavaEnvModel->SetMaterial(m_pVolcBaseMat, 10);

	auto pTransform =pEnv->GetTransform();

	pTransform->Scale(0.08f);
	pTransform->Rotate(0, -110, 0);
	pTransform->Translate(-8.0f, -2.f, 8.0f);
}

void EnvironmentBuilder::BuildMenuEnv(GameScene* pScene)
{
	if (!m_IsInitialized)
	{
		Initialize();
	}

	auto pEnv = new GameObject();
	pScene->AddChild(pEnv);
	const auto pEnvModel = new ModelComponent(L"Meshes/Environment.ovm");
	pEnv->AddComponent(pEnvModel);


	//Setting materials
	pEnvModel->SetMaterial(m_pPyramidMat, 0);
	pEnvModel->SetMaterial(m_pIslandGrassMat, 1);
	pEnvModel->SetMaterial(m_pShrubberyMat, 2);
	pEnvModel->SetMaterial(m_pLavaBubble, 3);
	pEnvModel->SetMaterial(m_pCooledLavaMat, 4);
	pEnvModel->SetMaterial(m_pPalmMat2, 5);
	pEnvModel->SetMaterial(m_pPalmMat1, 6);
	pEnvModel->SetMaterial(m_pSandMat, 7);
	pEnvModel->SetMaterial(m_pCooledLavaMat, 8);
	pEnvModel->SetMaterial(m_pVolcGrassMat, 9);
	pEnvModel->SetMaterial(m_pVolcBaseMat, 10);

	auto pTransform = pEnv->GetTransform();

	pTransform->Scale(0.08f);
	pTransform->Rotate(0, -110, 0);
	pTransform->Translate(-8.0f, -2.f, 8.0f);

}

