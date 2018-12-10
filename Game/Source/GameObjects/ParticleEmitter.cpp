#include "GamePCH.h"
#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
{

	m_Renderer = RENDERER::PARTICLE_RENDERER;

	m_ActiveParticles = new std::vector<Particle*>;
	m_ActiveParticles->reserve(5);

	m_InactiveParticles = new std::vector<Particle*>;
	m_InactiveParticles->reserve(5);

	for (unsigned int i = 0; i < 5; i++)
	{
		m_ActiveParticles->push_back(nullptr);
		m_InactiveParticles->push_back(nullptr);
	}

	TOTAL_PARTICLES = 0;
}

ParticleEmitter::~ParticleEmitter()
{
	//these vectors should be the same size so delete them both
	for (unsigned int i = 0; i < m_ActiveParticles->size(); i++)
	{
		if (m_ActiveParticles->at(i) != nullptr)
		{
			delete m_ActiveParticles->at(i);
			m_ActiveParticles->at(i) = nullptr;
		}

		if (m_InactiveParticles->at(i) != nullptr)
		{
			delete m_InactiveParticles->at(i);
			m_InactiveParticles->at(i) = nullptr;
		}
	}

	delete m_ActiveParticles;
	delete m_InactiveParticles;
}

void ParticleEmitter::Update(float deltatime)
{
	//check all active particles and remove any past their time limit
	for (unsigned int i = 0; i < m_ActiveParticles->size(); i++)
	{
		if (m_ActiveParticles->at(i) != nullptr)
		{

			//increment their duration
			m_ActiveParticles->at(i)->duration += deltatime;

			//if their time limit is more than 0 (0 means permanent) ignore
			if (m_ActiveParticles->at(i)->time_limit > 0.0f)
			{
				//If their duration is past their limit, remove them from the active particle vector
				if (m_ActiveParticles->at(i)->duration >= m_ActiveParticles->at(i)->time_limit)
				{
					m_ActiveParticles->at(i)->duration = 0.0f;
					m_InactiveParticles->at(i) = m_ActiveParticles->at(i);
					m_ActiveParticles->at(i) = nullptr;
				}
			}
		}
	}


	//add all active particles to the partile renderer
	for (unsigned int i = 0; i < m_ActiveParticles->size(); i++)
		if (m_ActiveParticles->at(i) != nullptr)
			m_Renderer->AddParticle(m_ActiveParticles->at(i));

}

void ParticleEmitter::AddParticle(WorldTransform * transform, AtlasChild ** image, int texture_unit, float time_limit, bool IsActive)
{
	if (IsActive == true)
	{
		m_ActiveParticles->at(TOTAL_PARTICLES) = new Particle();
		m_ActiveParticles->at(TOTAL_PARTICLES)->transform = *transform;
		m_ActiveParticles->at(TOTAL_PARTICLES)->image = image;
		m_ActiveParticles->at(TOTAL_PARTICLES)->texture_unit = texture_unit;
		m_ActiveParticles->at(TOTAL_PARTICLES)->time_limit = time_limit;
	}
	else
	{
		m_InactiveParticles->at(TOTAL_PARTICLES) = new Particle();
		m_InactiveParticles->at(TOTAL_PARTICLES)->transform = *transform;
		m_InactiveParticles->at(TOTAL_PARTICLES)->image = image;
		m_InactiveParticles->at(TOTAL_PARTICLES)->texture_unit = texture_unit;
		m_InactiveParticles->at(TOTAL_PARTICLES)->time_limit = time_limit;
	}

	TOTAL_PARTICLES++;
}

void ParticleEmitter::SetParticleActive(int index)
{
	if (m_InactiveParticles->at(index) != nullptr)
	{
		m_ActiveParticles->at(index) = m_InactiveParticles->at(index);
		m_InactiveParticles->at(index) = nullptr;
	}
}

Particle * ParticleEmitter::GetParticle(int index)
{
	if (m_ActiveParticles->at(index) != nullptr)
		return m_ActiveParticles->at(index);
	else
		if (m_InactiveParticles->at(index) != nullptr)
			return m_InactiveParticles->at(index);

	//if we get here we tried to access a particle that doesnt exist
	assert(false);
	return new Particle();
}
