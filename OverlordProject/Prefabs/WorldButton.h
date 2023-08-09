#pragma once
class DiffuseMaterial;
class WorldButton final : public GameObject
{
public: 
	WorldButton( const std::string& texturePath);
	~WorldButton() override = default;
	WorldButton(const WorldButton& other) = delete;
	WorldButton(WorldButton&& other) = delete;
	WorldButton& operator=(const WorldButton& other) = delete;
	WorldButton& operator=(WorldButton&& other) = delete;
protected:
	virtual void Initialize(const SceneContext&) override;
private:
	ModelComponent* m_pModelComp;
	DiffuseMaterial* m_pMaterial;
	const std::string m_Path;
};

