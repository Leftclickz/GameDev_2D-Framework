#pragma once
#include "Mesh/Mesh.h"


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

//Various vectors in world units.
static const vec2 HALF_SCREEN = vec2((float)SCREEN_SIZE_X, (float)SCREEN_SIZE_Y) * 0.5f;
static const vec2 HALF_LEVEL = vec2((float)LEVEL_TILE_DIMENSIONS.x, (float)LEVEL_TILE_DIMENSIONS.y) * 0.5f;

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
		VertexFormat(vec2(50.0f,0.0f),vec2(1.0f,0.0f), WHITE),
		VertexFormat(vec2(50.0f,50.0f),vec2(1.0f,1.0f), WHITE),
		VertexFormat(vec2(0.0f,50.0f),vec2(0.0f,1.0f), WHITE),
	};

	static const VertexFormat WALL_MESH[] = 
	{
		VertexFormat(vec2(0.0f,0.0f),vec2(0.0f,0.0f), WHITE),
		VertexFormat(vec2(50.0f,0.0f),vec2(1.0f,0.0f), WHITE),
		VertexFormat(vec2(50.0f,100.0f),vec2(1.0f,1.0f), WHITE),
		VertexFormat(vec2(0.0f,100.0f),vec2(0.0f,1.0f), WHITE),
	};

	static const int TILE_VERT_COUNT = 4;
	static const float TILE_LENGTH = 50.0f;
}