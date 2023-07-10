#pragma once
class LavaMaterial;
class LavaTestScene final : public GameScene
{
public:
	LavaTestScene();
	~LavaTestScene() override = default;

	LavaTestScene(const LavaTestScene& other) = delete;
	LavaTestScene(LavaTestScene&& other) noexcept = delete;
	LavaTestScene& operator=(const LavaTestScene& other) = delete;
	LavaTestScene& operator=(LavaTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	LavaMaterial* m_pLavaMat{};
};

