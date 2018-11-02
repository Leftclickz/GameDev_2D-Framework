#include "GamePCH.h"
#include "ImageManager.h"

std::vector<ImageData>* ImageManager::LoadedImages = new std::vector<ImageData>;



void ImageManager::Reserve(unsigned int value)
{
	LoadedImages->reserve(value);
}

void ImageManager::Release()
{
	delete LoadedImages;
}

void ImageManager::LoadImageData(const char* name)
{

	std::string file("");
	file += name;

	const char* filepath = file.c_str();
	
	GLuint index = LoadTexture(filepath);

	LoadedImages->push_back(ImageData(name, index, LoadedImages->size()));
}

ImageData ImageManager::UseImage(const char* name)
{
	for (unsigned int i = 0; i < LoadedImages->size(); i++)
	{
		if (LoadedImages->at(i).name == name)
			return LoadedImages->at(i);
	}
	
	return ImageData();
}
