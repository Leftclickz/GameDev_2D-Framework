#pragma once
#include "GameObject.h"

class Mesh;
class AnimatedSprite;

class AnimatedObject : public GameObject
{

public:
	AnimatedObject(GameCore* pGame, Mesh* pMesh, const char* pName);


	virtual void Draw(vec2 camPos, vec2 projScale) override;

protected:

	AnimatedSprite* m_Image;

};