#pragma once

class Game;
class GameObject;
class Mesh;
class Tile;
class Timer;
class Audio;
struct TileData;
struct TileProperties;

class Level
{

public:

	Level(GameCore* game, const char** name);
	virtual ~Level();

	void Draw();
	void Update(float deltatime);

	TileData* GetTileAtPosition(vec2 position);
	TileData* GetTileAtPosition(int tx, int ty);

	//checks for collisions. returns true for a collision, returns false if no collision
	bool CheckForCollisionsAt(int index, AnimatedObject* ObjectThatCalledThisFunction);

	void DestroyEntity(AnimatedObject* object);

	void AddBeat();
	void DoEnemyMoves();
	float GetBPM() { return BPM; }

private:

	void LoadLevelData(const char** name);
	void SwapFloorSprite();
	void SwapCanvas();

	void CreateCanvas(Canvas* canvas);

	TileData* m_TileMap;
	std::vector<Enemy*>* m_EnemyVector;

	TileProperties* m_WalkingTile;

	Game* m_pGame;
	Mesh* m_TileMesh;
	Mesh* m_WallMesh;
	Audio* m_Audio;

	Timer* m_BeatTimer;

	Canvas* m_Canvas;
	Canvas* m_CanvasVariant;
	Canvas* m_ActiveCanvas;

	const unsigned int level_dimensions = LEVEL_TILE_DIMENSIONS.x * LEVEL_TILE_DIMENSIONS.y;
	const float BPM = 60.0f / 130.0f;

};