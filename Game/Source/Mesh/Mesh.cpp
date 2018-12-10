#include "GamePCH.h"

#include "Mesh/Mesh.h"
#include "ImageManager/ImageManager.h"
#include "Mesh.h"

Mesh::Mesh()
{
    m_VBO = 0;
	m_Verts = new std::vector<VertexFormat>;

    m_PrimitiveType = -1;
    m_NumVerts = 0;
}
    
Mesh::Mesh(const VertexFormat* mesh_data, int vert_count, GLuint primitive)
{
	m_VBO = 0;
	m_Verts = new std::vector<VertexFormat>;

	m_DrawDebugLines = false;

	m_NumVerts = vert_count;
	Generate(mesh_data, vert_count, primitive);
}

Mesh::~Mesh()
{
    glDeleteBuffers( 1, &m_VBO );
	delete m_Verts;
}

void Mesh::SetDebugColor(std::string tag_name)
{
	if (tag_name == "Default")
		m_DebugColor = &TILE::BLUE;
	else if (tag_name == "Tile")
		m_DebugColor = &TILE::WHITE;
	else if (tag_name == "Enemy")
		m_DebugColor = &TILE::RED;
	else if (tag_name == "Player")
		m_DebugColor = &TILE::GREEN;
	else if(tag_name == "Wall")
		m_DebugColor = &TILE::PINK;
	else if (tag_name == "Text")
		m_DebugColor = &TILE::CYAN;
}

void SetUniform1f(GLuint shader, const char* uniformName, float value)
{
    GLint loc = glGetUniformLocation( shader, uniformName );
    if( loc != -1 )
    {
        glUniform1f( loc, value );
    }
}

void SetUniform2f(GLuint shader, const char* uniformName, vec2 value)
{
    GLint loc = glGetUniformLocation( shader, uniformName );
    if( loc != -1 )
    {
        glUniform2f( loc, value.x, value.y );
    }
}


//Internal draw call for textures.
void Mesh::DrawBuffer(WorldTransform* transform, GLuint shader)
{
    // Bind buffer and set up attributes.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

	//position attribute
    GLint loc = glGetAttribLocation(shader, "a_Position" );
    if( loc != -1 )
    {
        glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos) );
        glEnableVertexAttribArray( loc );
    }

	//Color attribute
    loc = glGetAttribLocation(shader, "a_Color" );
    if( loc != -1 )
    {
        glVertexAttribPointer( loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Color) );
        glEnableVertexAttribArray( loc );
    }

	if (shader != DEBUG_SHADER)
	{
		//UV attribute
		loc = glGetAttribLocation(shader, "a_UVCoord");
		if (loc != -1)
		{
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UVCoord));
			glEnableVertexAttribArray(loc);
		}
	}
	else
	{
		//Debug color uniform
		loc = glGetUniformLocation(shader, "u_Color");
		glUniform4f(loc, m_DebugColor->r, m_DebugColor->g, m_DebugColor->b, m_DebugColor->a);
	}

    //Transform uniforms
	if (transform != nullptr)
	{
		SetUniform2f(shader, "u_ObjectScale", transform->object_scale);
		SetUniform1f(shader, "u_ObjectAngleRadians", transform->angle / 180.0f * PI);
		SetUniform2f(shader, "u_ObjectPosition", transform->object_position);
		SetUniform2f(shader, "u_ObjectAnchor", transform->object_anchor);
	}
	else
	{
		SetUniform2f(shader, "u_ObjectScale", vec2(1.0f,1.0f));
		SetUniform1f(shader, "u_ObjectAngleRadians", 0.0f / 180.0f * PI);
		SetUniform2f(shader, "u_ObjectPosition", vec2(0.0f,0.0f));
		SetUniform2f(shader, "u_ObjectAnchor", vec2(0.0f,0.0f));
	}

	//Camera uniforms
    SetUniform2f(shader, "u_CameraTranslation", PLAYER_CAMERA_POSITION * -1 );
    SetUniform2f(shader, "u_ProjectionScale", PLAYER_CAMERA_PROJECTION );

	GLenum primitive;
	//Get primitive type

	if (shader == TEXTURE_SHADER)
		primitive = m_PrimitiveType;
	else if (shader == DEBUG_SHADER)
		primitive = DEBUG_RENDER_MODE;
	else if (shader == CANVAS_SHADER)
		primitive = CANVAS_RENDER_MODE;
	else
		primitive = GL_TRIANGLES;

	//Draw
    glDrawArrays(primitive, 0, m_NumVerts );

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
    CheckForGLErrors();
}

void Mesh::Draw(WorldTransform* transform, Sprite* texture, AtlasChild *sprite_data /*= nullptr*/)
{
	GLuint shader;

	{
		//Set texture shader active
		shader = TEXTURE_SHADER;
		glUseProgram(shader);

		//bind texture
		glActiveTexture(GL_TEXTURE0 + texture->TU_index);
		glBindTexture(GL_TEXTURE_2D, texture->GL_texture_index);

		//Pass TU to the fragment shader for sampling
		GLint textureLoc = glGetUniformLocation(shader, "u_Tex");
		if (textureLoc != -1)
			glUniform1i(textureLoc, texture->TU_index);

		//Pass UV values to sample the correct position
		if (sprite_data != nullptr)
		{
			SetUniform2f(shader, "u_UVOffset", sprite_data->sprite_UV_Offset);
			SetUniform2f(shader, "u_UVScale", sprite_data->sprite_UV_Scale);
		}
		else
		{
			SetUniform2f(shader, "u_UVOffset", vec2(0.0f, 0.0f));
			SetUniform2f(shader, "u_UVScale", vec2(1.0f, 1.0f));
		}

		//Draw buffers.
		DrawBuffer(transform, shader);
	}
	//Draw call for debug
	if (m_DrawDebugLines)
	{
		//Set debugging shader active.
		shader = DEBUG_SHADER;
		glUseProgram(shader);

		//Draw buffers.
		DrawBuffer(transform, shader);
	}
}

void Mesh::Draw(TexturedTransform* transform)
{
	Draw(transform->world_transform, transform->atlas_image, *transform->rendered_image);
}

//Generate a fresh mesh from vertex data
void Mesh::Generate(const VertexFormat* data, int vertcount, GLuint primitive)
{
	m_NumVerts = vertcount;
	m_Verts->reserve(vertcount);

	m_PrimitiveType = primitive;

	for (int i = 0; i < vertcount; i++)
		m_Verts->push_back(data[i]);

	Generate();
}

//Generate a fresh mesh from vertex data
void Mesh::Generate()
{
	
	// Recreate the VBO if its been assigned already
	if (m_VBO != 0)
		glDeleteBuffers(1, &m_VBO);

	// Gen and fill buffer with our attributes.
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVerts, &m_Verts->front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Check for errors.
	CheckForGLErrors();
}

//Generate a mesh from Atlas data.
void Mesh::GenerateMeshFromAtlas(vec2 sprite_size)
{
	// ATM this can only be called once, so we assert if it's called again with a VBO already allocated.
	assert(m_VBO == 0);

	m_NumVerts = 4;
	m_PrimitiveType = GL_TRIANGLE_FAN;

	VertexFormat data[] = {
		VertexFormat(vec2(0.0f,0.0f), vec2(0.f,0.0f), MyColor()),
		VertexFormat(vec2(sprite_size.x,0.0f), vec2(1.0f,0.0f), MyColor()),
		VertexFormat(vec2(sprite_size.x,sprite_size.y), vec2(1.0f,1.0f), MyColor()),
		VertexFormat(vec2(0.0f,sprite_size.y), vec2(0.0f,1.0f), MyColor())
	};

	m_Verts->reserve(m_NumVerts);

	for (unsigned int i = 0; i < m_NumVerts; i++)
		m_Verts->push_back(data[i]);

	Generate();
}

vec2 Mesh::GetSize()
{
	vec2 size(0.0f, 0.0f);

	for (unsigned int i = 0; i < m_Verts->size(); i++)
	{
		size.x = m_Verts->at(i).m_Pos.x > size.x ? m_Verts->at(i).m_Pos.x : size.x;
		size.y = m_Verts->at(i).m_Pos.y > size.y ? m_Verts->at(i).m_Pos.y : size.y;
	}

	return size;
}
