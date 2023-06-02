#pragma once
class MuzzleComponent : public BaseComponent
{
public:
	MuzzleComponent();
	~MuzzleComponent() override = default;

	MuzzleComponent(const MuzzleComponent& other) = delete;
	MuzzleComponent(MuzzleComponent&& other) noexcept = delete;
	MuzzleComponent& operator=(const MuzzleComponent& other) = delete;
	MuzzleComponent& operator=(MuzzleComponent&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext& /*sceneContext*/) override {};
private:
};

