#pragma once
class WorldButton;
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

private:	
	enum class MenuActions
	{
		Play,WorldBuilder,Exit, None
	};

	WorldButton* m_pPlayButton{};
	WorldButton* m_pWorldBuilderButton{};
	WorldButton* m_pExitButton{};

	class LevelBuilder* m_pLevelBuilder{};
	class PlayerTank* m_pPlayerTank{};

	void LockCamera();

	MenuActions m_MenuAction{ MenuActions::None };
	float m_ButtonLoadMaxTime{ 4.0f };
	float m_CurrentButtonLoadTime{};
	bool m_ShouldCountDown{ false };
};



