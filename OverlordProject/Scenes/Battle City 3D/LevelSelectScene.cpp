#include "stdafx.h"
#include "LevelSelectScene.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/PlayerTank.h"
#include "Prefabs/WorldButton.h"

LevelSelectScene::LevelSelectScene():
	GameScene{L"LevelSelectScene"}
{
}

LevelSelectScene::~LevelSelectScene()
{
}

void LevelSelectScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;


	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this);
	m_SceneContext.pLights->SetDirectionalLight(XMFLOAT3{ 6.897f,82.759f,13.793f }, XMFLOAT3{ -0.241f,-1.0f,-0.069f });


	//LockCamera();

	//Tank
	TankDesc tankDesc{ pDefaultMaterial };
	tankDesc.isCombat = false;
	XMFLOAT3 playerStart{ 20.0f,0.0f,6.0f };
	m_pPlayerTank = new PlayerTank(playerStart, XMFLOAT3{ 0.0f,0.5f,0.0f }, tankDesc);
	AddChild(m_pPlayerTank);

	//Level
	LevelBuilder::BuildSandbox(this, 10, 15, 3);
	
}

void LevelSelectScene::Update()
{
}
void LevelSelectScene::LockCamera()
{
	//Set a new fixed camera
	auto prevCamera = m_SceneContext.pCamera;
	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(20.f, 26.f, 6.f);
	pFixedCamera->GetTransform()->Rotate(80.0f, 0.0f, 0.0f);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	//remove the old camera
	RemoveChild(prevCamera->GetGameObject(), true);
}
