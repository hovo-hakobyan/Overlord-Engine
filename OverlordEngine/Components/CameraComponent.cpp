#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active?this:nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups) const
{
	const POINT cursorPosition = InputManager::GetMousePosition();
	const float viewportHalfWidth = m_pScene->GetSceneContext().windowWidth / 2.0f;
	const float viewportHalfHeight = m_pScene->GetSceneContext().windowHeight / 2.0f;

	const float xNdc = (cursorPosition.x - viewportHalfWidth) / viewportHalfWidth;
	const float yNdc = (viewportHalfHeight - cursorPosition.y) / viewportHalfHeight;

	XMFLOAT3 nearInput{ xNdc,yNdc,0.f };
	XMFLOAT3 farInput{ xNdc,yNdc,1.0f };
	XMMATRIX viewInverse = XMLoadFloat4x4(&m_ViewProjectionInverse);

	XMVECTOR nearPoint = XMVector3TransformCoord(XMLoadFloat3(&nearInput),viewInverse);
	XMVECTOR farPoint = XMVector3TransformCoord(XMLoadFloat3(&farInput),viewInverse);

	XMFLOAT3 startPos, endPos; 
	XMStoreFloat3(&startPos, nearPoint);
	XMStoreFloat3(&endPos, farPoint);

	PxQueryFilterData filterData{};
	//bitwise not ~
	filterData.data.word0 = ~UINT(ignoreGroups);

	PxVec3 origin = PxVec3{ startPos.x,startPos.y,startPos.z };
	PxVec3 direction = PxVec3{ endPos.x - origin.x,endPos.y - origin.y,endPos.z -origin.z};
	direction.normalize();

	PxRaycastBuffer hit{};
	if (m_pScene->GetPhysxProxy()->Raycast(origin,direction,PX_MAX_F32,hit,PxHitFlag::eDEFAULT,filterData))
	{
		return static_cast<BaseComponent*>(hit.block.actor->userData)->GetGameObject();
	}

	return nullptr;
}