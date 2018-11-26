#include "GamePCH.h"
#include "../Game/Game.h"

Pathfinder* AI_Patterns::AStarPathfinder = nullptr;
Game* AI_Patterns::m_Game = nullptr;

AI_Patterns::AI_Patterns(Game* game)
{
	if (AStarPathfinder == nullptr)
		AStarPathfinder = new Pathfinder(nullptr);

	if (m_Game == nullptr)
		m_Game = game;
}

AI_Patterns::~AI_Patterns()
{
	delete AStarPathfinder;
}

void AI_Patterns::IdleMovement(int* index)
{
	return;
}

void AI_Patterns::MoveUp(int* index)
{
	*index += LEVEL_TILE_DIMENSIONS.x;
}

void AI_Patterns::MoveDown(int* index)
{
	*index -= LEVEL_TILE_DIMENSIONS.x;
}

void AI_Patterns::MoveLeft(int* index)
{
	*index -= 1;
}

void AI_Patterns::MoveRight(int* index)
{
	*index += 1;
}

void AI_Patterns::CardinalAStarPathFinding(int* index)
{
	int startX = *index % LEVEL_TILE_DIMENSIONS.x;
	int startY = *index / LEVEL_TILE_DIMENSIONS.x;

	int player_i = m_Game->GetPlayer()->GetPositionByIndex();

	int endX = player_i % LEVEL_TILE_DIMENSIONS.x;
	int endY = player_i / LEVEL_TILE_DIMENSIONS.x;

	bool found = AStarPathfinder->FindPath(startX, startY, endX, endY);

	//If there's no viable path we just don't move.
	if (found == false)
		return;
	else
	{
		int indices[255];
		int len = AStarPathfinder->GetPath(indices, 255, endX, endY);

		//Checking if length sf greater than 1 (if it isnt that means we're on top of our target).
		if (len > 1)
			*index = indices[len - 2];//we subtract 2 here because 1 for the length converted into index, and another 1 because the end of the array is our initial index
	}
}

