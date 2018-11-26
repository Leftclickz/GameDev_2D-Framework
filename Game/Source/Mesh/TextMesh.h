#pragma once
#include "Mesh.h"

struct SpriteAtlas;
class ShaderProgram;

class TextMesh : public Mesh
{

public:

	TextMesh();
	TextMesh(std::string text, ShaderProgram* textureShader, ShaderProgram* debugShader);

	void SetText(std::string text);
	std::string GetText() { return m_Text; }

	void Draw(WorldTransform* transform);

protected:

	void GenerateTextMesh();

	std::string m_Text;
	SpriteAtlas* m_TextAtlas;
};