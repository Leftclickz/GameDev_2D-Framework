#include "GamePCH.h"

Level::Level(GameCore* game, Mesh* mesh, const char* name)
{
	m_pGame = game;
	m_TileMesh = mesh;
	
	LoadLevelData(name);
}

Level::~Level()
{
	delete[] m_TileMap;
	m_TileMap = nullptr;
}

void Level::Draw(vec2 camPos, vec2 projScale)
{
	for (unsigned int i = 0; i < level_dimensions; i++)
		m_TileMap[i].Draw(camPos, projScale);
}

void Level::LoadLevelData(const char* name)
{
	//for now we're just drawing a map of nothing but floor. We'd want to load a file here
		m_TileMap = new Tile[level_dimensions];

	for (unsigned int i = 0; i < level_dimensions; i++)
	{
		float x = (i % LEVEL_TILE_DIMENSIONS.x) * TILE_SIZE.x;
		float y = (i / LEVEL_TILE_DIMENSIONS.x) * TILE_SIZE.y;

		m_TileMap[i] = Tile(m_pGame, m_TileMesh, "Bomberman");
		m_TileMap[i].SetPosition(vec2(x, y));
	}
}
