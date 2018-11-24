#pragma once
#include "Enemy.h"

class Slime : public Enemy
{
public:

	Slime(GameCore* pGame, Mesh* pMesh);

	virtual void DoNextMove() override;

protected:

	int m_LastIndexMoved;
};