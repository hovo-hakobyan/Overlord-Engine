#pragma once
class GrassMaterial;
class GrassTestScene final : public GameScene
{
public:
	GrassTestScene();
	~GrassTestScene() override = default;

	GrassTestScene(const GrassTestScene& other) = delete;
	GrassTestScene(GrassTestScene&& other) noexcept = delete;
	GrassTestScene& operator=(const GrassTestScene& other) = delete;
	GrassTestScene& operator=(GrassTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	GrassMaterial* m_pGrassMat{};
};

