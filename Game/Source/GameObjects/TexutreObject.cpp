#include "GamePCH.h"
#include "Mesh/Mesh.h"

TextureObject::TextureObject(GameCore* pGame, Mesh* pMesh, const char* pName) : GameObject(pGame,pMesh)
{
	m_Image = ImageManager::UseImage(pName);
	assert(m_Image.gl_index != -1);
}

void TextureObject::Draw(vec2 camPos, vec2 projScale)
{

	if (m_pMesh != 0)
		m_pMesh->Draw(m_Position, m_Angle, 1, camPos, projScale, m_Image);
}
