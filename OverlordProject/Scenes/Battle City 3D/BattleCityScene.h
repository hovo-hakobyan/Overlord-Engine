#pragma once
#pragma once
enum class CurrentGameState
{
	Gameplay, Victory, Defeat
};

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

	
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	class LevelBuilder* m_pLevelBuilder{};
	class EnemySpawner* m_pEnemySpawner{};

	void LockCamera();

	CurrentGameState m_GameState{ CurrentGameState::Gameplay };
};

