#pragma once
class LevelSelectScene : public GameScene
{
public:
	LevelSelectScene();
	~LevelSelectScene() override;

	LevelSelectScene(const LevelSelectScene& other) = delete;
	LevelSelectScene(LevelSelectScene&& other) noexcept = delete;
	LevelSelectScene& operator=(const LevelSelectScene& other) = delete;
	LevelSelectScene& operator=(LevelSelectScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;

private:
	class PlayerTank* m_pPlayerTank{};

	void LockCamera();
};

