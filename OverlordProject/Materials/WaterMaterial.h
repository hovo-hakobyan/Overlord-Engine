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

	

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pWaterTexture{};
	TextureData* m_pFoamTexture{};
	TextureData* m_pNormalTexture{};
	TextureData* m_pNoiseTexture{};

};

