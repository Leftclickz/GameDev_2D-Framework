#include "GamePCH.h"

Slime::Slime(GameCore* pGame, Mesh* pMesh) : Enemy(pGame,pMesh, "SlimeGreen_Idle")
{
	m_Pathfinding = AI_Patterns::MoveUp;
}

void Slime::DoNextMove()
{
	int before_movement_index = GetPositionByIndex(); 
	int after_movement_index = before_movement_index;

	//Load the new movement into the index.
	m_Pathfinding(&after_movement_index);

	//Attempt a movement.
	bool moved = AttemptMovement(after_movement_index);

	//If the move actually happened, do some logic to swap the next AI state.
	if (moved)
	{
		//convert the new index into just the actual movement itself.
		after_movement_index -= before_movement_index;

		//If our move wasn't idle, remember the last movement index to set the next pathfinding properly.
		if (abs(after_movement_index) == LEVEL_TILE_DIMENSIONS.x )
		{
			m_Pathfinding = AI_Patterns::IdleMovement;
			m_LastIndexMoved = after_movement_index;
		}
		else 
		//If our move was idle, use our last remembered movement to determine whether we now go up or down.
		{
			if (m_LastIndexMoved < 0)
				m_Pathfinding = AI_Patterns::MoveUp;
			else
				m_Pathfinding = AI_Patterns::MoveDown;
		}
	}
}
