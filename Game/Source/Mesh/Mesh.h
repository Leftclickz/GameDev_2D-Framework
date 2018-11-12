#ifndef __Mesh_H__
#define __Mesh_H__

struct VertexFormat
{
    vec2 m_Pos;
	vec2 m_UVCoord;
    MyColor m_Color;

    VertexFormat(vec2 pos, vec2 uv, MyColor color)
    {
        m_Pos = pos;
		m_UVCoord = uv;
        m_Color = color;
    }
};

class Mesh
{
protected:
    GLuint m_VBO;
    ShaderProgram* m_pShader;

    GLenum m_PrimitiveType;
    unsigned int m_NumVerts;

public:
    Mesh();
	Mesh(const VertexFormat* mesh_data, int vert_count, ShaderProgram* shader, GLuint primitive);

    virtual ~Mesh();

    void SetShader(ShaderProgram* pShader) { m_pShader = pShader; }

    void Draw(vec2 objectPos, float objectAngle, vec2 objectScale, vec2 camPos, vec2 projScale);
	void Draw(vec2 objectPos, float objectAngle, vec2 objectScale, vec2 camPos, vec2 projScale, struct Sprite *texture);

    void GenerateTriangle();
    void GenerateCircle();
	void Generate(const VertexFormat* data);
	void GenerateMeshFromAtlas(vec2 sprite_size, vec2 offset, vec2 atlas_size);
};

#endif //__Mesh_H__
