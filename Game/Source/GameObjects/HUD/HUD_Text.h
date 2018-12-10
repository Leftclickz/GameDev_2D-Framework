#pragma once
#include "../TextObject.h"
#include"../HUD_Element.h"

enum TEXT_ALLIGNMENT
{
	ALLIGNMENT_NORMAL,
	ALLIGNMENT_CENTER
};

class HUD_Text : public TextObject, public HUD_ELEMENT
{

public:

	HUD_Text(GameCore* game, TextMesh* mesh);

	virtual void Draw() override;
	virtual void SetAllignment(TEXT_ALLIGNMENT allignment) { m_Allignment = allignment; }

protected:

	TEXT_ALLIGNMENT m_Allignment;

};

