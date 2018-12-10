#pragma once
#include "Mesh/Mesh.h"

class Camera;
class Game;
class ParticleRenderer;

//SHADERS
class SHADERS
{

public:

	static void LoadShaders() 
	{ 
		TEXTURE_SHADER_PROGRAM = new ShaderProgram("Data/Shaders/Moving.vert", "Data/Shaders/Moving.frag");
		DEBUG_SHADER_PROGRAM = new ShaderProgram("Data/Shaders/Color.vert", "Data/Shaders/Color.frag");
		CANVAS_SHADER_PROGRAM = new ShaderProgram("Data/Shaders/Canvas.vert", "Data/Shaders/Canvas.frag");
		PARTICLE_SHADER_PROGRAM = new ShaderProgram("Data/Shaders/Particle.vert", "Data/Shaders/Particle.frag");
	}

	static void DestroyShaders() 
	{
		delete TEXTURE_SHADER_PROGRAM;
		delete DEBUG_SHADER_PROGRAM;
		delete CANVAS_SHADER_PROGRAM;
		delete PARTICLE_SHADER_PROGRAM;
	}

	static ShaderProgram* TEXTURE_SHADER_PROGRAM;
	static ShaderProgram* DEBUG_SHADER_PROGRAM;
	static ShaderProgram* CANVAS_SHADER_PROGRAM;
	static ShaderProgram* PARTICLE_SHADER_PROGRAM;

};

class CAMERA
{

public:
	static void CreateCameras(Game* game);
	static void DestroyCameras();

	static Camera* GetPlayerCamera();

private:

	static Camera* PLAYER_CAMERA;

};

class MESHES
{
public:
	static void CreateDefaultMeshes();
	static void DestroyMeshes();
	static void SetMeshDebugMode(bool value = true);

	static Mesh* TILEMESH_DEFAULT_SIZE;
	static Mesh* TILEMESH_WALL_SIZE;
};

class RENDERER
{
public:
	static void CreateDefaultRenderer();
	static void DestroyRenderer();
	static ParticleRenderer* PARTICLE_RENDERER;
};

#define TEXTURE_SHADER SHADERS::TEXTURE_SHADER_PROGRAM->GetProgram()
#define DEBUG_SHADER  SHADERS::DEBUG_SHADER_PROGRAM->GetProgram()
#define CANVAS_SHADER  SHADERS::CANVAS_SHADER_PROGRAM->GetProgram()
#define PARTICLE_SHADER  SHADERS::CANVAS_SHADER_PROGRAM->GetProgram()

#define PLAYER_CAMERA_POSITION CAMERA::GetPlayerCamera()->GetPosition()
#define PLAYER_CAMERA_PROJECTION CAMERA::GetPlayerCamera()->GetProjection()

#define CANVAS_RENDER_MODE GL_TRIANGLE_STRIP
#define DEBUG_RENDER_MODE GL_LINE_STRIP
#define PARTICLE_RENDER_MODE GL_TRIANGLE_STRIP

//Generic tile size
static const vec2 TILE_SIZE = vec2(50.0f, 50.0f);
static const vec2 WALL_SIZE = vec2(50.0f, 100.0f);

//The amount of visible tile indices on the screen at a time
static const ivec2 SCREEN_TILE_DIMENSIONS = ivec2(20, 12);

//Screen size in world units
static const int SCREEN_SIZE_X = SCREEN_TILE_DIMENSIONS.x * (int)TILE_SIZE.x;
static const int SCREEN_SIZE_Y = SCREEN_TILE_DIMENSIONS.y * (int)TILE_SIZE.y;

//The total tiles in a level
static const ivec2 LEVEL_TILE_DIMENSIONS = ivec2(SCREEN_TILE_DIMENSIONS.x * 2, SCREEN_TILE_DIMENSIONS.y * 3);
static const int LEVEL_DIMENSIONS = 1440;

//Various vectors in world units.
static const vec2 HALF_SCREEN = vec2((float)SCREEN_SIZE_X, (float)SCREEN_SIZE_Y) * 0.5f;
static const vec2 HALF_LEVEL = vec2((float)LEVEL_TILE_DIMENSIONS.x, (float)LEVEL_TILE_DIMENSIONS.y) * 0.5f;

static const vec2 PROJECTION = vec2(1.0f / HALF_SCREEN);


namespace VERSION_CONTROL
{
	static const char* CURRENT_VERSION = "0.01337";
}

namespace TILE
{
	static MyColor WHITE = MyColor(255, 255, 255, 255);
	static MyColor RED = MyColor(255, 0, 0, 255);
	static MyColor GREEN = MyColor(0, 255, 0, 255);
	static MyColor BLUE = MyColor(0, 0, 255, 255); 
	static MyColor PINK = MyColor(255, 20, 147, 255);
	static MyColor CYAN = MyColor(0, 255, 255, 255);

	static const VertexFormat TILE_MESH[] = {
		VertexFormat(vec2(0.0f,0.0f),vec2(0.0f,0.0f), WHITE),
		VertexFormat(vec2(0.0f,50.0f),vec2(0.0f,1.0f), WHITE),
		VertexFormat(vec2(50.0f,0.0f),vec2(1.0f,0.0f), WHITE),
		VertexFormat(vec2(50.0f,50.0f),vec2(1.0f,1.0f), WHITE),
	};

	static const VertexFormat WALL_MESH[] = 
	{
		VertexFormat(vec2(0.0f,0.0f),vec2(0.0f,0.0f), WHITE),
		VertexFormat(vec2(50.0f,0.0f),vec2(1.0f,0.0f), WHITE),
		VertexFormat(vec2(0.0f,100.0f),vec2(0.0f,1.0f), WHITE),
		VertexFormat(vec2(50.0f,100.0f),vec2(1.0f,1.0f), WHITE),
	};

	static const int TILE_VERT_COUNT = 4;
	static const float TILE_LENGTH = 50.0f;
}

namespace ANIMATION_NAMES
{
	//Floor animation names
	static const char* F1_V1 = "Floor_1_Variant_1";
	static const char* F1_V2 = "Floor_1_Variant_2";

	//Enemy animation names
	static const char* SKELE_IDLE = "SkeletonWhite_Idle";
	static const char* SKELE_ATK = "SkeletonWhite_Attacking";
	static const char* SLIME_IDLE = "SlimeGreen_Idle";
	static const char* BAT_IDLE = "Bat_Idle";
	static const char* PLAYER_IDLE = "Player_Idle";
	
	//particle animation
	static const char* DUST = "Dust_Particles";

}

namespace JSON_PARSING_METHOD
{
	static const char* JIMMY = "SpriteTool";
}

namespace TEXTURE_NAMES
{
	//default
	static const char* DEFAULT = "Default";

	//player image
	static const char* PLAYER = "Player";

	//enemy images
	static const char* BAT = "Bat";
	static const char* SKELETON = "SkeletonWhite";
	static const char* SLIME = "SlimeGreen";

	//environment
	static const char* WALL = "Wall_1";
	static const char* FLOOR = "Floor_";

	//font
	static const char* FONT = "Text/DefaultFont_White";

	//hud
	static const char* HEARTS = "HUD";

	//particles
	static const char* DUST = "Particles";
}

namespace AUDIO_NAMES
{
	static const char* FLOOR_1 = "Floor_1";

	//player swings
	static const char* PLAYER_MELEE_1 = "Player_Swing_1";
	static const char* PLAYER_MELEE_2 = "Player_Swing_2";
	static const char* PLAYER_MELEE_3 = "Player_Swing_3";
	static const char* PLAYER_MELEE_4 = "Player_Swing_4";

	//clash sounds
	static const char* SWORD_CLASH = "Sword_Clash";

	//enemy getting hit
	static const char* SLIME_HIT = "Slime_Hit_1";
	static const char* SKELE_HIT = "Skele_Hit_1";
}

namespace LEVEL_DATA
{
	unsigned int TOTAL_ENEMY_VARIANTS = 2;
	unsigned int TOTAL_WALL_VARIANTS = 1;

	const unsigned int TOTAL_LEVELS = 5;

	enum WALL_BITMASK
	{
		NO_WALL = 0,
		WALL = 1
	};

	enum ENEMY_BITMASK
	{
		NO_ENEMIES = 0,
		SLIME = 2,
		SKELETON = 3
	};

	const unsigned int WALL_MASK = 1;
	const unsigned int ENEMY_MASK = 15;

	const ENEMY_BITMASK ENEMIES[2] = { SLIME, SKELETON };
}