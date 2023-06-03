#pragma once
class VertexColorMaterial final : public Material<VertexColorMaterial>
{
public:
	VertexColorMaterial();
	~VertexColorMaterial() override = default;

	VertexColorMaterial(const VertexColorMaterial& other) = delete;
	VertexColorMaterial(VertexColorMaterial&& other) noexcept = delete;
	VertexColorMaterial& operator=(const VertexColorMaterial& other) = delete;
	VertexColorMaterial& operator=(VertexColorMaterial&& other) noexcept = delete;
protected:
	void InitializeEffectVariables() override;
};

