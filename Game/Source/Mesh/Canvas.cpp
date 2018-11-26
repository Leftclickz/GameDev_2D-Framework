#include "GamePCH.h"

Canvas::Canvas(int object_count, Sprite* image)
{

	m_Verts = new std::vector<VertexFormat>;
	m_Atlas = image;
	m_NumVerts = object_count * 6;

	//Reserve room for each Tile (they have 4 verts each)
	m_Verts->reserve(m_NumVerts);
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

void Canvas::GenerateCanvas()
{
	// Gen and fill buffer with our attributes.
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVerts, &m_Verts->front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Check for errors.
	CheckForGLErrors();
}

void Canvas::DrawCanvas()
{
	//assign texture shader
	GLuint shader = CANVAS_SHADER;
	glUseProgram(shader);

	//Set active texture to our atlas
	glActiveTexture(GL_TEXTURE0 + m_Atlas->TU_index);
	glBindTexture(GL_TEXTURE_2D, m_Atlas->GL_texture_index);

	// Bind buffer and set up attributes.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	GLint loc = glGetAttribLocation(shader, "a_Position");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos));
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(shader, "a_UVCoord");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_UVCoord));
		glEnableVertexAttribArray(loc);
	}

	loc = glGetAttribLocation(shader, "a_Color");
	if (loc != -1)
	{
		glVertexAttribPointer(loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Color));
		glEnableVertexAttribArray(loc);
	}

	//Only thing we're uniforming now is the camera and TU.
	SetUniform2f(shader, "u_ProjectionScale", PLAYER_CAMERA_PROJECTION);
	SetUniform2f(shader, "u_CameraTranslation", PLAYER_CAMERA_POSITION * - 1);

	GLint textureLoc = glGetUniformLocation(shader, "u_Tex");
	if (textureLoc != -1)
		glUniform1i(textureLoc, m_Atlas->TU_index);

	//draw the canvas
	glDrawArrays(GL_TRIANGLES, 0, m_NumVerts);

	//Unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	CheckForGLErrors();
}
