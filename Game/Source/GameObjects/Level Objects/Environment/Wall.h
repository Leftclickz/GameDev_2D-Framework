#pragma once
#include "../../AtlasObject.h"

class Wall : public AtlasObject
{
public:

	Wall() : AtlasObject(nullptr,nullptr,nullptr) {}
	Wall(GameCore* pGame, Mesh* pMesh, const char** pName);

	virtual void SetPosition(int index) override;

protected:

	virtual std::string GetDebugTag() override { return "Wall"; }

};