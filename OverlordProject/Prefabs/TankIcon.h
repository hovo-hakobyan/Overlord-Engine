#pragma once

class TankIcon final : public GameObject
{
public:
	TankIcon(const XMFLOAT2& loc, const XMFLOAT2& dimensions);
	~TankIcon() override = default;
	TankIcon(const TankIcon& other) = delete;
	TankIcon(TankIcon&& other) = delete;
	TankIcon& operator=(const TankIcon& other) = delete;
	TankIcon& operator=(TankIcon&& other) = delete;

	void Show();
protected:
	virtual void Initialize(const SceneContext&) override;
private:
	XMFLOAT2 m_Location{};
	XMFLOAT2 m_Dimensions{};
	SpriteComponent* m_pSpriteComponent{};

	
};

