#include "stdafx.h"
#include "BaseTank.h"
#include "Prefabs/Hatch.h"

BaseTank::BaseTank(Hatch* pSpawnHatch, const XMFLOAT3& startRot, const TankDesc& tankDesc, GameScene* gameScene):
	m_pHatch{ pSpawnHatch },
	m_StartRotation{ startRot },
	m_TankDesc{ tankDesc },
	m_MoveAcceleration{ tankDesc.maxMoveSpeed / tankDesc.moveAccelerationTime },
	m_pGameScene{ gameScene }
{
	m_StartLocation = m_pHatch->GetTransform()->GetPosition();
}
