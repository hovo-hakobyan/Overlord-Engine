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
	std::vector<TankIcon*> m_pIcons{ 20 };
};

