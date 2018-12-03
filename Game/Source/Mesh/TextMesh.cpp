#include "GamePCH.h"

TextMesh::TextMesh() : Mesh()
{
	m_Text = "";
	m_PrimitiveType = GL_TRIANGLES;
	m_TextAtlas = ImageManager::UseImageAtlas(&TEXTURE_NAMES::FONT);
}

TextMesh::TextMesh(std::string text) : Mesh()
{

	m_DrawDebugLines = false;

	m_PrimitiveType = GL_TRIANGLES;
	m_TextAtlas = ImageManager::UseImageAtlas(&TEXTURE_NAMES::FONT);

	SetText(text);
}

void TextMesh::SetText(std::string text)
{
	m_Text = text;
	GenerateTextMesh();
}

void TextMesh::Draw(WorldTransform* transform)
{
	Mesh::Draw(transform, m_TextAtlas->atlas_image);
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

	m_Verts->reserve(m_NumVerts);

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

		//If we have a supported character we'll pull it up and use it for verts. If we have a space we'll use a default size (4,18)
		if (character != " ")
		{
			AtlasChild* sprite = m_TextAtlas->GetSprite(character.c_str());
			sprite_size = sprite->sprite_UV_Scale * m_TextAtlas->atlas_size;

			m_Verts->push_back(VertexFormat(position, (vec2(0.0f, 0.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + vec2(0.0f, sprite_size.y), (vec2(0.0f, 1.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + sprite_size, (vec2(1.0f, 1.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + sprite_size, (vec2(1.0f, 1.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + vec2(sprite_size.x, 0.0f), (vec2(1.0f, 0.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
			m_Verts->push_back(VertexFormat(position, (vec2(0.0f, 0.0f) * sprite->sprite_UV_Scale) + sprite->sprite_UV_Offset, TILE::WHITE));
		}
		else
		{
			sprite_size = vec2(4.0f, 18.0f);

			m_Verts->push_back(VertexFormat(position, vec2(0.0f, 0.0f), TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + vec2(0.0f, sprite_size.y), vec2(0.0f, 0.0f), TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + sprite_size, vec2(0.0f, 0.0f), TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + sprite_size, vec2(0.0f, 0.0f), TILE::WHITE));
			m_Verts->push_back(VertexFormat(position + vec2(sprite_size.x, 0.0f), vec2(0.0f, 0.0f), TILE::WHITE));
			m_Verts->push_back(VertexFormat(position, vec2(0.0f, 0.0f), TILE::WHITE));
		}

		position.x += sprite_size.x;
	}


	Generate();
	// Check for errors.
	CheckForGLErrors();
}
