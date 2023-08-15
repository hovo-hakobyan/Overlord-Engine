#include "stdafx.h"
#include "VignetteTestScene.h"
#include "Materials/Post/PostVignette.h"

VignetteTestScene::VignetteTestScene():
	GameScene{L"VignetteTest"}
{
}

void VignetteTestScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	m_pPostVignette = MaterialManager::Get()->CreateMaterial<PostVignette>();

	AddPostProcessingEffect(m_pPostVignette);
}

void VignetteTestScene::Update()
{
}

void VignetteTestScene::Draw()
{
}

void VignetteTestScene::OnGUI()
{
	
}
