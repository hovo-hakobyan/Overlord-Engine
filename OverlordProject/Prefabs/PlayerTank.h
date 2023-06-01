#pragma once
class PlayerTank : public GameObject
{
public:
	PlayerTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot);
	~PlayerTank() override = default;

	PlayerTank(const PlayerTank& other) = delete;
	PlayerTank(PlayerTank&& other) = delete;
	PlayerTank& operator=(const PlayerTank& other) = delete;
	PlayerTank& operator=(PlayerTank&& other) = delete;

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
private:
	XMFLOAT3 m_StartLocation;
	XMFLOAT3 m_StartRotation;

	ModelComponent* m_pModelComponent{};
	ModelAnimator* m_pAnimator{};
	class DiffuseMaterial_Skinned* m_pMaterial{};
};

