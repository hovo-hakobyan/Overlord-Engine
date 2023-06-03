#pragma once
class Shell final : public GameObject
{
public:
	Shell(const XMFLOAT3& shellLoc, const XMFLOAT3& rot,const XMFLOAT3& dir, GameScene* gameScene,const std::wstring& parentTag);
	~Shell() override = default;
	Shell(const Shell& other) = delete;
	Shell(Shell&& other) = delete;
	Shell& operator=(const Shell& other) = delete;
	Shell& operator=(Shell&& other) = delete;
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext&) override;
private:
	XMFLOAT3 m_Location;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Direction;
	GameScene* m_pParent;
	class DiffuseMaterial_Shadow* m_pMat{};

	float m_Lifetime{ 5.0f };
	float m_CurrentLifeTime{};

	RigidBodyComponent* m_pRigidBody{};
	GameObject* m_pHitObject{};

	float m_Speed{ 50.0f };
	bool m_IsEnabled = true;

	std::wstring m_ParentTag{};
};

