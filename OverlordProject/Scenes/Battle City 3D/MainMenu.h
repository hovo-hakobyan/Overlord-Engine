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
	WorldButton* m_pPlayButton{};
	WorldButton* m_pWorldBuilderButton{};
	WorldButton* m_pExitButton{};
};



