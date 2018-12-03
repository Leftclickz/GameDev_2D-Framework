#include "GamePCH.h"

Wall::Wall(GameCore* pGame, Mesh* pMesh, const char** pName) : AtlasObject(pGame,pMesh,pName)
{
	//unsigned int random_index = rand() % 8;

	m_Sprite = m_Image->GetSpriteAtIndex(0);

	SetAnchor(vec2(0.0f, TILE_SIZE.y * 0.5f));
}

void Wall::SetPosition(int index)
{
	GameObject::SetPosition(index);
	GameObject::SetPosition(m_Transform.object_position + vec2(0.0f, -50.0f));
}
