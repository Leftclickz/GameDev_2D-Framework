#include "GamePCH.h"
#include "HUD_Text.h"

HUD_Text::HUD_Text(GameCore * game, TextMesh * mesh) : TextObject(game,mesh)
{
}

void HUD_Text::Draw()
{
	//Get our anchor from HUD
	m_Transform.object_anchor = GetHUDAnchor();

	//allign our text appropriately
	switch (m_Allignment)
	{
	case ALLIGNMENT_NORMAL:
		break;
	case ALLIGNMENT_CENTER:
		m_Transform.object_anchor -= vec2(m_pMesh->GetSize().x / 2.0f, 0.0f);
		break;
	}

	TextObject::Draw();
}
