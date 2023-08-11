#pragma once
class ScreenButton final : public GameObject
{
public:
	ScreenButton(const XMFLOAT2& loc, const XMFLOAT2& dimensions, const std::string& text);
	~ScreenButton() override = default;
	ScreenButton(const ScreenButton& other) = delete;
	ScreenButton(ScreenButton&& other) = delete;
	ScreenButton& operator=(const ScreenButton& other) = delete;
	ScreenButton& operator=(ScreenButton&& other) = delete;

	void Hide();
	void Show();
	void Select();
	void Deselect();
	bool IsSelected();
protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;
private:
	XMFLOAT2 m_Location{};
	XMFLOAT2 m_Dimensions{};
	SpriteComponent* m_pSpriteComponent{};

	bool m_IsHidden{false};
	bool m_IsSelected{ false };

	SpriteFont* m_pFont{};
	std::string m_Text{};
	XMFLOAT2 m_TextPos{};
	XMFLOAT4 m_TextColor{};
};

