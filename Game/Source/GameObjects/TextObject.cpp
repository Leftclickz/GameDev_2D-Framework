#include "GamePCH.h"

TextObject::TextObject(GameCore* game, TextMesh* mesh) : GameObject (game, nullptr)
{
	m_pMesh = mesh;
}

void TextObject::Draw()
{
	GameObject::Draw();

	if (m_pMesh != nullptr)
		((TextMesh*)m_pMesh)->Draw(&m_Transform);
}
