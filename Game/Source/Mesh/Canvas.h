#pragma once

class Canvas : public Mesh
{

public:
	Canvas(int object_count, Sprite* image);
	virtual ~Canvas() { delete m_Verts; }

	void AddVerts(const VertexFormat* verts, int count, TexturedTransform* transform);


	void Clear() { m_Verts->clear(); m_Verts->reserve(m_NumVerts); }
	void GenerateCanvas();
	void DrawCanvas();

protected:

	std::vector<VertexFormat>* m_Verts;
	Sprite* m_Atlas;
};