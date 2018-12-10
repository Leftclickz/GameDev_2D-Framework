#pragma once
#include "Mesh.h"

struct AtlasChild;

struct Particle
{
	WorldTransform transform;
	AtlasChild** image;
	int texture_unit;
	float time_limit;
	float duration = 0.0f;
};

class ParticleRenderer : public Mesh
{

public:

	ParticleRenderer();
	ParticleRenderer(const VertexFormat* mesh_data, int vert_count, GLuint primitive = 0);
	virtual ~ParticleRenderer();

	virtual void Update(float deltatime);
	virtual void Draw();

	virtual void AddParticle(Particle* particle);

	virtual void AddSprite(Sprite* sprite) { m_Sprites->push_back(sprite); }

	virtual void BindTextureUnits();
	virtual void UnbindTextureUnits();

protected:

	std::vector<Particle>* m_Particles;
	std::vector<Sprite*>* m_Sprites;
};