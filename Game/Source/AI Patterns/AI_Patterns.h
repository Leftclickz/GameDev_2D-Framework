#pragma once

typedef int (*AIPathfindingFunction) ();

class AI_Patterns
{
public:

	static int IdleMovement();

	static int MoveUp();
	static int MoveDown();
};