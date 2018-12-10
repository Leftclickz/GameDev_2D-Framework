#pragma once
#include "../../AnimatedObject.h"

class Timer;

enum MOVEMENT_CODES
{
	MOVED_SUCCESSFULLY,
	BLOCKED_BY_WALL,
	COLLIDED_WITH_SOMETHING
};

class Enemy : public AnimatedObject
{

public:

	Enemy(GameCore* pGame, Mesh* pMesh, const char** pName);
	virtual void DoNextMove() {}

	virtual void TakeDamage(float damage) override;
	virtual float GetScore() { return m_Score; }

	//is this game object currently active
	void SetIsActive(bool value = false) { b_IsActive = value; }
	bool IsActive() { return b_IsActive; }

	virtual void Reset();

protected:

	virtual std::string GetDebugTag() override { return "Enemy"; }

	MOVEMENT_CODES AttemptMovement(int index);

	AIPathfindingFunction m_Pathfinding;

	float m_Score;
	bool b_IsActive;
};