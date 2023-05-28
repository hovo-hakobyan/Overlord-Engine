#pragma once
class GrassMaterial final :public Material<GrassMaterial>
{
public:
	GrassMaterial();
	~GrassMaterial() override = default;

	GrassMaterial(const GrassMaterial& other) = delete;
	GrassMaterial(GrassMaterial&& other) noexcept = delete;
	GrassMaterial& operator=(const GrassMaterial& other) = delete;
	GrassMaterial& operator=(GrassMaterial&& other) noexcept = delete;


protected:
	void InitializeEffectVariables() override;

};

