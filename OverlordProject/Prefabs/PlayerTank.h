#pragma once

struct TankDesc
{
	TankDesc(
		PxMaterial* pMaterial,
		float halfForwardExtent = .4f,
		float halfSideExtent = .35f,
		float halfHeight = .25f)
	{
		controller.setToDefault();
		controller.halfForwardExtent = halfForwardExtent;
		controller.halfSideExtent = halfSideExtent;
		controller.halfHeight = halfHeight;
		controller.material = pMaterial;
	}

	float maxMoveSpeed{ 5.f };

	float moveAccelerationTime{ 0.3f };

	PxBoxControllerDesc controller{};

	float rotationSpeed{ 1.047198f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Shoot{ -1 };
};

class PlayerTank : public GameObject
{
public:
	PlayerTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot,const TankDesc& tankDesc);
	~PlayerTank() override = default;

	PlayerTank(const PlayerTank& other) = delete;
	PlayerTank(PlayerTank&& other) = delete;
	PlayerTank& operator=(const PlayerTank& other) = delete;
	PlayerTank& operator=(PlayerTank&& other) = delete;

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext&) override;
private:
	XMFLOAT3 m_StartLocation;
	XMFLOAT3 m_StartRotation;

	ModelComponent* m_pModelComponent{};
	ModelAnimator* m_pAnimator{};
	class DiffuseMaterial_Skinned* m_pMaterial{};

	float m_MoveSpeed{};
	float m_MoveAcceleration{};

	enum InputIds
	{
		MoveLeft,
		MoveRight,
		MoveForward,
		MoveBackward,
		Shoot
	};

	RigidBodyComponent* m_pRigidBody{};
	class BoxControllerComponent* m_pBoxControllerComponent{};
	TankDesc m_TankDesc;

};

