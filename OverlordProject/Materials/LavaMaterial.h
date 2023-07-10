#pragma once
class LavaMaterial final :public Material<LavaMaterial>
{
public:
	LavaMaterial();
	~LavaMaterial() override = default;

	LavaMaterial(const LavaMaterial& other) = delete;
	LavaMaterial(LavaMaterial&& other) noexcept = delete;
	LavaMaterial& operator=(const LavaMaterial& other) = delete;
	LavaMaterial& operator=(LavaMaterial&& other) noexcept = delete;



protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pLavaTexture{};
	TextureData* m_pNoiseTexture{};

};

