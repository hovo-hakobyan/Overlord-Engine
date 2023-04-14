#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1():
	GameScene(L"Software Skinning Scene 1")
{
}

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	ColorMaterial* pColorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pColorMat->SetVariable_Vector(L"gColor", XMFLOAT4{ Colors::Green });

	GameObject* pRoot = new GameObject();
	AddChild(pRoot);

	m_pBone0 = new BoneObject(pColorMat, 15.f);
	m_pBone1 = new BoneObject(pColorMat, 15.f);
	
	pRoot->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);
}

void SoftwareSkinningScene_1::Update()
{
	if (m_ShoutAutoRotate)
	{
		float animationSpeed = 45.f;
		float deltaTime = m_SceneContext.pGameTime->GetElapsed();
		float upperBound = 45.f;
		float lowerBound = -45.f;

		if (m_BoneRotation >= upperBound || m_BoneRotation <= lowerBound)
		{
			m_RotationSign *= -1;
		}

		m_BoneRotation += animationSpeed * deltaTime * m_RotationSign;
		m_BoneRot0 = XMFLOAT3{ 0.f,0.f,m_BoneRotation };
		m_BoneRot1 = XMFLOAT3{ 0.f,0.f,-m_BoneRotation * 2.f };

	
	}

	m_pBone0->GetTransform()->Rotate(m_BoneRot0);
	m_pBone1->GetTransform()->Rotate(m_BoneRot1);
}
	

void SoftwareSkinningScene_1::Draw()
{
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::BeginDisabled(m_ShoutAutoRotate);
	ImGui::DragFloat3("Bone0 rot", reinterpret_cast<float*>(&m_BoneRot0));
	ImGui::DragFloat3("Bone1 rot", reinterpret_cast<float*>(&m_BoneRot1));
	ImGui::EndDisabled();
	ImGui::Checkbox("Auto rot", &m_ShoutAutoRotate);

}
