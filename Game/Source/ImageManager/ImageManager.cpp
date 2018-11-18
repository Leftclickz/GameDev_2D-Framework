#include "GamePCH.h"
#include "ImageManager.h"

std::vector<Sprite>* ImageManager::LoadedImages = nullptr;
std::vector<SpriteAtlas*>* ImageManager::LoadedAtlas = nullptr;
std::vector<AnimatedSprite*> *ImageManager::LoadedAnimations = nullptr;
unsigned int ImageManager::TU_NUMBER = 0;

void ImageManager::Initialize()
{
	if (LoadedImages == nullptr)
		LoadedImages = new std::vector<Sprite>;

	if (LoadedAtlas == nullptr)
		LoadedAtlas = new std::vector<SpriteAtlas*>;

	if (LoadedAnimations == nullptr)
		LoadedAnimations = new std::vector<AnimatedSprite *>;
}

void ImageManager::Reserve(unsigned int value)
{
	LoadedImages->reserve(value);
	LoadedAtlas->reserve(value);
	LoadedAnimations->reserve(value);
}

void ImageManager::Release()
{
	//Release all owned sprites by the atlas vector
	for (unsigned int i = 0; i < LoadedAtlas->size(); i++)
		delete LoadedAtlas->at(0);

	//Release all animation sequences
	for (unsigned int i = 0; i < LoadedAnimations->size(); i++)
		delete LoadedAnimations->at(0);

	//Release atlas vector memory
	delete LoadedAtlas;
	LoadedAtlas = nullptr;

	//Release animation sequence vector memory
	delete LoadedAnimations;
	LoadedAnimations = nullptr;

	//Release image vector memory
	delete LoadedImages;
	LoadedImages = nullptr;
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

void ImageManager::LoadImageAtlas(const char* name, const char* convention)
{
	//filename pathing
	std::string file = "Data/Images/Atlas/" + (std::string)name + ".png";

	//load the file
	GLuint gl_texture_index = LoadTexture(file.c_str(), TU_NUMBER);

	//Push a new Sprite (the giant atlas is still a sprite!) and a SpriteAtlas object (which will contain the new sprite). Increment TU counter once done.
	LoadedImages->push_back(Sprite(name, gl_texture_index, TU_NUMBER));
	TU_NUMBER++;

	LoadedAtlas->push_back(new SpriteAtlas(UseImage(name)));
	UnwrapJSONFileToAtlas(name, convention);
	
}

void ImageManager::CreateAnimation(const char* name, const char* atlas)
{
	LoadedAnimations->push_back(new AnimatedSprite(name, atlas));
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
		if (LoadedAtlas->at(i)->atlas_image->texture_name == name)
			return LoadedAtlas->at(i);
	}

	//If we got here the name is wrong.
	assert(false);
	return new SpriteAtlas();
}

AnimatedSprite* ImageManager::UseAnimation(const char* name)
{
	//Attempts to find an atlas in the list of loaded atlas that matches the name
	for (unsigned int i = 0; i < LoadedAnimations->size(); i++)
	{
		if (LoadedAnimations->at(i)->animation_name == name)
			return LoadedAnimations->at(i);
	}

	//If we got here the name is wrong.
	assert(false);
	return new AnimatedSprite();
}

void ImageManager::UnwrapJSONFileToAtlas(const char* name, const char* convention)
{
	//Get atlas we're loading data into
	SpriteAtlas* atlas = UseImageAtlas(name);

	if (convention == "SpriteTool")
	{
		//Get filepath
		std::string filepath = "Data/Images/Atlas/" + (std::string)atlas->atlas_image->texture_name + ".json";

		long length;

		//Load file, put into cJSON object and delete the old array.
		char* loaded_file = LoadCompleteFile(filepath.c_str(), &length);
		cJSON* root = cJSON_Parse(loaded_file);
		delete[] loaded_file;

		//Load size into vector
		atlas->atlas_size = vec2((float)cJSON_GetObjectItem(root, "width")->valuedouble, (float)cJSON_GetObjectItem(root, "height")->valuedouble);

		//Get all sprites into cJSON object
		cJSON* sprite_data = cJSON_GetObjectItem(root, "Files");

		//Get our sprite counter for this sheet
		const int sprite_count = cJSON_GetArraySize(sprite_data);
		atlas->atlas_sprite_total = sprite_count;

		//Size our array to fit all sprites in the Sprite Sheet.
		atlas->atlas_sprites = new AtlasChild[sprite_count];
		
		//Iterate through the file and load sprite data into array.
		for (int i = 0; i < sprite_count; i++)
		{
			AtlasChild child;
			cJSON* child_data = cJSON_GetArrayItem(sprite_data, i);

			//pull out the image offset and image size
			vec2 image_offset = vec2((float)cJSON_GetObjectItem(child_data, "posx")->valuedouble, (float)cJSON_GetObjectItem(child_data, "posy")->valuedouble);
			vec2 image_size = vec2((float)cJSON_GetObjectItem(child_data, "origw")->valuedouble, (float)cJSON_GetObjectItem(child_data, "origh")->valuedouble);

			//Load up our Atlas child. For now we're storing the name, UV Scale and UV Offset.
			child.name = cJSON_GetObjectItem(child_data, "filename")->valuestring;
			child.sprite_UV_Scale = image_size / atlas->atlas_size;
			child.sprite_UV_Offset = vec2(image_offset.x / atlas->atlas_size.x, image_offset.y / atlas->atlas_size.y);

			//Load it into our array of children
			atlas->atlas_sprites[i] = child;
		}
	}
	else
		//If we're here the convention was misnamed and there was no way to open the file.
		assert(false);

}
