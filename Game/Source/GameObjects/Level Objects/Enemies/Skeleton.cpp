#include "GamePCH.h"

Skeleton::Skeleton(GameCore* pGame, Mesh* pMesh) : Enemy(pGame,pMesh, &ANIMATION_NAMES::SKELE_IDLE)
{
	m_Pathfinding = AI_Patterns::IdleMovement;
	m_DamagedSound = AudioManager::GetAudio(&AUDIO_NAMES::SKELE_HIT);

	m_Life = 1.0f;
	m_Damage = 0.5f;
}

void Skeleton::DoNextMove()
{
	int index = GetPositionByIndex();
	m_Pathfinding(&index);

	bool moved = AttemptMovement(index);

	if (moved == true)
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
