#include "GamePCH.h"
#include "ImageManager.h"

std::vector<Sprite>* ImageManager::LoadedImages = nullptr;
std::vector<SpriteAtlas>* ImageManager::LoadedAtlas = nullptr;
unsigned int ImageManager::TU_NUMBER = 0;


SpriteAtlas::SpriteAtlas(Sprite* ATLAS) : atlas_image(ATLAS)
{
	//Get file into JSON root
	std::string filepath = "Data/Images/Atlas/" + (std::string)atlas_image->texture_name + ".json";

	long length;
	char* loaded_file = LoadCompleteFile(filepath.c_str(), &length);
	cJSON* root = cJSON_Parse(loaded_file);

	//Load size into vector and list of sprites into data storage
	atlas_size = vec2((float)cJSON_GetObjectItem(root, "width")->valuedouble, (float)cJSON_GetObjectItem(root, "height")->valuedouble);
	atlas_data = cJSON_GetObjectItem(root, "Files");

	delete[] loaded_file;
}

//Generate an image using Atlas data. Requires the name of a sprite inside the image. Omit file endings.
void SpriteAtlas::UseFrame(const char* image_name)
{
	cJSON* object;

	//filename pathing
	std::string file = image_name;
	file += ".png";

	bool found = false;
	int index = 0;

	//loop until we find our frame. if we can't find it the program will break.
	while (!found)
	{
		object = cJSON_GetArrayItem(atlas_data, index);

		if (cJSON_GetObjectItem(object, "filename")->valuestring == file)
		{
			found = true;
			active_image = object;

			//Load our image offset in the atlas as well as the image size for UV calculations later.
			image_offset = vec2((float)cJSON_GetObjectItem(active_image, "posx")->valuedouble, (float)cJSON_GetObjectItem(active_image, "posy")->valuedouble);
			image_size = vec2((float)cJSON_GetObjectItem(active_image, "origw")->valuedouble, (float)cJSON_GetObjectItem(active_image, "origh")->valuedouble);
		}
		else
			index++;
	}
}

void ImageManager::Initialize()
{
	if (LoadedImages == nullptr)
		LoadedImages = new std::vector<Sprite>;

	if (LoadedAtlas == nullptr)
		LoadedAtlas = new std::vector<SpriteAtlas>;
}

void ImageManager::Reserve(unsigned int value)
{
	LoadedImages->reserve(value);
	LoadedAtlas->reserve(value);
}

void ImageManager::Release()
{
	delete LoadedImages;
	LoadedImages = nullptr;
	delete LoadedAtlas;
	LoadedAtlas = nullptr;
}


void ImageManager::LoadImageData(const char* name)
{
	//filename pathing
	std::string file = "Data/Images/" + (std::string)name + ".png";

	//load the file
	GLuint gl_texture_index = LoadTexture(file.c_str(), TU_NUMBER);

	//Push a new sprite and increment the TU counter.
	LoadedImages->push_back(Sprite(name, gl_texture_index, TU_NUMBER));
	TU_NUMBER++;
}

void ImageManager::LoadImageAtlas(const char* name)
{
	//filename pathing
	std::string file = "Data/Images/Atlas/" + (std::string)name + ".png";

	//load the file
	GLuint gl_texture_index = LoadTexture(file.c_str(), TU_NUMBER);

	//Push a new Sprite (the giant atlas is still a sprite!) and a SpriteAtlas object (which will contain the new sprite). Increment TU counter once done.
	LoadedImages->push_back(Sprite(name, gl_texture_index, TU_NUMBER));
	LoadedAtlas->push_back(SpriteAtlas(UseImage(name)));
	TU_NUMBER++;
}

Sprite* ImageManager::UseImage(const char* name)
{

	//Attempts to find an image in the list of loaded images that matches the name
	for (unsigned int i = 0; i < LoadedImages->size(); i++)
	{
		if (LoadedImages->at(i).texture_name == name)
			return &LoadedImages->at(i);

	}
	//If we got here the name is wrong.
	assert(false);
	return new Sprite();
}

SpriteAtlas* ImageManager::UseImageAtlas(const char* name)
{
	//Attempts to find an atlas in the list of loaded atlas that matches the name
	for (unsigned int i = 0; i < LoadedImages->size(); i++)
	{
		if (LoadedAtlas->at(i).atlas_image->texture_name == name)
			return &LoadedAtlas->at(i);
	}
	//If we got here the name is wrong.
	assert(false);
	return new SpriteAtlas();
}
