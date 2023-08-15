#pragma once
class PostVignette;
class VignetteTestScene final : public GameScene
{
public:
	VignetteTestScene();
	~VignetteTestScene() override = default;

	VignetteTestScene(const VignetteTestScene& other) = delete;
	VignetteTestScene(VignetteTestScene&& other) noexcept = delete;
	VignetteTestScene& operator=(const VignetteTestScene& other) = delete;
	VignetteTestScene& operator=(VignetteTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	PostVignette* m_pPostVignette{};
};

