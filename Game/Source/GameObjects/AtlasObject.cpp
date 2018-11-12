#include "GamePCH.h"

AtlasObject::AtlasObject(GameCore* pGame, Mesh* pMesh, const char* pName) : GameObject(pGame, pMesh)
{
	m_Image = ImageManager::UseImageAtlas(pName);
}

void AtlasObject::UseFrame(const char* name)
{
	m_Image->UseFrame(name);
	m_pMesh->GenerateMeshFromAtlas(m_Image->image_size, m_Image->image_offset, m_Image->atlas_size);
}

void AtlasObject::Draw(vec2 camPos, vec2 projScale)
{
	if (m_pMesh != 0)
		m_pMesh->Draw(m_Position, m_Angle, 1, camPos, projScale, m_Image->atlas_image);
}
