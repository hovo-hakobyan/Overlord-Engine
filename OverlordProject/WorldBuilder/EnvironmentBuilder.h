#pragma once
class LavaMaterial;
class BattleCityScene;
class EnvironmentBuilder final
{
public:
	EnvironmentBuilder(BattleCityScene* gameScene);
	~EnvironmentBuilder() = default;
	EnvironmentBuilder(const EnvironmentBuilder& other) = delete;
	EnvironmentBuilder(EnvironmentBuilder&& other) noexcept = delete;
	EnvironmentBuilder& operator=(const EnvironmentBuilder& other) = delete;
	EnvironmentBuilder& operator=(EnvironmentBuilder&& other) noexcept = delete;

	void BuildLavaEnv();
private:

	BattleCityScene* m_pGameScene;

};