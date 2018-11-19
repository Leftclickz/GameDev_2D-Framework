#include "GamePCH.h"

#include "Mesh/Mesh.h"
#include "ImageManager/ImageManager.h"

Mesh::Mesh()
{
    m_VBO = 0;
    m_TextureShader = 0;

    m_PrimitiveType = -1;
    m_NumVerts = 0;
}
    
Mesh::Mesh(const VertexFormat* mesh_data, int vert_count, ShaderProgram* shader, ShaderProgram* debug, GLuint primitive)
{
	m_VBO = 0;
	m_DrawDebugLines = false;
	m_PrimitiveType = primitive;

	m_TextureShader = shader;
	m_DebugShader = debug;

	m_NumVerts = vert_count;
	Generate(mesh_data);
}

Mesh::~Mesh()
{
    glDeleteBuffers( 1, &m_VBO );
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

//Internal swapping of shader programs
GLuint Mesh::SetActiveShader(ShaderProgram* shader)
{
	GLuint shadeObj = shader->GetProgram();
	glUseProgram(shadeObj);
	return shadeObj;
}

//Internal draw call for textures.
void Mesh::DrawTexture(WorldTransform* transform, GLuint shader)
{
    // Bind buffer and set up attributes.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    GLint loc = glGetAttribLocation( shader, "a_Position" );
    if( loc != -1 )
    {
        glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos) );
        glEnableVertexAttribArray( loc );
    }

	loc = glGetAttribLocation(shader, "a_UVCoord");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UVCoord));
		glEnableVertexAttribArray(loc);
	}

    loc = glGetAttribLocation(shader, "a_Color" );
    if( loc != -1 )
    {
        glVertexAttribPointer( loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Color) );
        glEnableVertexAttribArray( loc );
    }

    // Set up uniforms.
    SetUniform2f( shader, "u_ObjectScale", transform->object_scale );
    SetUniform1f( shader, "u_ObjectAngleRadians", transform->angle / 180.0f * PI );
    SetUniform2f( shader, "u_ObjectPosition", transform->object_position );
    SetUniform2f( shader, "u_CameraTranslation", transform->cam_pos * -1 );
    SetUniform2f( shader, "u_ProjectionScale", transform->proj_scale );

    // Draw.
    glDrawArrays( m_PrimitiveType, 0, m_NumVerts );

	glBindTexture(GL_TEXTURE_2D, 0);
    CheckForGLErrors();
}

//Internal draw call for debug lines
void Mesh::DebugDraw(WorldTransform* transform, GLuint shader)
{
	GLint loc = glGetAttribLocation(shader, "a_Position");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos));
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(shader, "a_Color");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Color));
		glEnableVertexAttribArray(loc);
	}

	// Set up uniforms.
	SetUniform2f(shader, "u_ObjectScale", transform->object_scale);
	SetUniform1f(shader, "u_ObjectAngleRadians", transform->angle / 180.0f * PI);
	SetUniform2f(shader, "u_ObjectPosition", transform->object_position);
	SetUniform2f(shader, "u_CameraTranslation", transform->cam_pos * -1);
	SetUniform2f(shader, "u_ProjectionScale", transform->proj_scale);

	//Debug color uniform
	loc = glGetUniformLocation(shader, "u_Color");
	glUniform4f(loc, m_DebugColor->r, m_DebugColor->g, m_DebugColor->b, m_DebugColor->a);

	glDrawArrays(GL_LINE_LOOP, 0, m_NumVerts);

	CheckForGLErrors();
}

void Mesh::Draw(WorldTransform* transform, Sprite *texture)
{
	GLuint shader = SetActiveShader(m_TextureShader);
	//Draw call for texture
	{
		glActiveTexture(GL_TEXTURE0 + texture->TU_index);
		glBindTexture(GL_TEXTURE_2D, texture->GL_texture_index);

		GLint textureLoc = glGetUniformLocation(m_TextureShader->GetProgram(), "u_Tex");
		if (textureLoc != -1)
			glUniform1i(textureLoc, texture->TU_index);

		SetUniform2f(shader, "u_UVOffset", vec2(0.0f, 0.0f));
		SetUniform2f(shader, "u_UVScale", vec2(1.0f, 1.0f));

		DrawTexture(transform, shader);
	}
	//Draw call for debug
	if (m_DrawDebugLines)
	{
		shader = SetActiveShader(m_DebugShader);
		DebugDraw(transform, shader);
	}
}

void Mesh::Draw(WorldTransform* transfrom, Sprite* texture, AtlasChild *sprite_data)
{
	GLuint shader = SetActiveShader(m_TextureShader);
	//Draw call for texture
	{
		glActiveTexture(GL_TEXTURE0 + texture->TU_index);
		glBindTexture(GL_TEXTURE_2D, texture->GL_texture_index);

		GLint textureLoc = glGetUniformLocation(m_TextureShader->GetProgram(), "u_Tex");
		if (textureLoc != -1)
			glUniform1i(textureLoc, texture->TU_index);

		SetUniform2f(shader, "u_UVOffset", sprite_data->sprite_UV_Offset);
		SetUniform2f(shader, "u_UVScale", sprite_data->sprite_UV_Scale);

		DrawTexture(transfrom, shader);
	}
	//Draw call for debug
	if (m_DrawDebugLines)
	{
		shader = SetActiveShader(m_DebugShader);
		DebugDraw(transfrom, shader);
	}
}

//Generate a fresh mesh from vertex data
void Mesh::Generate(const VertexFormat* data, int vertcount, GLuint primitive)
{
	m_NumVerts = vertcount;
	m_PrimitiveType = primitive;

	Generate(data);
}

//Generate a fresh mesh from vertex data
void Mesh::Generate(const VertexFormat* data)
{
	
	// ATM this can only be called once, so we assert if it's called again with a VBO already allocated.
	assert(m_VBO == 0);

	// Gen and fill buffer with our attributes.
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVerts, data, GL_STATIC_DRAW);
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

	// Gen and fill buffer with our attributes.
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVerts, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
