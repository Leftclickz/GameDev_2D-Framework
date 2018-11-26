#ifndef __Mesh_H__
#define __Mesh_H__

struct Sprite;
struct AtlasChild;

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

struct WorldTransform
{
	vec2 object_position;
	vec2 object_anchor;
	float angle;
	vec2 object_scale;
};

struct TexturedTransform
{
	WorldTransform* world_transform;
	Sprite* atlas_image;
	AtlasChild** rendered_image;
};

void SetUniform2f(GLuint shader, const char* uniformName, vec2 value);

class Mesh
{
protected:
    GLuint m_VBO;

    GLenum m_PrimitiveType;
    unsigned int m_NumVerts;

	//debug vars
	bool m_DrawDebugLines;
	MyColor* m_DebugColor;

	//Internal mesh generation.
	void Generate(const VertexFormat* data);

	//Interal draw call for textures
	void DrawTexture(WorldTransform* transform);

	//Internal Draw call for debug lines
	void DebugDraw(WorldTransform* transform);

public:
    Mesh();
	Mesh(const VertexFormat* mesh_data, int vert_count, GLuint primitive);

    virtual ~Mesh();

	//Is the mesh in debug mode?
	bool IsDebugging() { return m_DrawDebugLines; }

	//Set the color of the debug lines
	void SetDebugColor(std::string color);

	//Set debug mode on or off
	void SetDrawDebugLines(bool value = true) { m_DrawDebugLines = value; }

	//Draw call for game objects
	void Draw(WorldTransform* transform, Sprite *texture);
	void Draw(WorldTransform* transform, Sprite *texture,  AtlasChild *sprite_data);
	void Draw(TexturedTransform* transform);

	//Generate a mesh from paramaters. Default primitive is TRIANGLE_FAN
	void Generate(const VertexFormat* data, int vertcount, GLuint primitive = GL_TRIANGLE_FAN);

	//Generate a mesh from Atlas data.
	void GenerateMeshFromAtlas(vec2 sprite_size);
};

#endif //__Mesh_H__
