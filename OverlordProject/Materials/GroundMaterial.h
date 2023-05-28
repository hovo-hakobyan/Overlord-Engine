#pragma once
class GroundMaterial final : public Material<GroundMaterial>
{
public:
	GroundMaterial();
	~GroundMaterial() override = default;

	GroundMaterial(const GroundMaterial& other) = delete;
	GroundMaterial(GroundMaterial&& other) noexcept = delete;
	GroundMaterial& operator=(const GroundMaterial& other) = delete;
	GroundMaterial& operator=(GroundMaterial&& other) noexcept = delete;


protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pGroundDiffuse{};
	TextureData* m_pGroundNormalMap{};
};


