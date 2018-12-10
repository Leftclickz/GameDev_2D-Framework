#pragma once
#include "Mesh.h"

struct SpriteAtlas;
class ShaderProgram;

class TextMesh : public Mesh
{

public:

	TextMesh();
	virtual ~TextMesh() {}

	TextMesh(std::string text);

	void SetText(std::string text);
	std::string GetText() { return m_DisplayText; }

	void Draw(WorldTransform* transform);

protected:

	void GenerateTextMesh();

	std::string m_DisplayText;
	SpriteAtlas* m_TextAtlas;
};