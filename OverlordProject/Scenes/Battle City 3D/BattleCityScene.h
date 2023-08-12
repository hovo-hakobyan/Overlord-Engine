#pragma once
class PauseMenu;
class EndScreen;
enum class CurrentGameState
{
	Gameplay, Victory, Defeat
};

class ShellManager;
class BattleCityScene final : public GameScene
{
public:
	BattleCityScene(const std::string& path);
	~BattleCityScene() override;

	BattleCityScene(const BattleCityScene& other) = delete;
	BattleCityScene(BattleCityScene&& other) noexcept = delete;
	BattleCityScene& operator=(const BattleCityScene& other) = delete;
	BattleCityScene& operator=(BattleCityScene&& other) noexcept = delete;

	void SetGameState(const CurrentGameState& gameState) { m_GameState = gameState; }
	const CurrentGameState& GetGameState() const { return m_GameState; }

	ShellManager* GetShellManager() const { return m_pShellManager; }

	bool IsGamePaused() const;
	
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void PostDraw() override;
	void OnSceneDeactivated() override;

private:
	enum InputIds
	{
		Pause=5
	};
	class LevelBuilder* m_pLevelBuilder{};
	ModelComponent* m_pGrowingLava{};
	class EnemySpawner* m_pEnemySpawner{};
	ShellManager* m_pShellManager{};
	
	CurrentGameState m_GameState{ CurrentGameState::Gameplay };
	class PlayerTank* m_pPlayerTank{};

	EndScreen* m_pEndScreen{};
	
	const float m_LavaGrowSpeed{ 0.005f };
	bool m_HasLavaFinishedGrowing{ false };

	std::string m_LevelPath{};

	bool m_IsPaused{false};
	PauseMenu* m_pPauseMenu{};

	void LockCamera();
};

