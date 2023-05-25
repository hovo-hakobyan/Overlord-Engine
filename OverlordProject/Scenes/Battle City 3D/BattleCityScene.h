#pragma once
#pragma once
class GrassMaterial;
class BattleCityScene final : public GameScene
{
public:
	BattleCityScene();
	~BattleCityScene() override = default;

	BattleCityScene(const BattleCityScene& other) = delete;
	BattleCityScene(BattleCityScene&& other) noexcept = delete;
	BattleCityScene& operator=(const BattleCityScene& other) = delete;
	BattleCityScene& operator=(BattleCityScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:

};

