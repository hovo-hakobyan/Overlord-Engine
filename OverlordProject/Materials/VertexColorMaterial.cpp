#include "stdafx.h"
#include "VertexColorMaterial.h"

VertexColorMaterial::VertexColorMaterial():
	Material<VertexColorMaterial>(L"Effects/PosNormCol3D.fx")
{
}

void VertexColorMaterial::InitializeEffectVariables()
{
}
