#pragma once
#pragma once

class BoxControllerComponent : public BaseComponent
{
public:
	BoxControllerComponent(const PxBoxControllerDesc& controllerDesc);
	~BoxControllerComponent() override = default;

	BoxControllerComponent(const BoxControllerComponent& other) = delete;
	BoxControllerComponent(BoxControllerComponent&& other) noexcept = delete;
	BoxControllerComponent& operator=(const BoxControllerComponent& other) = delete;
	BoxControllerComponent& operator=(BoxControllerComponent&& other) noexcept = delete;

	void Translate(const XMFLOAT3& pos) const;
	void Move(const XMFLOAT3& displacement, float minDistance = 0);

	void SetCollisionGroup(CollisionGroup groups);
	void SetCollisionIgnoreGroup(CollisionGroup ignoreGroups);

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetFootPosition() const;
	PxControllerCollisionFlags GetCollisionFlags() const { return m_CollisionFlag; }
	PxController* GetPxController() const { return m_pController; }
	PxShape* GetBoxShape() const { return m_pShape; }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void OnSceneDetach(GameScene*) override;
	void OnSceneAttach(GameScene*) override;
	void OnOwnerAttach(GameObject*) override;
	void OnOwnerDetach(GameObject*) override;

private:

	PxBoxControllerDesc m_ControllerDesc{};

	PxController* m_pController{};
	PxShape* m_pShape{};
	PxControllerCollisionFlags m_CollisionFlag{};
	PxFilterData m_CollisionGroups{ static_cast<UINT32>(CollisionGroup::Group0), 0, 0, 0 };

	void ApplyFilterData() const;
};

