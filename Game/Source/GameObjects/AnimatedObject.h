#pragma once
#include "GameObject.h"

class Mesh;
class AnimatedSprite;

enum EntityType
{
	DEFAULT = 0,
	PLAYER = 1,
	ENEMY = 2,
};

class AnimatedObject : public GameObject
{

public:
	AnimatedObject(GameCore* pGame, Mesh* pMesh, const char** pName);
	virtual ~AnimatedObject() {}

	virtual void Draw() override;

	virtual AnimatedSprite* GetAnimation() { return m_Image; }

	virtual TexturedTransform* GetTexturedTransform() { return &m_TexTransform; }

	virtual void SetDamageCount(float damage) { m_Damage = damage; }
	virtual float GetDamageCount() { return m_Damage; }

	virtual void TakeDamage(float damage);
	virtual float GetLifeCount() { return m_Life; }

	virtual EntityType GetType() { return m_Type; }

protected:

	virtual void SetTextureTransform() { m_TexTransform.atlas_image = m_Image->sprite_atlas->atlas_image; m_TexTransform.rendered_image = m_Image->FetchActiveSprite(); }

	AnimatedSprite* m_Image;
	TexturedTransform m_TexTransform;

	//sounds
	Audio* m_DamagedSound;

	//stats
	float m_Life;
	float m_Damage;

	//entity type
	EntityType m_Type;
};