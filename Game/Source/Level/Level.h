#pragma once

class GameCore;
class Mesh;
class Tile;

class Level
{

public:

	Level(GameCore* game, Mesh* mesh, const char* name);
	virtual ~Level();

	void Draw(vec2 camPos, vec2 projScale);

private:

	void LoadLevelData(const char* name);

	Tile* m_TileMap;
	GameCore* m_pGame;
	Mesh* m_TileMesh;

	const unsigned int level_dimensions = LEVEL_TILE_DIMENSIONS.x * LEVEL_TILE_DIMENSIONS.y;

};