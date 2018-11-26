#include "GamePCH.h"

TextMesh::TextMesh() : Mesh()
{
	m_Text = "";
	m_PrimitiveType = GL_TRIANGLES;
	m_TextAtlas = ImageManager::UseImageAtlas("Text/DefaultFont_White");
}

TextMesh::TextMesh(std::string text, ShaderProgram* TexShader, ShaderProgram* DebugShader) : Mesh()
{
	m_Text = "";
	m_PrimitiveType = GL_TRIANGLES;
	m_TextAtlas = ImageManager::UseImageAtlas("Text/DefaultFont_White");

	m_TextureShader = TexShader;
	m_DebugShader = DebugShader;

	SetText(text);
}

void TextMesh::SetText(std::string text)
{
	m_Text = text;
	GenerateTextMesh();
}

void TextMesh::Draw(WorldTransform* transform)
{
	GLuint shader = SetActiveShader(m_TextureShader);

	//Draw call for texture
	{
		glActiveTexture(GL_TEXTURE0 + m_TextAtlas->atlas_image->TU_index);
		glBindTexture(GL_TEXTURE_2D, m_TextAtlas->atlas_image->GL_texture_index);

		GLint textureLoc = glGetUniformLocation(m_TextureShader->GetProgram(), "u_Tex");
		if (textureLoc != -1)
			glUniform1i(textureLoc, m_TextAtlas->atlas_image->TU_index);

		SetUniform2f(shader, "u_UVOffset", vec2(0.0f,0.0f));
		SetUniform2f(shader, "u_UVScale", vec2(1.0f,1.0f));

		DrawTexture(transform, shader);
	}
	if (m_DrawDebugLines)
	{
		shader = SetActiveShader(m_DebugShader);
		DebugDraw(transform, shader);
	}
}

void TextMesh::GenerateTextMesh()
{
	//Get the length of our string
	int length = m_Text.length();

	//If the string is empty just return out.
	if (length == 0)
		return;

	//Convert the length into verts
	m_NumVerts = length * 6;

	std::vector<VertexFormat> vertices;
	vertices.reserve(m_NumVerts);

	vec2 position(0.0f, 0.0f);

	for (int i = 0; i < length; i++)
	{
		std::string character(1,m_Text[i]);

		//fucking files cant be saved using certain symbols... so here we are.
		if (character == "/")
			character = "_Backslash";
		else if (character == "?")
			character = "_Question";
		else if (character == ":")
			character = "_Colon";
		else if (character == "(")
			character = "_OpeningBrace";
		else if (character == ")")
			character = "_ClosingBrace";
		else if (character == "{")
			character = "_LessThan";
		else if (character == "}")
			character = "_GreaterThan";
		else if (character == ".")
			character = "_Period";
		else if (character == ",")
			character = "_Comma";
		else if (character == "-")
			character = "_Minus";
		else if (character == "=")
			character = "_Equals";
		else if (character == "+")
			character = "_Plus";
		else if (character == "'")
			character = "_SingleQuote";
		else if (character == "\"")
			character = "_DoubleQuote";
		else if (character == "!")
			character = "_Exclamation";

		vec2 sprite_size;

		if (character != " ")
		{
			AtlasChild* sprite = m_TextAtlas->GetSprite(character.c_str());
			sprite_size = sprite->sprite_UV_Scale * m_TextAtlas->atlas_size;

			vertices.push_back(VertexFormat(position, (vec2(0.0f, 0.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			vertices.push_back(VertexFormat(position + vec2(0.0f, sprite_size.y), (vec2(0.0f, 1.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			vertices.push_back(VertexFormat(position + sprite_size, (vec2(1.0f, 1.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			vertices.push_back(VertexFormat(position + sprite_size, (vec2(1.0f, 1.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			vertices.push_back(VertexFormat(position + vec2(sprite_size.x, 0.0f), (vec2(1.0f, 0.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			vertices.push_back(VertexFormat(position, (vec2(0.0f, 0.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
		}
		else
		{
			sprite_size = vec2(4.0f, 18.0f);

			vertices.push_back(VertexFormat(position, vec2(0.0f, 0.0f), TILE::WHITE));
			vertices.push_back(VertexFormat(position + vec2(0.0f, sprite_size.y), vec2(0.0f, 0.0f), TILE::WHITE));
			vertices.push_back(VertexFormat(position + sprite_size, vec2(0.0f, 0.0f), TILE::WHITE));
			vertices.push_back(VertexFormat(position + sprite_size, vec2(0.0f, 0.0f), TILE::WHITE));
			vertices.push_back(VertexFormat(position + vec2(sprite_size.x, 0.0f), vec2(0.0f, 0.0f), TILE::WHITE));
			vertices.push_back(VertexFormat(position, vec2(0.0f, 0.0f), TILE::WHITE));
		}

		position.x += sprite_size.x;
	}

	// ATM this can only be called once, so we assert if it's called again with a VBO already allocated.
	assert(m_VBO == 0);

	// Gen and fill buffer with our attributes.
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVerts,  &vertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Check for errors.
	CheckForGLErrors();
}
