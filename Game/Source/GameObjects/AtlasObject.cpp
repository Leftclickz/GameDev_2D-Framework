#include "GamePCH.h"

AtlasObject::AtlasObject(GameCore* pGame, Mesh* pMesh, const char* pName) : GameObject(pGame, pMesh)
{
	if (pName != "empty")
		m_Atlas = ImageManager::UseImageAtlas(pName);
}

//Generate an image using Atlas data. Requires the name of a sprite inside the image. Omit file endings.
void AtlasObject::UseSprite(const char* name)
{
	m_Image = m_Atlas->GetSprite(name);
}

void AtlasObject::Draw(vec2 camPos, vec2 projScale)
{
	if (m_pMesh != 0)
		m_pMesh->Draw(m_Position, m_Angle, 1, camPos, projScale, m_Atlas->atlas_image, m_Image);
}
