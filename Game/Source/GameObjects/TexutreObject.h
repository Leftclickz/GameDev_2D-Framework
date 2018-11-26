#pragma once
#include "GameObject.h"

class Mesh;

class TextureObject : public GameObject
{

public:

	TextureObject(GameCore* pGame, Mesh* pMesh, const char* pName);
	virtual void Draw() override;

protected:

	Sprite *m_Image;

};

