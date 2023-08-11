#pragma once
class WorldButton;
class LevelSelectScene : public GameScene
{
public:
	LevelSelectScene();
	~LevelSelectScene() override;

	LevelSelectScene(const LevelSelectScene& other) = delete;
	LevelSelectScene(LevelSelectScene&& other) noexcept = delete;
	LevelSelectScene& operator=(const LevelSelectScene& other) = delete;
	LevelSelectScene& operator=(LevelSelectScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;

private:
	class PlayerTank* m_pPlayerTank{};

	void LockCamera();

	float m_ButtonLoadMaxTime{ 4.0f };
	float m_CurrentButtonLoadTime{};
	bool m_ShouldCountDown{ false };
	int m_SelectedLevelIdx{-1};
	bool m_ShouldReturn{false};

	//HUD
	SpriteFont* m_pFont{};
	std::string m_HudText{};
	XMFLOAT2 m_HudTextPos{};
	XMFLOAT4 m_HudTextColor{};

	WorldButton* m_pReturnButton{};
};

