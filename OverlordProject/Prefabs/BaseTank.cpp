#include "stdafx.h"
#include "BaseTank.h"

BaseTank::BaseTank(const XMFLOAT3& startLoc, const XMFLOAT3& startRot, const TankDesc& tankDesc, GameScene* gameScene):
	m_StartLocation{ startLoc },
	m_StartRotation{ startRot },
	m_TankDesc{ tankDesc },
	m_MoveAcceleration{ tankDesc.maxMoveSpeed / tankDesc.moveAccelerationTime },
	m_pGameScene{ gameScene }
{
}
