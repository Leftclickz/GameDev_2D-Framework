#pragma once
#include "../../AnimatedObject.h"

class Enemy : public AnimatedObject
{

public:

	Enemy(GameCore* pGame, Mesh* pMesh, const char* pName);


protected:

	virtual std::string GetDebugTag() override { return "Enemy"; }
};