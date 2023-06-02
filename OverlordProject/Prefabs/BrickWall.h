#pragma once
class BrickWall final : public GameObject
{
public:
	BrickWall(const XMFLOAT3& loc, const XMFLOAT3& scale);
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

	class SimpleDiffuseMaterial* m_pBrickWallMaterial{};
};

