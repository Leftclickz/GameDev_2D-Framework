#pragma once
#include "Mesh/Mesh.h"


//Generic tile size
static const vec2 TILE_SIZE = vec2(50.0f, 50.0f);

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
	static const MyColor DEFAULT_COLOR = MyColor(255, 255, 255, 255);

	static const VertexFormat TILE_MESH[] = {
		VertexFormat(vec2(0.0f,0.0f),vec2(0.0f,0.0f), DEFAULT_COLOR),
		VertexFormat(vec2(50.0f,0.0f),vec2(1.0f,0.0f), DEFAULT_COLOR),
		VertexFormat(vec2(50.0f,50.0f),vec2(1.0f,1.0f), DEFAULT_COLOR),
		VertexFormat(vec2(0.0f,50.0f),vec2(0.0f,1.0f), DEFAULT_COLOR),
	};

	static const int TILE_VERT_COUNT = 4;
	static const float TILE_LENGTH = 50.0f;
}