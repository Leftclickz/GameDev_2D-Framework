#pragma once
#include "../../AnimatedObject.h"


class Wall;

struct TileProperties
{
	//Standard transform positions
	float angle = 0.0f;
	vec2 object_scale = vec2(1.0f, 1.0f);
	vec2 object_anchor = vec2(0.0f, 0.0f);

	//Specific per tile type
	Mesh* tile_mesh;
	Sprite* atlas_image;
};

struct TileData
{
	TileData() : m_Wall(nullptr) {}
	TileData(vec2 object_position, AtlasChild** image) 
		: position(object_position), rendered_image(image), m_Wall(nullptr)  {}

	~TileData();

	vec2 position;
	AtlasChild** rendered_image;
	Wall* m_Wall;

	bool IsWalkable() { return (m_Wall == nullptr) ? true : false; }
};

class Tile : public AnimatedObject
{
public:

	Tile();
	Tile(GameCore* pGame, Mesh* pMesh, const char* pName);
	virtual ~Tile();

	virtual void Draw() override;

	void AddWall(Wall* aWall);

	virtual void SetPosition(int index) override;
	virtual bool IsWalkable() { return (m_Wall != nullptr) ? false : true; }

	virtual WorldTransform* GetTransform() { return &m_Transform; }

protected:

	virtual std::string GetDebugTag() override { return "Tile"; }

	Wall* m_Wall;

};