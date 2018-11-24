#include "GamePCH.h"
#include "SpriteDataStructures.h"

SpriteAtlas::~SpriteAtlas()
{
	delete[] atlas_sprites;
	atlas_sprites = nullptr;
}

//Generate an image using Atlas data. Requires the name of a sprite inside the image. Omit file endings.
AtlasChild* SpriteAtlas::GetSprite(const char* image_name)
{
	//filename pathing
	std::string file = (std::string)image_name + ".png";

	bool found = false;
	int index = 0;

	//loop until we find our frame. if we can't find it the program will break.
	while (!found)
	{
		if (atlas_sprites[index].name == file)
		{
			found = true;
			return &atlas_sprites[index];
		}
		else
			index++;

		//this runs if the name is wrong and the while loop is going to go indefinitely.
		if (index > atlas_sprite_total)
			assert(false);
	}

	return new AtlasChild();
}

AtlasChild* SpriteAtlas::GetSpriteAtIndex(unsigned int index)
{
	return &atlas_sprites[index];
}

AnimatedSprite::AnimatedSprite(const char* name, const char* atlas_name)
{
	//the name of our animation sequence
	animation_name = name;

	//fetch the atlas we'll be making an animation from
	sprite_atlas = ImageManager::UseImageAtlas(atlas_name);

	//Create the timer
	animation_timer = new Timer();
	animation_timer->SetDoesLoop(true);

	//initialize total sprites in animation to 0
	total_animated_sprites = 0;

	animation_sprites = nullptr;

	updates_per_frame = true;
}

void AnimatedSprite::BuildAnimationArray(int size)
{
	if (animation_sprites != nullptr)
		delete[] animation_sprites;

	animation_sprites = new AtlasChild*[size];
}

void AnimatedSprite::CreateAnimationUsingAtlas(unsigned int indexToStart, unsigned int indexToEnd)
{
	assert(indexToEnd - indexToStart <= (unsigned int)(sprite_atlas->atlas_sprite_total + 1));
	assert(indexToEnd <= (unsigned int)(sprite_atlas->atlas_sprite_total - 1));

	BuildAnimationArray(indexToEnd - indexToStart + 1);

	for (unsigned int i = indexToStart; i <= indexToEnd; i++)
		UseFrame(sprite_atlas->GetSpriteAtIndex(i));

	SetFramerate((float)(indexToEnd - indexToStart + 1) * 1.5f);

}

AnimatedSprite::~AnimatedSprite()
{
	delete[] animation_sprites; 
	delete animation_timer;
}

void AnimatedSprite::UseFrame(const char* image_name)
{
	animation_sprites[total_animated_sprites] = sprite_atlas->GetSprite(image_name);
	total_animated_sprites++;

	active_sprite_index = total_animated_sprites - 1;
}

void AnimatedSprite::UseFrame(AtlasChild* image)
{
	animation_sprites[total_animated_sprites] = image;
	total_animated_sprites++;

	active_sprite_index = total_animated_sprites - 1;
}

void AnimatedSprite::SetFramerate(float frame_rate)
{
	animation_timer->SetDuration(1.0f / frame_rate);
	animation_timer->Start();
}

void AnimatedSprite::NextFrame()
{
	//If we're at the end. Loop back over.
	if (active_sprite_index == total_animated_sprites - 1)
		active_sprite_index = 0;
	else
		active_sprite_index++;

	animation_timer->SetElapsed(0.0f);
}

void AnimatedSprite::Update(float delta)
{
	if (updates_per_frame == true)
	{
		animation_timer->Update(delta);

		if (animation_timer->GetPercentage() == 0.0f)
			NextFrame();
	}
}

AtlasChild* AnimatedSprite::FetchActiveSprite()
{
	return animation_sprites[active_sprite_index];
}
