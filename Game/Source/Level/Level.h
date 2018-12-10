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

	Level(GameCore* game, const char** name, unsigned int index);
	virtual ~Level();

	//game loop
	void Draw();
	void BeginPlay();
	void StopPlay();
	void Update(float deltatime);

	//tile collision checking functions
	TileData* GetTileAtPosition(vec2 position);
	TileData* GetTileAtPosition(int tx, int ty);

	//checks for collisions. returns true for a collision, returns false if no collision
	bool CheckForCollisionsAt(int index, AnimatedObject* ObjectThatCalledThisFunction);

	//destroys a game object that the level is responsible for if it exists
	void DestroyEntity(AnimatedObject* object);

	//beat management functions
	void AddBeat();
	void DoEnemyMoves();
	float GetBPM() { return BPM; }

	//Reset the level
	void ResetLevel();

private:

	//load a level with tiles, enemies etc
	void LoadLevelData(const char** name, unsigned int index);

	//fetch raw map values from file
	const unsigned char* LoadMap(const char** name, unsigned int index);

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