#include "stdafx.h"
#include "BaseTank.h"
#include "Scenes/Battle City 3D/BattleCityScene.h"

int BaseTank::EnemyCounter = 0;
BaseTank::BaseTank(const XMFLOAT3& loc, const XMFLOAT3& startRot, const TankDesc& tankDesc, BattleCityScene* gameScene):
	m_StartLocation{ loc },
	m_StartRotation{ startRot },
	m_TankDesc{ tankDesc },
	m_MoveAcceleration{ tankDesc.maxMoveSpeed / tankDesc.moveAccelerationTime },
	m_pGameScene{ gameScene }
{

}
