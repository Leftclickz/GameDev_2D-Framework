#include "GamePCH.h"
#include "../../../Game/Game.h"

Enemy::Enemy(GameCore* pGame, Mesh* pMesh, const char* pName) : AnimatedObject(pGame,pMesh,pName)
{
	SetAnchor(vec2(0.0f, TILE_SIZE.y * 0.5f));

	m_pGame = pGame;

	m_Pathfinding = AI_Patterns::IdleMovement;
}


bool Enemy::AttemptMovement(int index)
{
	Game* game = (Game*)m_pGame;

	index -= GetPositionByIndex();

	int x = index % LEVEL_TILE_DIMENSIONS.x;
	int y = index / LEVEL_TILE_DIMENSIONS.x;

	vec2 new_pos = vec2((float)x, (float)y) * TILE_SIZE;

	if (game->GetActiveLevel()->GetTileAtPosition(GetPosition() + new_pos)->IsWalkable())
	{
		m_Transform.object_position += new_pos;
		return true;
	}

	return false;
}

