#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	delete[] m_ParticlesArray;
	m_ParticlesArray = nullptr;

	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	if (!m_pParticleMaterial)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	}
	
	CreateVertexBuffer(sceneContext);

	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	if (m_pVertexBuffer)
	{
		SafeRelease(m_pVertexBuffer);
	}

	//*************
		//VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBuffDesc{};
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.ByteWidth = sizeof(VertexParticle) * m_ParticleCount;
	vertexBuffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vertexBuffDesc.MiscFlags = 0;

	HRESULT result = sceneContext.d3dContext.pDevice->CreateBuffer(&vertexBuffDesc, nullptr, &m_pVertexBuffer);
	if (FAILED(result))
	{
		Logger::LogError(L"Couldn't create vertex buffer");
	}
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	const float deltaTime = sceneContext.pGameTime->GetElapsed();
	const float particleInterval = ((m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / 2.0f) / m_ParticleCount;

	m_LastParticleSpawn += deltaTime;
	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	VertexParticle* pBuffer = reinterpret_cast<VertexParticle*>(mappedSubResource.pData);
	
	for (UINT i = 0; i < m_ParticleCount; i++)
	{
		Particle& particle = m_ParticlesArray[i];
		if (particle.isActive)
		{
			UpdateParticle(particle, deltaTime);
		}

		if (!particle.isActive && m_LastParticleSpawn >= particleInterval)
		{
			SpawnParticle(particle);
			m_LastParticleSpawn -= particleInterval;
		}

		if (particle.isActive)
		{
			pBuffer[m_ActiveParticles] = particle.vertexInfo;
			++m_ActiveParticles;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	if (!p.isActive)
		return;

	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy < 0.f)
	{
		p.isActive = false;
		return;
	}

	XMVECTOR vel = XMLoadFloat3(&m_EmitterSettings.velocity) * elapsedTime;
	XMVECTOR pos = XMLoadFloat3(&p.vertexInfo.Position);
	XMFLOAT3 resultPos;
	XMStoreFloat3(&resultPos, pos + vel);
	p.vertexInfo.Position = resultPos;

	float lifePercentage = p.currentEnergy / p.totalEnergy;
	XMFLOAT4 color = m_EmitterSettings.color;
	color.w *= lifePercentage;
	p.vertexInfo.Color = color;


	float finalSize = p.initialSize * p.sizeChange;
	p.vertexInfo.Size = p.initialSize + (finalSize - p.initialSize) * (1 -lifePercentage);

	
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	p.isActive = true;
	
	p.totalEnergy = p.currentEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);

	XMVECTOR randomDir = XMVectorSet(MathHelper::randF(-1.0f,1.0f), MathHelper::randF(-1.0f, 1.0f), MathHelper::randF(-1.0f, 1.0f), 0.0f);
	randomDir = XMVector3Normalize(randomDir);
	XMMATRIX randRotMatrix = XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI));
	randomDir = XMVector3TransformNormal(randomDir, randRotMatrix);
	float distance = MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius);
	XMStoreFloat3(&p.vertexInfo.Position,XMLoadFloat3(&GetTransform()->GetWorldPosition())+ randomDir * distance);

	p.vertexInfo.Size = p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);

	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	auto& techniqueContext = m_pParticleMaterial->GetTechniqueContext();
	auto& pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	pDeviceContext->IASetInputLayout(techniqueContext.pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	const UINT stride = sizeof(VertexParticle);
	const UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//DRAW
	D3DX11_TECHNIQUE_DESC techDesc;
	techniqueContext.pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		techniqueContext.pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_ActiveParticles,0,0);
	}

}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}