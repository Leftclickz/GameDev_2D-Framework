#pragma once
#include "../../AtlasObject.h"

class Tile : public AtlasObject
{
public:

	Tile();
	Tile(GameCore* pGame, Mesh* pMesh, const char* pName);
	virtual ~Tile() {}

protected:


};