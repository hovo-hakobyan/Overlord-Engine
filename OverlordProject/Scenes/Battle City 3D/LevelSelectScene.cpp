#include "stdafx.h"
#include "LevelSelectScene.h"
#include "WorldBuilder/LevelBuilder.h"
#include "Prefabs/PlayerTank.h"
#include "Prefabs/WorldButton.h"
#include "Misc/LevelExtractor.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"


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


	LockCamera();

	//Tank
	TankDesc tankDesc{ pDefaultMaterial };
	tankDesc.isCombat = false;
	XMFLOAT3 playerStart{ 20.0f,0.0f,6.0f };
	m_pPlayerTank = new PlayerTank(playerStart, XMFLOAT3{ 0.0f,0.5f,0.0f }, tankDesc);
	AddChild(m_pPlayerTank);

	//Level
	LevelBuilder::BuildSandbox(this, 10, 15, 3);
	
	LevelExtractor::ReadAndUpdateLevelsFromFile();
	const auto& paths = LevelExtractor::GetAllLevelsPath();

	//Buttons

	m_pReturnButton = new WorldButton("Textures/Menu/return.png");
	AddChild(m_pReturnButton);

	m_pReturnButton->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				//Logic when tank is on play button
				m_ShouldCountDown = true;
				m_CurrentButtonLoadTime = m_ButtonLoadMaxTime;
				m_ShouldReturn = true;
			}
			if (triggerAction == PxTriggerAction::LEAVE)
			{
				m_ShouldCountDown = false;
				m_ShouldReturn = false;
			}
		}
	);

	auto returnTransform = m_pReturnButton->GetTransform();
	returnTransform->Translate(XMFLOAT3{ 35.f,0.f,3.f });

	XMFLOAT3 startPos{ 5.f,0.f,18.f };
	const float gap{ 5.0f };
	for (size_t i = 0; i < paths.size(); i++)
	{
		if (i % 7 == 0 && i != 0)
		{
			startPos.z -= gap;
			startPos.x = 5.0f;
		}

		auto button = new WorldButton(paths[i]);
		AddChild(button);

		auto pTransform = button->GetTransform();
		pTransform->Translate(startPos);
		pTransform->Scale(2.0f, 1.0f, 2.0f);

		startPos.x += gap;

		button->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction triggerAction)
			{
				if (triggerAction == PxTriggerAction::ENTER)
				{
					//Logic when tank is on play button
					m_ShouldCountDown = true;
					m_CurrentButtonLoadTime = m_ButtonLoadMaxTime;
					m_SelectedLevelIdx = static_cast<int>(i);
					LevelExtractor::SetSelectedLevelIdx(m_SelectedLevelIdx);
				}
				if (triggerAction == PxTriggerAction::LEAVE)
				{
					m_SelectedLevelIdx = -1;
					m_ShouldCountDown = false;
				}
			}
		);

		
	}

	//HUD
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Arial_32.fnt");
	m_HudTextColor = XMFLOAT4{ Colors::Red };
	m_HudTextPos = XMFLOAT2{ m_SceneContext.windowWidth / 2.f,m_SceneContext.windowHeight / 2.0f };
}

void LevelSelectScene::Update()
{
	if (!m_ShouldCountDown)
	{
		return;
	}

	m_CurrentButtonLoadTime -= m_SceneContext.pGameTime->GetElapsed();
	if (m_CurrentButtonLoadTime > 0)
	{
		m_HudText = std::format("{:.0f}", m_CurrentButtonLoadTime);
		TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_HudText), m_HudTextPos, m_HudTextColor);
		return;
	}

	m_ShouldCountDown = false;
	m_pPlayerTank->Reset();
	if (m_ShouldReturn)
	{
		SceneManager::Get()->SetActiveGameScene(L"MainMenu");
	}
	else
	{
		SceneManager::Get()->AddGameScene(new BattleCityScene(LevelExtractor::GetLevelPath(m_SelectedLevelIdx)));
		SceneManager::Get()->SetActiveGameScene(L"BattleCity");
	}
	
	

}
void LevelSelectScene::OnSceneActivated()
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
