#include "stdafx.h"
#include "MainMenu.h"
#include "Prefabs/WorldButton.h"

MainMenu::MainMenu():
	GameScene(L"Main Menu")
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Initialize()
{
	m_pPlayButton = new WorldButton(L"Textures/Menu/play.png");
	AddChild(m_pPlayButton);
}

void MainMenu::Update()
{
}

void MainMenu::Draw()
{
}

void MainMenu::OnGUI()
{
}

void MainMenu::PostDraw()
{
}
