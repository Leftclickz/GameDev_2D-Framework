#include "GamePCH.h"
#include "HUD_ScoreDisplay.h"

HUD_ScoreDisplay::HUD_ScoreDisplay(Game * game, TextMesh * mesh) : HUD_Text(game,mesh)
{
	m_Score = 0.0f;
}

void HUD_ScoreDisplay::GenerateNewText()
{
	int score = (int)m_Score;

	std::string text = m_Text + std::to_string(score);

	((TextMesh*)m_pMesh)->SetText(text);
}
