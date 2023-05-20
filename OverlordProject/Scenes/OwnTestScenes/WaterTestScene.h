#pragma once
class WaterMaterial;
class WaterTestScene final : public GameScene
{
public:
	WaterTestScene();
	~WaterTestScene() override = default;

	WaterTestScene(const WaterTestScene& other) = delete;
	WaterTestScene(WaterTestScene&& other) noexcept = delete;
	WaterTestScene& operator=(const WaterTestScene& other) = delete;
	WaterTestScene& operator=(WaterTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	WaterMaterial* m_pWaterMat{};
};

