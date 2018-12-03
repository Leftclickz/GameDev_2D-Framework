#include "GamePCH.h"
#include "../../../Game/Game.h"
#include "Enemy.h"

Enemy::Enemy(GameCore* pGame, Mesh* pMesh, const char** pName) : AnimatedObject(pGame,pMesh,pName)
{
	SetAnchor(vec2(0.0f, TILE_SIZE.y * 0.5f));

	m_pGame = pGame;
	m_Life = 1.0f;
	m_Damage = 0.5f;
	m_Type = ENEMY;
	m_Pathfinding = AI_Patterns::IdleMovement;
}

void Enemy::TakeDamage(float damage)
{
	AnimatedObject::TakeDamage(damage);
}


bool Enemy::AttemptMovement(int index)
{

	index -= GetPositionByIndex();

	//idle movement is always possible
	if (index == 0)
		return true;

	Game* game = (Game*)m_pGame;


	int x = index % LEVEL_TILE_DIMENSIONS.x;
	int y = index / LEVEL_TILE_DIMENSIONS.x;

	vec2 new_pos = vec2((float)x, (float)y) * TILE_SIZE;

	//check if the enemy will collide into a wall
	if (game->GetActiveLevel()->GetTileAtPosition(GetPosition() + new_pos)->IsWalkable() == false)
		return false;

	//check if it will collide with any enemies or the player
	if (game->GetActiveLevel()->CheckForCollisionsAt(index + GetPositionByIndex(), this) == true)
		return false;

	//move the enemy if it wont be blocked.
	m_Transform.object_position += new_pos;
	return true;
}

