#pragma once
class Nest final : public GameObject
{
public: 
	Nest(const XMFLOAT3& loc);
	~Nest() override = default;
	Nest(const Nest& other) = delete;
	Nest(Nest&& other) = delete;
	Nest& operator=(const Nest& other) = delete;
	Nest& operator=(Nest&& other) = delete;
protected:
	virtual void Initialize(const SceneContext&) override;
private:
	const XMFLOAT3 m_Location;

	ModelComponent* m_pModelComponent{};
	ModelAnimator* m_pAnimator{};
	class DiffuseMaterial_Shadow_Skinned* m_pMaterial{};
	RigidBodyComponent* m_pRigidBody{};
};

