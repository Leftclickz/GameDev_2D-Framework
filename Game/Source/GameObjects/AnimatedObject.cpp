#include "GamePCH.h"
#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(GameCore* pGame, Mesh* pMesh, const char* pName) : GameObject(pGame, pMesh)
{
	m_Image = ImageManager::UseAnimation(pName);
}

void AnimatedObject::Update(float deltatime)
{
	m_Image->Update(deltatime);
}

void AnimatedObject::Draw(vec2 camPos, vec2 projScale)
{
	if (m_pMesh != 0)
		m_pMesh->Draw(m_Position, m_Angle, 1, camPos, projScale, m_Image->sprite_atlas->atlas_image, m_Image->FetchActiveSprite());
}
