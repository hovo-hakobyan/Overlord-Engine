#pragma once
#include "Prefabs/BaseTank.h"
class PlayerTank : public BaseTank
{
public:
	PlayerTank(const XMFLOAT3& loc, const XMFLOAT3& startRot,const TankDesc& tankDesc);
	~PlayerTank() override = default;

	PlayerTank(const PlayerTank& other) = delete;
	PlayerTank(PlayerTank&& other) = delete;
	PlayerTank& operator=(const PlayerTank& other) = delete;
	PlayerTank& operator=(PlayerTank&& other) = delete;

	void Reset();
	void DisableMovement();
	void EnableMovement();
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext&) override;
private:
	float m_MaxShootCooldown{ 1.0f };
	float m_CurrentShootCooldown{ 1.0f };

	float m_AnimResetTime{ .5f };
	float m_CurrentAnimTime{};

	FMOD::Sound* m_pShootingSound{};
	FMOD::Channel* m_pShootingChannel{ nullptr };

	FMOD::Sound* m_pMovingSound{};
	FMOD::Channel* m_pMovingChannel{ nullptr };

	bool m_IsPlayingMovingSound{};
	bool m_ShouldMove{true};
};

