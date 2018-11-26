#include "GamePCH.h"
#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(GameCore* pGame, Mesh* pMesh, const char* pName) : GameObject(pGame, pMesh)
{
	m_TexTransform.world_transform = &m_Transform;

	if (pName != nullptr)
	{
		m_Image = ImageManager::UseAnimation(pName);
		SetTextureTransform();
	}
	else
		m_Image = nullptr;
}

void AnimatedObject::Draw()
{
	GameObject::Draw();

	if (m_pMesh != 0 && m_Image != nullptr)
		m_pMesh->Draw(&m_TexTransform);

}
