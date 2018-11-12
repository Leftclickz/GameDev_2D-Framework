#pragma once
#include "Mesh/Mesh.h"


static const ivec2 MAP_SIZE = ivec2(20, 12);
static const vec2 TILE_SIZE = vec2(50.0f, 50.0f);

static const int SCREEN_SIZE_X = MAP_SIZE.x * (int)TILE_SIZE.x;
static const int SCREEN_SIZE_Y = MAP_SIZE.y * (int)TILE_SIZE.y;

static const vec2 HALF_SCREEN = vec2((float)SCREEN_SIZE_X, (float)SCREEN_SIZE_Y) * 0.5f;

static const int LEVEL_SIZE_X = SCREEN_SIZE_X * 2;
static const int LEVEL_SIZE_Y = SCREEN_SIZE_Y * 4;

static const vec2 HALF_LEVEL = vec2((float)LEVEL_SIZE_X, (float)LEVEL_SIZE_Y) * 0.5f;

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