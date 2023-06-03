#include "stdafx.h"
#include "VertexMaterial_Shadow_Skinned.h"

VertexMaterial_Shadow_Skinned::VertexMaterial_Shadow_Skinned():
	Material(L"Effects/Shadow/PosNormCol3D_Shadow_Skinned.fx")
{
}

void VertexMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void VertexMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	const auto pShadowMapRenderer = ShadowMapRenderer::Get();

	XMFLOAT4X4 lightWVP;
	XMMATRIX meshWorld = XMLoadFloat4x4(&pModel->GetTransform()->GetWorld());
	XMMATRIX lightVP = XMLoadFloat4x4(&pShadowMapRenderer->GetLightVP());
	XMStoreFloat4x4(&lightWVP, meshWorld * lightVP);

	SetVariable_Matrix(L"gWorldViewProj_Light", lightWVP);
	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
	SetVariable_MatrixArray(L"gBones", reinterpret_cast<const float*>(pModel->GetAnimator()->GetBoneTransforms().data()), static_cast<UINT>(pModel->GetAnimator()->GetBoneTransforms().size()));
}
