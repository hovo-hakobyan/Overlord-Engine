#include "stdafx.h"
#include "ShellManager.h"
#include "Prefabs/Shell.h"

ShellManager::ShellManager()
{
}


void ShellManager::SpawnShell(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& dir, const std::wstring& parentTag)
{
	auto pShell = new Shell(pos,rot, parentTag, dir);
	AddChild(pShell);
	
}


