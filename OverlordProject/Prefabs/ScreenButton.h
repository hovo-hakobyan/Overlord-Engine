#pragma once
class ScreenButton final : public GameObject
{
public:
	ScreenButton(const XMFLOAT2& loc, const XMFLOAT2& dimensions);
	~ScreenButton() override = default;
	ScreenButton(const ScreenButton& other) = delete;
	ScreenButton(ScreenButton&& other) = delete;
	ScreenButton& operator=(const ScreenButton& other) = delete;
	ScreenButton& operator=(ScreenButton&& other) = delete;

	void Hide();
	void Show();
protected:
	virtual void Initialize(const SceneContext&) override;
private:
	XMFLOAT2 m_Location{};
	XMFLOAT2 m_Dimensions{};

	bool m_IsHidden{};
};

