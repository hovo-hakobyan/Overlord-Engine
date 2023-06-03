#pragma once
#include "Prefabs/BaseTank.h"
class EnemyTank : public BaseTank
{
public:
	EnemyTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot, const TankDesc& tankDesc, GameScene* gameScene);
	~EnemyTank() override = default;

	EnemyTank(const EnemyTank& other) = delete;
	EnemyTank(EnemyTank&& other) = delete;
	EnemyTank& operator=(const EnemyTank& other) = delete;
	EnemyTank& operator=(EnemyTank&& other) = delete;
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext&) override;
private:
	enum class Direction
	{
		Left,Right,Up,Down
	};

	Direction m_Direction{ Direction::Down };

	float m_VelocityMagnitudeSqr;
	float m_TimeSinceZeroVelocity;

	float m_TimeThreshold{3.0f};
	float m_VelocityThreshold{ 0.1f };

	PxRigidBody* m_pRigidBody{};

	void Move(const XMFLOAT2& dir, float deltaTime);
};

