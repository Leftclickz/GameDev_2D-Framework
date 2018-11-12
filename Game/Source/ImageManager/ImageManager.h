#pragma once

struct Sprite
{
	const char* texture_name;
	GLuint GL_texture_index;
	unsigned int TU_index;


	Sprite() : texture_name("failure to load"), GL_texture_index(-1), TU_index(-1){}
	Sprite(const char* NAME, GLuint INDEX, unsigned int ITEX) : texture_name(NAME), GL_texture_index(INDEX), TU_index(ITEX) {}
};

class SpriteAtlas
{

	//Raw Atlas data
	cJSON* atlas_data;

public:

	//Atlas size
	vec2 atlas_size;

	//Atlas spritesheet
	Sprite* atlas_image;

	//data for sprite
	cJSON* active_image;
	vec2 image_offset;
	vec2 image_size;

	SpriteAtlas() {}
	SpriteAtlas(Sprite* ATLAS);
	void UseFrame(const char* image_name);

};

class ImageManager
{
public:

	static void Initialize();
	static void LoadImageData(const char* name);

	static void LoadImageAtlas(const char* name);

	static void Reserve(unsigned int value);
	static void Release();
	static Sprite* UseImage(const char* name); 
	static SpriteAtlas* UseImageAtlas(const char* name);

private:

	static std::vector<Sprite> *LoadedImages;
	static std::vector<SpriteAtlas> *LoadedAtlas;
	static unsigned int TU_NUMBER;
};

