#pragma once
#include "../../AnimatedObject.h"

class Timer;

class Enemy : public AnimatedObject
{

public:

	Enemy(GameCore* pGame, Mesh* pMesh, const char** pName);
	virtual void DoNextMove() {}

	virtual void TakeDamage(float damage) override;

protected:

	virtual std::string GetDebugTag() override { return "Enemy"; }

	bool AttemptMovement(int index);

	AIPathfindingFunction m_Pathfinding;
};