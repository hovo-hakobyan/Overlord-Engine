#pragma once
class BattleCityScene;
class Shell final : public GameObject
{
public:
	Shell(const XMFLOAT3& pos, const XMFLOAT3& rot,const std::wstring& parentTag, const XMFLOAT3& dir);
	~Shell() override = default;
	Shell(const Shell& other) = delete;
	Shell(Shell&& other) = delete;
	Shell& operator=(const Shell& other) = delete;
	Shell& operator=(Shell&& other) = delete;
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext&) override;
private:
	XMFLOAT3 m_Direction;
	XMFLOAT3 m_Loc;
	XMFLOAT3 m_Rot;

	class DiffuseMaterial_Shadow* m_pMat{};

	float m_Lifetime{ 6.0f };
	float m_CurrentLifeTime{};

	RigidBodyComponent* m_pRigidBody{};
	GameObject* m_pHitObject{};
	BattleCityScene* m_pGameScene{};
	ModelComponent* m_pModelComponent{};

	float m_Speed{ 50.0f };
	bool m_IsEnabled = true;
	bool m_IsDead = false;

	std::wstring m_ParentTag{};

	ParticleEmitterComponent* m_pShotParticleEmitter{};
	ParticleEmitterComponent* m_pExplosionEmitter{};

	float m_ParticleLifeTime{ 0.2f };
	float m_DisabledLifetime{ 0.5f };
	float m_CurrentDisabledTime{ 0.0f };

	void Disable();

	bool m_ShouldDisable = true;

	FMOD::Sound* m_pBrickSound{};
	FMOD::Sound* m_pEnemyDeathSound{};
	FMOD::Sound* m_pFriendlyDeathSound{};
	FMOD::Channel* m_pChannelBrick{nullptr};
	FMOD::Channel* m_pChannelEnemyDeath{ nullptr };
	FMOD::Channel* m_pChannelFriendlyDeath{ nullptr };
};

