#pragma once
class ShellManager final : public GameObject
{
public:
	ShellManager();
	~ShellManager() override = default;
	ShellManager(const ShellManager& other) = delete;
	ShellManager(ShellManager&& other) noexcept = delete;
	ShellManager& operator=(const ShellManager& other) = delete;
	ShellManager& operator=(ShellManager&& other) noexcept = delete;

	void SpawnShell(const XMFLOAT3& pos,const XMFLOAT3& rot, const XMFLOAT3& dir, const std::wstring& parentTag);
protected:
	virtual void Initialize(const SceneContext&) override {};
	virtual void Update(const SceneContext&) override {};
private:

};

