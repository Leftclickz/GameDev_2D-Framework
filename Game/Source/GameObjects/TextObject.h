#pragma once
#include "GameObject.h"
#include "HUD_Element.h"

class TextMesh;

class TextObject : public GameObject
{
public:

	TextObject(GameCore* game, TextMesh* mesh);
	virtual ~TextObject() { delete m_pMesh; m_pMesh = nullptr; }
	void SetText(std::string text) {m_Text = text; GenerateNewText();}

	virtual void GenerateNewText() { ((TextMesh*)m_pMesh)->SetText(m_Text); }

	virtual void Draw() override;

protected:

	virtual std::string GetDebugTag() { return "Text"; }
	std::string m_Text;
};