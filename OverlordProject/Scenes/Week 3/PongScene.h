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

	float m_PaddleSpeed{ 10.f };

	GameObject* m_pLeftPaddle{};
	GameObject* m_pRightPaddle{};

	RigidBodyComponent* m_pLeftRigidBody{};
	RigidBodyComponent* m_pRightRigidBody{};
};


