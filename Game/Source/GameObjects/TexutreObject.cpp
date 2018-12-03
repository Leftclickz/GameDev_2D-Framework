#include "GamePCH.h"
#include "Mesh/Mesh.h"

TextureObject::TextureObject(GameCore* pGame, Mesh* pMesh, const char** pName) : GameObject(pGame,pMesh)
{
	m_Image = ImageManager::UseImage(pName);
	assert(m_Image->GL_texture_index != -1);
}

void TextureObject::Draw()
{
	GameObject::Draw();
	if (m_pMesh != 0)
		m_pMesh->Draw(&m_Transform, m_Image);
}
