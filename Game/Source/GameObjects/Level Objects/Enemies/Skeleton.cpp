#include "GamePCH.h"

Skeleton::Skeleton(GameCore* pGame, Mesh* pMesh) : Enemy(pGame,pMesh, &ANIMATION_NAMES::SKELE_IDLE)
{
	m_Pathfinding = AI_Patterns::IdleMovement;
	m_DamagedSound = AudioManager::GetAudio(&AUDIO_NAMES::SKELE_HIT);

	m_Life = 1.0f;
	m_Damage = 1.0f;
	m_Score = 20.0f;
}

void Skeleton::DoNextMove()
{
	int index = GetPositionByIndex();
	m_Pathfinding(&index);

	MOVEMENT_CODES moved = AttemptMovement(index);

	if (moved != MOVEMENT_CODES::BLOCKED_BY_WALL)
	{
		if (m_Pathfinding == AI_Patterns::IdleMovement)
		{
			m_Pathfinding = AI_Patterns::CardinalAStarPathFinding;
			m_Image = ImageManager::UseAnimation(&ANIMATION_NAMES::SKELE_ATK);
			SetTextureTransform();
		}
		else
		{
			m_Pathfinding = AI_Patterns::IdleMovement;
			m_Image = ImageManager::UseAnimation(&ANIMATION_NAMES::SKELE_IDLE);
			SetTextureTransform();
		}
	}
}
