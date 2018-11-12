#pragma once
#include "GameObject.h"

class Mesh;

class AtlasObject : public GameObject
{

public:

	AtlasObject(GameCore* pGame, Mesh* pMesh,const char* pName);

	void UseFrame(const char* name);

	virtual void Draw(vec2 camPos, vec2 projScale) override;

protected:

	SpriteAtlas *m_Image;

};