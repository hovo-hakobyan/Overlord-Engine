#pragma once
#include "Prefabs/BaseTank.h"
class PlayerTank : public BaseTank
{
public:
	PlayerTank(Hatch* pspawnHatch, const XMFLOAT3& startRot,const TankDesc& tankDesc, GameScene* gameScene);
	~PlayerTank() override = default;

	PlayerTank(const PlayerTank& other) = delete;
	PlayerTank(PlayerTank&& other) = delete;
	PlayerTank& operator=(const PlayerTank& other) = delete;
	PlayerTank& operator=(PlayerTank&& other) = delete;

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext&) override;
private:
	float m_MaxShootCooldown{ 1.0f };
	float m_CurrentShootCooldown{ 1.0f };

};

