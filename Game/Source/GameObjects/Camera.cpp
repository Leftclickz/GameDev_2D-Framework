#include "GamePCH.h"

Camera::Camera(GameCore* m_Game, Mesh* mesh /*= nullptr*/) : GameObject(m_Game,mesh)
{
	m_Owner = nullptr;
}

