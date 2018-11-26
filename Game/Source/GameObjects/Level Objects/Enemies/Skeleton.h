#pragma once
#include "Enemy.h"

class Skeleton : public Enemy
{
public:

	Skeleton(GameCore* pGame, Mesh* pMesh);

	virtual void DoNextMove() override;

protected:

	int m_LastIndexMoved;
};