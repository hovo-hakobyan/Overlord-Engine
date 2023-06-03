#pragma once
class VertexMaterial_Shadow_Skinned : public Material<VertexMaterial_Shadow_Skinned>
{
public:
	VertexMaterial_Shadow_Skinned();
	~VertexMaterial_Shadow_Skinned() override = default;

	VertexMaterial_Shadow_Skinned(const VertexMaterial_Shadow_Skinned& other) = delete;
	VertexMaterial_Shadow_Skinned(VertexMaterial_Shadow_Skinned&& other) noexcept = delete;
	VertexMaterial_Shadow_Skinned& operator=(const VertexMaterial_Shadow_Skinned& other) = delete;
	VertexMaterial_Shadow_Skinned& operator=(VertexMaterial_Shadow_Skinned&& other) noexcept = delete;

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const override;
private:

};

