#include "GamePCH.h"
#include "../Game/Game.h"

Level::Level(GameCore* game, Mesh* mesh, Mesh* wallMesh, const char* name)
{
	m_pGame = (Game*)game;
	m_TileMesh = mesh;
	m_WallMesh = wallMesh;
	m_BeatTimer = nullptr;

	m_Audio = new Audio(name);
	m_Audio->SetDoesLoop(true);
	m_Audio->Play();
	m_Audio->SetVolume(0.5f);
	
	LoadLevelData(name);
}

Level::~Level()
{
	delete[] m_TileMap;
	m_TileMap = nullptr;

	delete m_BeatTimer;
	m_BeatTimer = nullptr;

	delete m_Audio;
}

void Level::Draw(vec2 camPos, vec2 projScale)
{
	for (int i = level_dimensions - 1; i >= 0; i--)
		m_TileMap[i].Draw(camPos, projScale);
}

void Level::Update(float deltatime)
{

	m_BeatTimer->Update(deltatime);
	if (m_BeatTimer->GetPercentage() == 0.0f)
		AddBeat();
}

Tile* Level::GetTileAtPosition(vec2 position)
{
	int x = (int)(position.x / TILE_SIZE.x);
	int y = (int)(position.y / TILE_SIZE.y);

	int index = x + (LEVEL_TILE_DIMENSIONS.x * y);

	return &m_TileMap[index];
}

Tile* Level::GetTileAtPosition(int tx, int ty)
{
	return &m_TileMap[tx + ty * LEVEL_TILE_DIMENSIONS.x];
}

void Level::AddBeat()
{
	//Swap the floor sprite every baet
	SwapFloorSprite();

	//If the player didnt move during the beat, too bad. They lose their movement and the enemies get a free move.
	if (m_pGame->GetPlayer()->HasMovedThisBeat() == false)
		m_pGame->NextBeat();

	//Set the player moving to false to prep for next beat.
	m_pGame->GetPlayer()->SetMoved(false);
}

void Level::LoadLevelData(const char* name)
{
	//for now we're just drawing a map of nothing but floor. We'd want to load a file here
		m_TileMap = new Tile[level_dimensions];

		//Using a default beat timer for now at 188bpm.
		m_BeatTimer = new Timer(BPM, true);
		m_BeatTimer->Start();

		//std::string animation_name = std::string(name) + "_Variant_";

	for (unsigned int i = 0; i < level_dimensions; i++)
	{
		int d = (i % LEVEL_TILE_DIMENSIONS.x);
		int f = (i / LEVEL_TILE_DIMENSIONS.x);

		float x = (float)d * TILE_SIZE.x;
		float y = (float)f * TILE_SIZE.y;

		if (d % 2 == 0 && f % 2 == 0)
			m_TileMap[i] = Tile(m_pGame, m_TileMesh, "Floor_1_Variant_1");
		else if (d % 2 == 1 && f % 2 == 1)
			m_TileMap[i] = Tile(m_pGame, m_TileMesh, "Floor_1_Variant_1");
		else
			m_TileMap[i] = Tile(m_pGame, m_TileMesh, "Floor_1_Variant_2");

		//std::string variant_name = animation_name;

		//if (d % 2 == 0 && f % 2 == 0)
		//	variant_name += "1";
		//else if (d % 2 == 1 && f % 2 == 1)
		//	variant_name += "1";
		//else
		//	variant_name += "2";

		//m_TileMap[i] = Tile(m_pGame, m_TileMesh, variant_name);

		if (d == 3)
		{
			if (f != 2)
				m_TileMap[i].AddWall(new Wall(m_pGame, m_WallMesh,"Wall_1"));
		}

		m_TileMap[i].SetPosition(i);
	}
}

void Level::SwapFloorSprite()
{
	AnimatedSprite* floor = ImageManager::UseAnimation("Floor_1_Variant_1");
	floor->NextFrame();

	floor = ImageManager::UseAnimation("Floor_1_Variant_2");
	floor->NextFrame();
}
