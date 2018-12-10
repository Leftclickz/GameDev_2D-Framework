#pragma once
#include "../AtlasObject.h"
#include "../HUD_Element.h"

class HUD_Sprite : public AtlasObject, public HUD_ELEMENT
{

public:
	//Default class for creating HUD textures that do no animate.
	HUD_Sprite(GameCore* pGame, Mesh* pMesh, const char** pName);

	//Draw our HUD sprite
	virtual void Draw() override;
};
