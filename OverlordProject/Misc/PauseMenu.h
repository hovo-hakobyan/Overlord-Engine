#pragma once
class ScreenButton;
class PauseMenu final : public GameObject
{
public:
	PauseMenu();
	~PauseMenu() override = default;
	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu(PauseMenu&& other) noexcept = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu& operator=(PauseMenu&& other) noexcept = delete;

	void Show();
	void Hide();
protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Update(const SceneContext&) override ;
private:
	enum InputIds
	{
		Up = 6,
		Down = 7,
		Select = 8
	};

	ScreenButton* m_pRestart{};
	ScreenButton* m_pMainMenu{};
	ScreenButton* m_pExit{};

	bool m_IsHidden{};

	void UpdateButtonNavigation(const SceneContext& sceneContext);
	void SelectPreviousButton();
	void SelectNextButton();
	void TriggerButton();
};