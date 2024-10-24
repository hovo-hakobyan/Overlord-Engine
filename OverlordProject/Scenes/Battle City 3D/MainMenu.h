#pragma once
class WorldButton;
class DiffuseMaterial;
class FlagDAE;
class MainMenu final : public GameScene
{
public:
	MainMenu();
	~MainMenu() override;

	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) noexcept = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void PostDraw() override;
	void OnSceneDeactivated() override;
	void OnSceneActivated() override;

private:	
	enum class MenuActions
	{
		Play,Exit, None
	};

	WorldButton* m_pPlayButton{};
	WorldButton* m_pExitButton{};

	class LevelBuilder* m_pLevelBuilder{};
	class PlayerTank* m_pPlayerTank{};

	void LockCamera();

	MenuActions m_MenuAction{ MenuActions::None };
	float m_ButtonLoadMaxTime{ 3.0f };
	float m_CurrentButtonLoadTime{};
	bool m_ShouldCountDown{ false };

	XMFLOAT3 m_PlayerStart{};

	//HUD
	SpriteFont* m_pFont{};
	std::string m_HudText{};
	XMFLOAT2 m_HudTextPos{};
	XMFLOAT4 m_HudTextColor{};

	ModelComponent* m_pCreditsModel;
	DiffuseMaterial* m_pCreditsMat;

	FlagDAE* m_pDAEFlag{};
	FlagDAE* m_pDAEFlag1{};
};



