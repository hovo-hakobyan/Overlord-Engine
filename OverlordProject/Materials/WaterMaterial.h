#pragma once
class WaterMaterial final :public Material<WaterMaterial>
{
public:
	WaterMaterial();
	~WaterMaterial() override = default;

	WaterMaterial(const WaterMaterial& other) = delete;
	WaterMaterial(WaterMaterial&& other) noexcept = delete;
	WaterMaterial& operator=(const WaterMaterial& other) = delete;
	WaterMaterial& operator=(WaterMaterial&& other) noexcept = delete;

	void UpdateTime(float gameTime);

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pWaterTexture{};

};

