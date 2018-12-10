#include "GamePCH.h"
#include "HUD_Sprite.h"

HUD_Sprite::HUD_Sprite(GameCore* pGame, Mesh* pMesh, const char** pName) : AtlasObject(pGame,pMesh,pName)
{
	m_Sprite = m_Image->GetSprite("heart");
}

void HUD_Sprite::Draw()
{
	m_Transform.object_anchor = GetHUDAnchor();

	AtlasObject::Draw();
}
