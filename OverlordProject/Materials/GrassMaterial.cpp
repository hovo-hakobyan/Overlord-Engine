#include "stdafx.h"
#include "GrassMaterial.h"

GrassMaterial::GrassMaterial():
	Material<GrassMaterial>(L"Effects/GrassShader.fx")
{

}

void GrassMaterial::InitializeEffectVariables()
{

}
