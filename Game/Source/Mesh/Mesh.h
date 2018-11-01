#ifndef __Mesh_H__
#define __Mesh_H__

struct VertexFormat
{
    vec2 m_Pos;
    MyColor m_Color;

    VertexFormat(vec2 pos, MyColor color)
    {
        m_Pos = pos;
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
    virtual ~Mesh();

    void SetShader(ShaderProgram* pShader) { m_pShader = pShader; }

    void Draw(vec2 objectPos, float objectAngle, vec2 objectScale, vec2 camPos, vec2 projScale);

    void GenerateTriangle();
    void GenerateCircle();
};

#endif //__Mesh_H__
