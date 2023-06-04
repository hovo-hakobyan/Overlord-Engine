#pragma once
class DiffuseMaterial_Shadow;
class BrickWall final : public GameObject
{
public:
	BrickWall(const XMFLOAT3& loc, const XMFLOAT3& scale,DiffuseMaterial_Shadow* pMat );
	~BrickWall() override = default;
	BrickWall(const BrickWall& other) = delete;
	BrickWall(BrickWall&& other) = delete;
	BrickWall& operator=(const BrickWall& other) = delete;
	BrickWall& operator=(BrickWall&& other) = delete;
protected:
	virtual void Initialize(const SceneContext&) override;
private:
	const XMFLOAT3 m_Location;
	const XMFLOAT3 m_Scale;

	 DiffuseMaterial_Shadow* m_pBrickWallMaterial{};
};

