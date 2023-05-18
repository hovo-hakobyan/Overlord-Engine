#include "stdafx.h"
#include "GrassMaterial.h"

GrassMaterial::GrassMaterial():
	Material<GrassMaterial>(L"Effects/GrassShader.fx")
{

}

void GrassMaterial::UpdateTime(float gameTime)
{
	SetVariable_Scalar(L"gTime", gameTime);
}

void GrassMaterial::InitializeEffectVariables()
{
	
}
