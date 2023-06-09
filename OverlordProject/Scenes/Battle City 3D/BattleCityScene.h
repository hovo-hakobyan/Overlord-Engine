#pragma once
class ParticleAtLocation;
enum class CurrentGameState
{
	Gameplay, Victory, Defeat
};

class ShellManager;
class BattleCityScene final : public GameScene
{
public:
	BattleCityScene();
	~BattleCityScene() override;

	BattleCityScene(const BattleCityScene& other) = delete;
	BattleCityScene(BattleCityScene&& other) noexcept = delete;
	BattleCityScene& operator=(const BattleCityScene& other) = delete;
	BattleCityScene& operator=(BattleCityScene&& other) noexcept = delete;

	void SetGameState(const CurrentGameState& gameState) { m_GameState = gameState; }
	const CurrentGameState& GetGameState() const { return m_GameState; }

	ShellManager* GetShellManager() const { return m_pShellManager; }
	
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void PostDraw() override;

private:
	class LevelBuilder* m_pLevelBuilder{};
	class EnemySpawner* m_pEnemySpawner{};
	ShellManager* m_pShellManager{};
	
	CurrentGameState m_GameState{ CurrentGameState::Gameplay };
	class PlayerTank* m_pPlayerTank{};

	ParticleAtLocation* m_pDefeatParticle{};
	ParticleAtLocation* m_pVictoryParticle{};
	
	void LockCamera();
};

