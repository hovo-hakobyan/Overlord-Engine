#pragma once
class SimpleDiffuseMaterial final : public Material<SimpleDiffuseMaterial>
{
public:
	SimpleDiffuseMaterial();
	~SimpleDiffuseMaterial() override = default;

	SimpleDiffuseMaterial(const SimpleDiffuseMaterial& other) = delete;
	SimpleDiffuseMaterial(SimpleDiffuseMaterial&& other) noexcept = delete;
	SimpleDiffuseMaterial& operator=(const SimpleDiffuseMaterial& other) = delete;
	SimpleDiffuseMaterial& operator=(SimpleDiffuseMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetNormalMapTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pSimpleDiffuse{};
	TextureData* m_pSimpleDiffuseNormalMap{};
};


