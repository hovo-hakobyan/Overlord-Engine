#include "stdafx.h"
#include "SoftwareSkinningScene_3.h"
#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"

SoftwareSkinningScene_3::SoftwareSkinningScene_3():
	GameScene(L"Software Skinning Scene 3")
{
}

void SoftwareSkinningScene_3::Initialize()
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

	m_pBone0->CalculateBindPose();

	GameObject* pBoxDrawer = new GameObject();
	AddChild(pBoxDrawer);
	m_pMeshDrawer = pBoxDrawer->AddComponent(new MeshDrawComponent(24, true));

	InitializeVertices(15.f);
}

void SoftwareSkinningScene_3::Update()
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

	// transformation required to transform our bone from BindPose to its current orientation
	XMFLOAT4X4 bone1Transform, bone0Transform;

	XMFLOAT4X4 bindPose = m_pBone0->GetBindPose();
	XMFLOAT4X4 worldPos = m_pBone0->GetTransform()->GetWorld();
	XMMATRIX xmResult = XMMatrixMultiply(XMLoadFloat4x4(&bindPose), XMLoadFloat4x4(&worldPos));
	XMStoreFloat4x4(&bone0Transform, xmResult);

	bindPose = m_pBone1->GetBindPose();
	worldPos = m_pBone1->GetTransform()->GetWorld();
	xmResult = XMMatrixMultiply(XMLoadFloat4x4(&bindPose), XMLoadFloat4x4(&worldPos));
	XMStoreFloat4x4(&bone1Transform, xmResult);

	for (size_t i = 0; i < m_SkinnedVertices.size(); i++)
	{
		XMVECTOR originalPos = XMLoadFloat3(&m_SkinnedVertices[i].originalVertex.Position);	
		XMVECTOR res;

		if (m_SkinnedVertices[i].blendWeight0 < 1.f && m_SkinnedVertices[i].blendWeight1 < 1.f)
		{
			XMVECTOR transform0 = XMVector3TransformCoord(originalPos, XMLoadFloat4x4(&bone0Transform));
			XMVECTOR transform1 = XMVector3TransformCoord(originalPos, XMLoadFloat4x4(&bone1Transform));
			res = XMVectorLerp(transform0, transform1, m_SkinnedVertices[i].blendWeight0);
		}
		else
		{
			XMMATRIX transform;
			if (i <= 23)
			{
				transform = XMLoadFloat4x4(&bone0Transform);
			}
			else
			{
				transform = XMLoadFloat4x4(&bone1Transform);
			}

			res = XMVector3TransformCoord(originalPos, transform);
		}

		

		
		XMStoreFloat3(&m_SkinnedVertices[i].transformedVertex.Position, res);

	}

	m_pMeshDrawer->RemoveTriangles();
	for (size_t i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		m_pMeshDrawer->AddQuad(m_SkinnedVertices[i].transformedVertex,
			m_SkinnedVertices[i + 1].transformedVertex,
			m_SkinnedVertices[i + 2].transformedVertex,
			m_SkinnedVertices[i + 3].transformedVertex, true);
	}
}

void SoftwareSkinningScene_3::Draw()
{
}

void SoftwareSkinningScene_3::OnGUI()
{

	ImGui::BeginDisabled(m_ShoutAutoRotate);
	ImGui::DragFloat3("Bone0 rot", reinterpret_cast<float*>(&m_BoneRot0));
	ImGui::DragFloat3("Bone1 rot", reinterpret_cast<float*>(&m_BoneRot1));
	ImGui::EndDisabled();
	ImGui::Checkbox("Auto rot", &m_ShoutAutoRotate);
}

void SoftwareSkinningScene_3::InitializeVertices(float length)
{

	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col,1.f,0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, .5f, .5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, .5f, .5f);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.0f);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f,0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
#pragma endregion
}
