#pragma once
class UberMaterial final :public Material<UberMaterial>
{
public: 
	UberMaterial();
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture{};
	TextureData* m_pSpecularTexture{};
	TextureData* m_pNormal{};
	TextureData* m_pEnvironment{};
	TextureData* m_pOpacity{};
};

