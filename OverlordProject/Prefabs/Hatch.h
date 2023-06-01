#pragma once
class Hatch final : public GameObject
{
public: 
	Hatch(const XMFLOAT3& loc, const XMFLOAT3& rot, const std::wstring& filePath);
	~Hatch() override = default;
	Hatch(const Hatch& other) = delete;
	Hatch(Hatch&& other) = delete;
	Hatch& operator=(const Hatch& other) = delete;
	Hatch& operator=(Hatch&& other) = delete;

	void CreateMaterial(const std::wstring& diffusePath, const std::wstring& normalPath);
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
private:
	XMFLOAT3 m_Location;
	XMFLOAT3 m_Rotation;
	const std::wstring m_FilePath;


	ModelComponent* m_pModelComponent{};
	class SimpleDiffuseMaterial* m_pMaterial{};
};

