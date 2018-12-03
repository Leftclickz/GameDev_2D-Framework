#include "GamePCH.h"
#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(GameCore* pGame, Mesh* pMesh, const char** pName) : GameObject(pGame, pMesh)
{
	m_TexTransform.world_transform = &m_Transform;

	if (pName != nullptr)
	{
		m_Image = ImageManager::UseAnimation(pName);
		SetTextureTransform();
	}
	else
		m_Image = nullptr;

	m_Type = DEFAULT;
	m_DamagedSound = nullptr;
}

void AnimatedObject::Draw()
{
	GameObject::Draw();

	if (m_pMesh != 0 && m_Image != nullptr)
		m_pMesh->Draw(&m_TexTransform);

}

void AnimatedObject::TakeDamage(float damage)
{
	m_Life -= damage;

	if (m_DamagedSound != nullptr)
	{
		m_DamagedSound->Stop();
		m_DamagedSound->Play();
	}

	if (m_Life <= 0.0f)
	{
		//tell the level to forget this entity exists
		Game* game = (Game*)m_pGame;
		game->GetActiveLevel()->DestroyEntity(this);
	}
}
