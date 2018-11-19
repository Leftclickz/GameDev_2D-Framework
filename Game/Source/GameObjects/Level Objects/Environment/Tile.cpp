#include "GamePCH.h"

Tile::Tile() : AtlasObject(nullptr, nullptr, "empty")
{
}

Tile::Tile(GameCore* pGame, Mesh* pMesh, const char* pName) : AtlasObject(pGame, pMesh, pName)
{
	UseSprite("Floor");
}



