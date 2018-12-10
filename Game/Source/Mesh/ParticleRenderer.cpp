#include "GamePCH.h"
#include "ParticleRenderer.h"
#include <string>

ParticleRenderer::ParticleRenderer()
{
	m_Particles = new std::vector<Particle>;
	m_Particles->reserve(100);

	m_Sprites = new std::vector<Sprite*>;
	m_Sprites->reserve(5);
}

ParticleRenderer::ParticleRenderer(const VertexFormat * mesh_data, int vert_count, GLuint primitive) : Mesh(mesh_data,vert_count,primitive)
{
	m_Particles = new std::vector<Particle>;
	m_Particles->reserve(100);

	m_Sprites = new std::vector<Sprite*>;
	m_Sprites->reserve(5);
}

ParticleRenderer::~ParticleRenderer()
{
	delete m_Particles;
	delete m_Sprites;
}

void ParticleRenderer::Update(float deltatime)
{
}

void ParticleRenderer::Draw()
{

	//if there's no particles to draw this frame just skip the sequence entirely
	if (m_Particles->size() == 0)
		return;

	//bind and allocate our texture units 
	BindTextureUnits();

	//set up shader
	GLuint shader = PARTICLE_SHADER;
	glUseProgram(shader);

	// Bind buffer and set up attributes.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//position attribute
	GLint loc = glGetAttribLocation(shader, "a_Position");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos));
		glEnableVertexAttribArray(loc);
	}

	//UV attribute
	loc = glGetAttribLocation(shader, "a_UVCoord");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UVCoord));
		glEnableVertexAttribArray(loc);
	}

	
	//pass all possible texture units to the fragment shader sampler
	{
		std::vector<int> texture_units;

		for (unsigned int i = 0; i < m_Sprites->size(); i++)
			texture_units.push_back(m_Sprites->at(i)->TU_index);

		GLint textureLoc = glGetUniformLocation(shader, "u_Tex");
		if (textureLoc != -1)
			glUniform1iv(textureLoc, texture_units.size(), &texture_units.front());
	}

	//pass all uniform values for each particle to the vertex shader
	{
		std::vector<vec2> uv_offsets, uv_scales, object_scale, object_position, object_anchor;
		std::vector<float> object_angle;
		std::vector<int> texture_units;

		//load vectors with data to pass to the shader
		for (unsigned int i = 0; i < m_Particles->size(); i++)
		{
			uv_offsets.push_back((*m_Particles->at(i).image)->sprite_UV_Offset);
			uv_scales.push_back((*m_Particles->at(i).image)->sprite_UV_Scale);
			object_scale.push_back(m_Particles->at(i).transform.object_scale);
			object_angle.push_back(m_Particles->at(i).transform.angle / 180.0f * PI);
			object_position.push_back(m_Particles->at(i).transform.object_position);
			object_anchor.push_back(m_Particles->at(i).transform.object_anchor);
			texture_units.push_back(m_Particles->at(i).texture_unit);
		}

		//pass all the vector data to the shader
		glUniform2fv(glGetUniformLocation(shader, "u_UVOffset"), uv_offsets.size(), &uv_offsets[0].x);
		glUniform2fv(glGetUniformLocation(shader, "u_UVScale"), uv_scales.size(), &uv_scales[0].x);
		glUniform2fv(glGetUniformLocation(shader, "u_ObjectScale"), object_scale.size(), &object_scale[0].x);
		glUniform1fv(glGetUniformLocation(shader, "u_ObjectAngleRadians"), object_angle.size(), &object_angle.front());
		glUniform2fv(glGetUniformLocation(shader, "u_ObjectPosition"), object_position.size(), &object_position[0].x);
		glUniform2fv(glGetUniformLocation(shader, "u_ObjectAnchor"), object_anchor.size(), &object_anchor[0].x);
		glUniform1iv(glGetUniformLocation(shader, "u_TU"), texture_units.size(), &texture_units.front());
	}

	//Camera uniforms
	SetUniform2f(shader, "u_CameraTranslation", PLAYER_CAMERA_POSITION * -1);
	SetUniform2f(shader, "u_ProjectionScale", PLAYER_CAMERA_PROJECTION);

	//instance draw all particles
	glDrawArraysInstanced(PARTICLE_RENDER_MODE, 0, 4, m_Particles->size());

	//unbind the texture units
	UnbindTextureUnits();

	//clear the particle vector
	m_Particles->clear();
}

void ParticleRenderer::AddParticle(Particle * particle)
{
	m_Particles->push_back(*particle);
}

void ParticleRenderer::BindTextureUnits()
{
	for (unsigned int i = 0; i < m_Sprites->size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + m_Sprites->at(i)->TU_index);
		glBindTexture(GL_TEXTURE_2D, m_Sprites->at(i)->GL_texture_index);
	}
}

void ParticleRenderer::UnbindTextureUnits()
{
	for (unsigned int i = 0; i < m_Sprites->size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + m_Sprites->at(i)->TU_index);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
