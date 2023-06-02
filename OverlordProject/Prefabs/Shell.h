#pragma once
class Shell final : public GameObject
{
public:
	Shell(const XMFLOAT3& loc, const XMFLOAT3& rot,const XMFLOAT3& dir, GameObject* parent);
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
	GameObject* m_pParent;
	class DiffuseMaterial_Shadow* m_pMat{};

	float m_Lifetime{ 5.0f };
	float m_CurrentLifeTime{};
};

