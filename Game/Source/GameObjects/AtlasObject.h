#pragma once
#include "GameObject.h"

class Mesh;

class AtlasObject : public GameObject
{

public:

	AtlasObject(GameCore* pGame, Mesh* pMesh,const char* pName);

	//Generate an image using Atlas data. Requires the name of a sprite inside the image. Omit file endings.
	virtual void UseSprite(const char* name);

	virtual void Draw() override;

	virtual SpriteAtlas* GetSpriteAtlas() { return m_Image; }

protected:

	SpriteAtlas *m_Image;
	AtlasChild* m_Sprite;

};