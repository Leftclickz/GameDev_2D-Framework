#pragma once

class ParticleRenderer;

class ParticleEmitter

{

public:

	ParticleEmitter();
	virtual ~ParticleEmitter();

	virtual void Update(float deltatime);

	//add a new particle. Requires a transform (in world coords), an image, a texture unit, a time limit (set 0 for infinite duration), and whether its active right away or not.
	virtual void AddParticle(WorldTransform* transform, AtlasChild** image, int texture_unit, float time_limit, bool IsActive = true);

	virtual void SetParticleActive(int index);

	virtual Particle* GetParticle(int index);

private:

	ParticleRenderer* m_Renderer;

	std::vector<Particle*>* m_ActiveParticles;
	std::vector<Particle*>* m_InactiveParticles;

	unsigned int TOTAL_PARTICLES;

};