#pragma once
class LavaMaterial;
class DiffuseMaterial_Shadow;
class DiffuseMaterial;
class EnvironmentBuilder final
{
public:
	EnvironmentBuilder();
	~EnvironmentBuilder() = default;
	EnvironmentBuilder(const EnvironmentBuilder& other) = delete;
	EnvironmentBuilder(EnvironmentBuilder&& other) noexcept = delete;
	EnvironmentBuilder& operator=(const EnvironmentBuilder& other) = delete;
	EnvironmentBuilder& operator=(EnvironmentBuilder&& other) noexcept = delete;

	
	static void BuildLavaEnv(GameScene* pScene);

private:

	static LavaMaterial* m_pLavaMat;
	static DiffuseMaterial_Shadow* m_pIslandGrassMat;
	static DiffuseMaterial_Shadow* m_pPyramidMat;
	static DiffuseMaterial_Shadow* m_pShrubberyMat;
	static DiffuseMaterial_Shadow* m_pLavaBubble;
	static DiffuseMaterial_Shadow* m_pPalmMat1;
	static DiffuseMaterial_Shadow* m_pPalmMat2;
	static DiffuseMaterial* m_pSandMat;
	static DiffuseMaterial_Shadow* m_pVolcGrassMat;
	static DiffuseMaterial_Shadow* m_pVolcBaseMat;

	static bool m_IsInitialized;

	static void Initialize();
};