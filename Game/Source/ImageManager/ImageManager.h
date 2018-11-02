#pragma once

struct ImageData
{
	ImageData() : name("failure to load"), gl_index(-1), texture_index(-1){};
	ImageData(const char* NAME, GLuint INDEX, unsigned int ITEX) : name(NAME), gl_index(INDEX), texture_index(ITEX)  {}
	const char* name;
	GLuint gl_index;
	unsigned int texture_index;
};

class ImageManager
{
public:

	static void LoadImageData(const char* name);
	static void Reserve(unsigned int value);
	static void Release();
	static ImageData UseImage(const char* name);

private:

	static std::vector<ImageData> *LoadedImages;
};

