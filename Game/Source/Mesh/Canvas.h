#pragma once

struct TileData;
struct TileProperties;

class Canvas : public Mesh
{

public:
	Canvas(int object_count, Sprite* image);
	virtual ~Canvas() {}

	//Add verts using predefined VertexFormat arrays and a texture transform
	void AddVerts(const VertexFormat* verts, int count, TexturedTransform* transform);

	//Converts world transform and image data into a texture transform and adds those verts to the mesh
	void AddVerts(const VertexFormat* verts, int count, WorldTransform world_transform, Sprite* atlas, AtlasChild** child);

	//Add verts using tile properties and tile data.
	void AddVerts(TileData* data, TileProperties* properties);

	//Clear the allocation of the verts.
	void Clear() { m_Verts->clear(); m_Verts->reserve(m_NumVerts); }

	//Generate a VBO of our Mesh.
	void GenerateCanvas();

	//Effectively an overload of Mesh::Draw but this one takes in no parameters.
	void DrawCanvas();

protected:

	//Internal vertex combination of 1 vector to another.
	void AddVertsByVector(std::vector<VertexFormat>* verts, TexturedTransform* transform);

	Sprite* m_Atlas;
};