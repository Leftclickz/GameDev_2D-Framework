#pragma once

struct Sprite;
struct SpriteAtlas;
class AnimatedSprite;

class ImageManager
{
public:

	static void Initialize();

	static void LoadImageData(const char** name);
	static void LoadImageAtlas(const char** name, const char** convention);
	static AnimatedSprite* CreateAnimation(const char** name, const char** atlas);

	static void Update(float deltatime);

	static void Reserve(unsigned int value);
	static void Release();

	static Sprite* UseImage(const char** name); 
	static SpriteAtlas* UseImageAtlas(const char** name);
	static AnimatedSprite* UseAnimation(const char** name);

private:

	static void UnwrapJSONFileToAtlas(const char** name, const char** convention);


	static std::vector<Sprite> *LoadedImages;
	static std::vector<SpriteAtlas*> *LoadedAtlas;
	static std::vector<AnimatedSprite*> *LoadedAnimations;
	static unsigned int TU_NUMBER;
};

