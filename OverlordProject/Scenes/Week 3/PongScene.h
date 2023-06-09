#pragma once
class PongScene final : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	enum InputIds
	{
		FirstUp,
		FirstDown,
		SecondUp,
		SecondDown
	};

	float m_PaddleSpeed{ 15.f };
	float m_BallSpeed{ 20.f };
	float m_MaxHeight = 13.6f;
	float m_MaxWidth = 24.f;
	const XMFLOAT3 m_PaddleSize{ 0.3f,6.f,2.5f };

	bool m_ShouldReset{ true };

	GameObject* m_pLeftPaddle{};
	GameObject* m_pRightPaddle{};
	GameObject* m_pBall{};

	RigidBodyComponent* m_pLeftRigidBody{};
	RigidBodyComponent* m_pRightRigidBody{};
	RigidBodyComponent* m_pBallRigidBody{};

	void Reset();
};


