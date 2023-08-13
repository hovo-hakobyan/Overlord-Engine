#pragma once
class TankIcon;

class Hud final : public GameObject
{
public:
	Hud();
	~Hud() override = default;
	Hud(const Hud& other) = delete;
	Hud(Hud&& other) noexcept = delete;
	Hud& operator=(const Hud& other) = delete;
	Hud& operator=(Hud&& other) noexcept = delete;

	void Show();

protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;
private:
	SpriteComponent* m_pBackground{};
	std::deque<TankIcon*> m_pIcons{ 20 };

	int m_CurrentIcons{20};

	SpriteFont* m_pFont{};
	std::string m_Text{};
	XMFLOAT2 m_TextPos{};
	XMFLOAT4 m_TextColor{};
};

