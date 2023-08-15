#pragma once
class DiffuseMaterial;
class FlagDAE final : public GameObject
{
public:
	FlagDAE();
	~FlagDAE() override = default;
	FlagDAE(const FlagDAE& other) = delete;
	FlagDAE(FlagDAE&& other) = delete;
	FlagDAE& operator=(const FlagDAE& other) = delete;
	FlagDAE& operator=(FlagDAE&& other) = delete;
protected:
	virtual void Initialize(const SceneContext&) override;
private:
	ModelComponent* m_pModelComp;
	DiffuseMaterial* m_pFlagMaterial;
};
