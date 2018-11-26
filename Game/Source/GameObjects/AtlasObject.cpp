#include "GamePCH.h"

AtlasObject::AtlasObject(GameCore* pGame, Mesh* pMesh, const char* pName) : GameObject(pGame, pMesh)
{
	if (pName != "empty")
		m_Image = ImageManager::UseImageAtlas(pName);
}

//Generate an image using Atlas data. Requires the name of a sprite inside the image. Omit file endings.
void AtlasObject::UseSprite(const char* name)
{
	m_Sprite = m_Image->GetSprite(name);
}

void AtlasObject::Draw()
{
	GameObject::Draw();

	if (m_pMesh != 0)
		m_pMesh->Draw(&m_Transform, m_Image->atlas_image, m_Sprite);
}
