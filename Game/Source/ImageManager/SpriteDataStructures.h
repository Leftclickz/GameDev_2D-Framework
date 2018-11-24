#pragma once

#include "Timer/Timer.h"

struct Sprite
{
	const char* texture_name;
	GLuint GL_texture_index;
	unsigned int TU_index;


	Sprite() : texture_name("failure to load"), GL_texture_index(-1), TU_index(-1) {}
	Sprite(const char* NAME, GLuint INDEX, unsigned int ITEX) : texture_name(NAME), GL_texture_index(INDEX), TU_index(ITEX) {}
};

struct AtlasChild
{
	std::string name;
	vec2 sprite_UV_Scale;
	vec2 sprite_UV_Offset;
};

struct SpriteAtlas
{

	//Atlas size
	vec2 atlas_size;

	//Atlas spritesheet
	Sprite* atlas_image;

	//list of sprites
	AtlasChild* atlas_sprites;
	int atlas_sprite_total;

	SpriteAtlas() {}
	SpriteAtlas(Sprite* ATLAS) : atlas_image(ATLAS), atlas_sprite_total(0) {}
	virtual ~SpriteAtlas();

	//Generate an image using Atlas data. Requires the name of a sprite inside the image. Omit file endings.
	virtual AtlasChild* GetSprite(const char* image_name);

	//Generate an image using Atlas index. Requires the specific index of a sprite indiex the image.
	virtual AtlasChild* GetSpriteAtIndex(unsigned int index);

};

class AnimatedSprite
{

public:

	AnimatedSprite(const char* name, const char* atlas_name);
	AnimatedSprite() {}
	virtual ~AnimatedSprite();

	//Attach a frame of the owned sprite atlas to the animated sequence.
	virtual void UseFrame(const char* image_name);

	//Attach a frame of any sprite atlas child to the animation sequence.
	virtual void UseFrame(AtlasChild* image);

	//Set how many frames are run per second.
	void SetFramerate(float frame_rate);

	//Manually forces the animation to go to the next frame.
	void NextFrame();

	//Sets whether or not the animation auto-updates with game.
	void SetAnimatedWithUpdates(bool value) { updates_per_frame = value; }

	//Build the container to contain the sprites
	void BuildAnimationArray(int size);

	//Automatic animation creation using sprite atlas index values.
	void CreateAnimationUsingAtlas(unsigned int indexToStart, unsigned int indexToEnd);

	//Update to make animations happen
	void Update(float delta);

	//Fetch active sprite to draw
	AtlasChild* FetchActiveSprite();

	//the name of our animation
	const char* animation_name;

	//Sprite atlas this animation is using
	SpriteAtlas* sprite_atlas;


private:

	//Animation timer
	Timer* animation_timer;

	//Currently active sprite index
	unsigned int active_sprite_index;
	
	//Controls whether the animation updates with time or manually
	bool updates_per_frame;

	//List of sprites in the animation sequence
	AtlasChild** animation_sprites;
	unsigned int total_animated_sprites;
};