#include "GamePCH.h"

int AI_Patterns::IdleMovement()
{
	return 0;
}

int AI_Patterns::MoveUp()
{
	return LEVEL_TILE_DIMENSIONS.x;
}

int AI_Patterns::MoveDown()
{
	return -LEVEL_TILE_DIMENSIONS.x;
}
