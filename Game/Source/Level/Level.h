#pragma once

class Game;
class Mesh;
class Tile;
class Timer;
class Audio;

class Level
{

public:

	Level(GameCore* game, Mesh* mesh, Mesh* wallMesh, const char* name);
	virtual ~Level();

	void Draw();
	void Update(float deltatime);

	Tile* GetTileAtPosition(vec2 position);
	Tile* GetTileAtPosition(int tx, int ty);

	void AddBeat();
	float GetBPM() { return BPM; }

private:

	void LoadLevelData(const char* name);
	void SwapFloorSprite();

	void CreateCanvas();

	Tile** m_TileMap;
	Game* m_pGame;
	Mesh* m_TileMesh;
	Mesh* m_WallMesh;
	Audio* m_Audio;

	Timer* m_BeatTimer;

	Canvas* m_Canvas;

	const unsigned int level_dimensions = LEVEL_TILE_DIMENSIONS.x * LEVEL_TILE_DIMENSIONS.y;
	const float BPM = 60.0f / 130.0f;

};