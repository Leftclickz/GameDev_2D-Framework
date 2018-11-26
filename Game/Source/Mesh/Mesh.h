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
	WorldTransform() : object_position(vec2(0.0f, 0.0f)), object_anchor(vec2(0.0f, 0.0f)), angle(0.0f), object_scale(vec2(1.0f, 1.0f)) {}
	WorldTransform(vec2 position) : object_position(position), object_anchor(vec2(0.0f, 0.0f)), angle(0.0f), object_scale(vec2(1.0f, 1.0f)) {}

	vec2 object_position;
	vec2 object_anchor;
	float angle;
	vec2 object_scale;
};

struct TexturedTransform
{
	TexturedTransform() {}
	TexturedTransform(WorldTransform* trans, Sprite* atlas, AtlasChild** image) : world_transform(trans), atlas_image(atlas), rendered_image(image) {}

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

	//Vertices
    unsigned int m_NumVerts;
	std::vector<VertexFormat>* m_Verts;

	//debug vars
	bool m_DrawDebugLines;
	MyColor* m_DebugColor;

	//Internal mesh generation.
	void Generate();

	//Interal draw call for textures
	virtual void DrawBuffer(WorldTransform* transform, GLuint shader);

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

	//Draw call for game objects with world transforms
	void Draw(WorldTransform* transform, Sprite *texture,  AtlasChild *sprite_data = nullptr);

	//Overloaded call for textures with a full transform
	void Draw(TexturedTransform* transform);

	//Generate a mesh from paramaters. Default primitive is TRIANGLE_FAN
	void Generate(const VertexFormat* data, int vertcount, GLuint primitive = GL_TRIANGLE_FAN);

	//Generate a mesh from Atlas data.
	void GenerateMeshFromAtlas(vec2 sprite_size);

	//Fetch vertex count
	int GetVertCount() { return m_NumVerts; }

	//Fetch vertex vector
	std::vector<VertexFormat>* GetVerts() { return m_Verts; }
};

#endif //__Mesh_H__