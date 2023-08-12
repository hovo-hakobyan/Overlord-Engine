#pragma once

class EndScreen final : public GameObject
{
public:
	EndScreen();
	~EndScreen() override = default;
	EndScreen(const EndScreen& other) = delete;
	EndScreen(EndScreen&& other) noexcept = delete;
	EndScreen& operator=(const EndScreen& other) = delete;
	EndScreen& operator=(EndScreen&& other) noexcept = delete;

	void ShowVictory();
	void ShowDefeat();
protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;
private:

	SpriteComponent* m_pSpriteComp{};

	XMFLOAT2 m_Location{};

	void ShowEndScreen();

	bool m_ShouldCountDown{ false };

	float m_MaxTime{4.0f};
	float m_CurrentTime{};
};

