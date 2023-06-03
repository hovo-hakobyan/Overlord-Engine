#pragma once
#include "Prefabs/BaseTank.h"
class EnemyTank : public BaseTank
{
public:
	EnemyTank(Hatch* pSpawnHatch, const XMFLOAT3& startRot, const TankDesc& tankDesc, GameScene* gameScene);
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

	float m_TimeThreshold{2.0f};
	float m_VelocityThreshold{ 0.1f };

	float m_ZeroVelocityShootCooldown{ 1.0f };
	float m_NormalShootCooldown{ 2.0f };
	float m_CurrentShootCooldown{ 0.0f };

	PxRigidBody* m_pRigidBody{};

	void Move(const XMFLOAT2& dir, float deltaTime);
	void ChangeDirection();
	void Shoot();

};

