#pragma once

#include "AStarPathFinding.h"
class Level;
class Game;

typedef void (*AIPathfindingFunction) (int* index);

//static class for handling AI movements.
class AI_Patterns
{
public:
	//constructor for AI patterns. Only make one of these ever.
	AI_Patterns(Game* game);

	//destructor
	~AI_Patterns();

	//Set the pathfinder with level info.
	void SetLevel(Level* level) { AStarPathfinder->SetLevel(level); }

	//Idle movement
	static void IdleMovement(int* index);

	//Cardinal direction movement
	static void MoveUp(int* index);
	static void MoveDown(int* index);
	static void MoveLeft(int* index);
	static void MoveRight(int* index);

	static void CardinalAStarPathFinding(int* index);

	static Pathfinder* AStarPathfinder;

protected:

	static Game* m_Game;
};