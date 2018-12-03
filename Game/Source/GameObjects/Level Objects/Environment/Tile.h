#pragma once
#include "../../AnimatedObject.h"


class Wall;

enum TileImageVariant
{
	VARIANT_ONE = 1,
	VARIANT_TWO = 2
};

// Data structure for a tile type. Contains basic transforms (except a position) and a mesh.
struct TileProperties
{
	//Standard transform positions
	float angle = 0.0f;
	vec2 object_scale = vec2(1.0f, 1.0f);
	vec2 object_anchor = vec2(0.0f, 0.0f);

	//Specific per tile type
	Mesh* tile_mesh;
};

// Data structure for individual tiles. Must contain an index (position), a variant (will default to variant 1), and properties (info shared between similar tiles)
struct TileData
{
	TileData() : m_Wall(nullptr) {}
	TileData(TileProperties* tile_properties, int object_index, TileImageVariant tile_variant)
		: properties(tile_properties), index(object_index), variant(tile_variant), m_Wall(nullptr)  {}

	~TileData();

	//tile position
	int index;

	//tile variant
	TileImageVariant variant;

	//tile properties
	TileProperties* properties;

	//does the tile have a wall? (this may be temporary but the idea is any tile can contain a wall for now)
	Wall* m_Wall;

	bool IsWalkable() { return (m_Wall == nullptr) ? true : false; }
};

//class Tile : public AnimatedObject
//{
//public:
//
//	Tile();
//	Tile(GameCore* pGame, Mesh* pMesh, const char* pName);
//	virtual ~Tile();
//
//	virtual void Draw() override;
//
//	void AddWall(Wall* aWall);
//
//	virtual void SetPosition(int index) override;
//	virtual bool IsWalkable() { return (m_Wall != nullptr) ? false : true; }
//
//	virtual WorldTransform* GetTransform() { return &m_Transform; }
//
//protected:
//
//	virtual std::string GetDebugTag() override { return "Tile"; }
//
//	Wall* m_Wall;
//
//};