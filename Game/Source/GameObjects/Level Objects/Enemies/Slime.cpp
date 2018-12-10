#include "GamePCH.h"

Slime::Slime(GameCore* pGame, Mesh* pMesh) : Enemy(pGame,pMesh, &ANIMATION_NAMES::SLIME_IDLE)
{
	m_Pathfinding = AI_Patterns::MoveUp;
	m_DamagedSound = AudioManager::GetAudio(&AUDIO_NAMES::SLIME_HIT);

	m_Life = 2.0f;
	m_Damage = 1.0f;
	m_Score = 10.0f;
}

void Slime::DoNextMove()
{
	int before_movement_index = GetPositionByIndex(); 
	int after_movement_index = before_movement_index;

	//Load the new movement into the index.
	m_Pathfinding(&after_movement_index);

	//Attempt a movement.
	MOVEMENT_CODES moved = AttemptMovement(after_movement_index);

	//If the move actually happened, do some logic to swap the next AI state.
	if (moved == MOVEMENT_CODES::MOVED_SUCCESSFULLY)
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
