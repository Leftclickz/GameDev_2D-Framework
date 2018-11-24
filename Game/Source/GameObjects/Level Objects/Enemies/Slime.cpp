#include "GamePCH.h"

Slime::Slime(GameCore* pGame, Mesh* pMesh) : Enemy(pGame,pMesh, "SlimeGreen_Idle")
{
	m_Pathfinding = AI_Patterns::MoveUp;
}

void Slime::DoNextMove()
{
	int next_index = m_Pathfinding();
	bool moved = AttemptMovement(next_index);

	if (moved)
	{
		//If we moved, remember the last movement to set the next pathfinding properly.
		if (abs(next_index) == LEVEL_TILE_DIMENSIONS.x )
		{
			m_Pathfinding = AI_Patterns::IdleMovement;
			m_LastIndexMoved = next_index;
		}
		else {
			if (m_LastIndexMoved < 0)
				m_Pathfinding = AI_Patterns::MoveUp;
			else
				m_Pathfinding = AI_Patterns::MoveDown;
		}
	}
}
