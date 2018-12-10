#pragma once


#include "HUD_Text.h"

class HUD_ScoreDisplay : public HUD_Text
{

public:
	HUD_ScoreDisplay(Game* game, TextMesh* mesh);

	//score setters and getters
	virtual void IncreaseScore(float value) { m_Score += value; GenerateNewText(); }
	virtual void SetScore(float value) { m_Score = value; GenerateNewText(); }
	virtual float GetScore() { return m_Score; }

	virtual void GenerateNewText() override;


protected:

	float m_Score;

};