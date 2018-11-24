#include "GamePCH.h"
#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(GameCore* pGame, Mesh* pMesh, const char* pName) : GameObject(pGame, pMesh)
{
	if (pName != nullptr)
		m_Image = ImageManager::UseAnimation(pName);
	else
		m_Image = nullptr;
}

void AnimatedObject::Draw(vec2 camPos, vec2 projScale)
{
	GameObject::Draw(camPos, projScale);
	if (m_pMesh != 0 && m_Image != nullptr)
		m_pMesh->Draw(&m_Transform, m_Image->sprite_atlas->atlas_image, m_Image->FetchActiveSprite());

}
