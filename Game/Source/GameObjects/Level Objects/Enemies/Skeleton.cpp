#include "GamePCH.h"

Skeleton::Skeleton(GameCore* pGame, Mesh* pMesh) : Enemy(pGame,pMesh, "SkeletonWhite_Idle")
{
	m_Pathfinding = AI_Patterns::IdleMovement;
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
			m_Image = ImageManager::UseAnimation("SkeletonWhite_Attacking");
			SetTextureTransform();
		}
		else
		{
			m_Pathfinding = AI_Patterns::IdleMovement;
			m_Image = ImageManager::UseAnimation("SkeletonWhite_Idle");
			SetTextureTransform();
		}
	}
}
