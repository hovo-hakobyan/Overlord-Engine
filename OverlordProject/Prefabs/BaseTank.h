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


	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Shoot{ -1 };
};
class BaseTank : public GameObject
{
public:
	BaseTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot, const TankDesc& tankDesc, GameScene* gameScene);
	~BaseTank() override = default;

	BaseTank(const BaseTank& other) = delete;
	BaseTank(BaseTank&& other) = delete;
	BaseTank& operator=(const BaseTank& other) = delete;
	BaseTank& operator=(BaseTank&& other) = delete;
protected:
	virtual void Initialize(const SceneContext& /*sceneContext*/) override {};
	virtual void Update(const SceneContext&) override {};

	XMFLOAT3 m_StartLocation;
	XMFLOAT3 m_StartRotation;

	ModelComponent* m_pModelComponent{};
	ModelAnimator* m_pAnimator{};
	class DiffuseMaterial_Shadow_Skinned* m_pMaterial{};

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
	class BoxControllerComponent* m_pBoxControllerComponent{};
	GameScene* m_pGameScene;
	TankDesc m_TankDesc;
	PxShape* m_pBoxShape;
private:
};

