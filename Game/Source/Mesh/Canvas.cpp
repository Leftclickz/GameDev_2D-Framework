#include "GamePCH.h"

Canvas::Canvas(int object_count, Sprite* image) : Mesh()
{
	m_VBO = 0;
	m_Atlas = image;
	m_NumVerts = object_count * 6;

	//Reserve room for each Tile (they have 4 verts each)
	m_Verts->reserve(m_NumVerts);

	m_DebugColor = &TILE::WHITE;
}

void Canvas::AddVerts(const VertexFormat* verts, int count, TexturedTransform* texTransform)
{

	//default vertex
	VertexFormat new_vert(vec2(0.0f,0.0f),vec2(0.0f,0.0f),TILE::WHITE);
	VertexFormat last_vert(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), TILE::WHITE);

	//Loop through each vert in the file. apply transforms to prepare to add to attributes.
	for (int i = 0; i < count; i++)
	{
		new_vert = verts[i];

		//scaling
		new_vert.m_Pos *= texTransform->world_transform->object_scale;

		//rotation
		new_vert.m_Pos = vec2(new_vert.m_Pos.x*cos(texTransform->world_transform->angle) - new_vert.m_Pos.y*sin(texTransform->world_transform->angle),
			new_vert.m_Pos.x*sin(texTransform->world_transform->angle) + new_vert.m_Pos.y*cos(texTransform->world_transform->angle));

		//translation
		new_vert.m_Pos += texTransform->world_transform->object_position + texTransform->world_transform->object_anchor;
		
		//uv scaling
		new_vert.m_UVCoord *= (*texTransform->rendered_image)->sprite_UV_Scale;

		//uv offset
		new_vert.m_UVCoord += (*texTransform->rendered_image)->sprite_UV_Offset;

		//push back the converted vertex
		m_Verts->push_back(new_vert);

		//Were converting triangle strip into triangles. So we need to add 2 extra verts for every square.
		//We duplicate the 3rd vert, and save the 2nd to be copied over at the end.
		if (i == 2)
			m_Verts->push_back(new_vert);
		else if (i == 1)
			last_vert = new_vert;
	}

	m_Verts->push_back(last_vert);

}

void Canvas::AddVertsByVector(std::vector<VertexFormat>* verts, TexturedTransform* texTransform)
{

	//default vertex
	VertexFormat new_vert(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), TILE::WHITE);
	VertexFormat last_vert(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), TILE::WHITE);

	//Loop through each vert in the file. apply transforms to prepare to add to attributes.
	for (unsigned int i = 0; i < verts->size(); i++)
	{
		new_vert = verts->at(i);

		//scaling
		new_vert.m_Pos *= texTransform->world_transform->object_scale;

		//rotation
		new_vert.m_Pos = vec2(new_vert.m_Pos.x*cos(texTransform->world_transform->angle) - new_vert.m_Pos.y*sin(texTransform->world_transform->angle),
			new_vert.m_Pos.x*sin(texTransform->world_transform->angle) + new_vert.m_Pos.y*cos(texTransform->world_transform->angle));

		//translation
		new_vert.m_Pos += texTransform->world_transform->object_position + texTransform->world_transform->object_anchor;

		//uv scaling
		new_vert.m_UVCoord *= (*texTransform->rendered_image)->sprite_UV_Scale;

		//uv offset
		new_vert.m_UVCoord += (*texTransform->rendered_image)->sprite_UV_Offset;

		//push back the converted vertex
		m_Verts->push_back(new_vert);

		//Were converting triangle strip into triangles. So we need to add 2 extra verts for every square.
		//We duplicate the 3rd vert, and save the 2nd to be copied over at the end.
		if (i == 2)
			m_Verts->push_back(new_vert);
		else if (i == 1)
			last_vert = new_vert;
	}

	//Add last vert.
	m_Verts->push_back(last_vert);
}


void Canvas::AddVerts(const VertexFormat* verts, int count, WorldTransform world_transform, Sprite* atlas, AtlasChild** child)
{
	//Convert world transform into texture transform
	TexturedTransform transform = TexturedTransform(&world_transform, atlas, child);

	//Add verts
	AddVerts(verts, count, &transform);
}

void Canvas::AddVerts(TileData* data, TileProperties* properties)
{
	//Converting the TileData and TileProperties into VertexFormat and TexturedTransform
	TexturedTransform transform;
	WorldTransform world_transform;

	//Assign world transformations
	world_transform.angle = properties->angle;
	world_transform.object_anchor = properties->object_anchor;
	world_transform.object_position = data->position;
	world_transform.object_scale = properties->object_scale;

	//Asign texture transformation
	transform.atlas_image = properties->atlas_image;
	transform.rendered_image = data->rendered_image;
	transform.world_transform = &world_transform;

	//Add verts
	AddVertsByVector(properties->tile_mesh->GetVerts(), &transform);
}

void Canvas::GenerateCanvas()
{
	Mesh::Generate();
}

void Canvas::DrawCanvas()
{
	GLuint shader;
	{
		//Set canvas shader active
		shader = CANVAS_SHADER;
		glUseProgram(shader);

		//bind texture
		glActiveTexture(GL_TEXTURE0 + m_Atlas->TU_index);
		glBindTexture(GL_TEXTURE_2D, m_Atlas->GL_texture_index);

		//Pass TU to the fragment shader for sampling
		GLint textureLoc = glGetUniformLocation(shader, "u_Tex");
		if (textureLoc != -1)
			glUniform1i(textureLoc, m_Atlas->TU_index);

		//Draw buffer
		DrawBuffer(nullptr, shader);
	}
	if (m_DrawDebugLines == true)
	{
		//Set debug shader active
		shader = DEBUG_SHADER;
		glUseProgram(shader);



		//Draw buffer
		DrawBuffer(nullptr, shader);
	}
}