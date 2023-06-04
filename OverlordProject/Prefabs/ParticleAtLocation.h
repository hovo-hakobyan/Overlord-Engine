#pragma once
class ParticleAtLocation final : public GameObject
{
public:
	ParticleAtLocation(const XMFLOAT3& loc, float duration, const ParticleEmitterSettings& settings, const std::wstring& filePath);
	~ParticleAtLocation() override = default;
	ParticleAtLocation(const ParticleAtLocation& other) = delete;
	ParticleAtLocation(ParticleAtLocation&& other) = delete;
	ParticleAtLocation& operator=(const ParticleAtLocation& other) = delete;
	ParticleAtLocation& operator=(ParticleAtLocation&& other) = delete;

	void Play();
	
protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;
private:
	float m_Duration{};
	XMFLOAT3 m_Location;
	ParticleEmitterSettings m_Settings{};
	ParticleEmitterComponent* m_pComponent{};
	std::wstring m_Path{};

	bool m_ShouldPlay{ false };
	bool m_HasBeenInitialized{ false };

	float m_CurrentDuration{0.0f};
};

