#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length):
	m_Length{length},
	m_pMaterial{pMaterial}
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(GetTransform()->GetWorldPosition().x+ m_Length, 0.f, 0.f);

	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	XMFLOAT4X4 worldMat = GetTransform()->GetWorld();
	XMMATRIX worldMatXM = XMLoadFloat4x4(&worldMat);
	XMMATRIX worldMatXMInv = XMMatrixInverse(nullptr,worldMatXM);
	XMStoreFloat4x4(&m_BindPose, worldMatXMInv);

	for (BoneObject* pChildBone : GetChildren<BoneObject>() )
	{
		pChildBone->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = new GameObject();
	AddChild(pEmpty);
	const auto pModel = new ModelComponent(L"Meshes/Bone.ovm");
	pModel->SetMaterial(m_pMaterial);

	pEmpty->AddComponent(pModel);

	//Orient the bone along x-axis
	pEmpty->GetTransform()->Rotate(0.f, -90.f, 0.f);
	pEmpty->GetTransform()->Scale(m_Length);

}
