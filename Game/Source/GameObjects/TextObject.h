#pragma once
#include "GameObject.h"

class TextMesh;

class TextObject : public GameObject
{
public:

	TextObject(GameCore* game, TextMesh* mesh);

	void SetText(std::string text) { ((TextMesh*)m_pMesh)->SetText(text); }

	virtual void Draw(vec2 camPos, vec2 projScale) override;

protected:

	virtual std::string GetDebugTag() { return "Text"; }

};