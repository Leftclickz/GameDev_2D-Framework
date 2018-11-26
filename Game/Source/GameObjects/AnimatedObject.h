#pragma once
#include "GameObject.h"

class Mesh;
class AnimatedSprite;

class AnimatedObject : public GameObject
{

public:
	AnimatedObject(GameCore* pGame, Mesh* pMesh, const char* pName);

	virtual void Draw() override;

	virtual AnimatedSprite* GetAnimation() { return m_Image; }

	virtual TexturedTransform* GetTexturedTransform() { return &m_TexTransform; }

protected:

	virtual void SetTextureTransform() { m_TexTransform.atlas_image = m_Image->sprite_atlas->atlas_image; m_TexTransform.rendered_image = m_Image->FetchActiveSprite(); }

	AnimatedSprite* m_Image;
	TexturedTransform m_TexTransform;

};