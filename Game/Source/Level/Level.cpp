#include "GamePCH.h"
#include "../Game/Game.h"

Level::Level(GameCore* game, Mesh* mesh, Mesh* wallMesh, const char* name)
{
	m_pGame = (Game*)game;
	m_TileMesh = mesh;
	m_WallMesh = wallMesh;
	m_BeatTimer = nullptr;

	m_Canvas = new Canvas(level_dimensions, ImageManager::UseImage("Floor_1"));
	m_CanvasVariant = new Canvas(level_dimensions, ImageManager::UseImage("Floor_1"));
	m_Canvas->SetDrawDebugLines();
	m_CanvasVariant->SetDrawDebugLines();

	m_TileMap = new TileData[level_dimensions];

	m_WalkingTile = new TileProperties;
	m_WalkingTile->tile_mesh = mesh;
	m_WalkingTile->atlas_image = ImageManager::UseImage("Floor_1");

	m_Audio = new Audio(name);
	m_Audio->SetDoesLoop(true);
	m_Audio->Play();
	m_Audio->SetVolume(0.5f);
	
	LoadLevelData(name);

	CreateCanvas(m_Canvas);
	SwapFloorSprite();
	CreateCanvas(m_CanvasVariant);

	m_ActiveCanvas = m_Canvas;
}

Level::~Level()
{
	//for (unsigned int i = 0; i < level_dimensions; i++)
	//	delete m_TileMap[i];

	delete[] m_TileMap;
	m_TileMap = nullptr;

	delete m_BeatTimer;
	m_BeatTimer = nullptr;

	delete m_Audio;
	delete m_WalkingTile;

	delete m_Canvas;
	delete m_CanvasVariant;
}

void Level::Draw()
{
	//Draw the tile map
	m_ActiveCanvas->DrawCanvas();

	//We check tiles for walls and then draw those too.
	for (int i = level_dimensions - 1; i >= 0; i--)
		if (m_TileMap[i].m_Wall != nullptr)
			m_TileMap[i].m_Wall->Draw();
}

void Level::Update(float deltatime)
{
	m_BeatTimer->Update(deltatime);
	if (m_BeatTimer->GetPercentage() == 0.0f)
		AddBeat();
}

TileData* Level::GetTileAtPosition(vec2 position)
{
	int x = (int)(position.x / TILE_SIZE.x);
	int y = (int)(position.y / TILE_SIZE.y);

	int index = x + (LEVEL_TILE_DIMENSIONS.x * y);

	return &m_TileMap[index];
}

TileData* Level::GetTileAtPosition(int tx, int ty)
{
	return &m_TileMap[tx + ty * LEVEL_TILE_DIMENSIONS.x];
}

void Level::AddBeat()
{
	//Swap the canvas every baet
	SwapCanvas();

	//If the player didnt move during the beat, too bad. They lose their movement and the enemies get a free move.
	if (m_pGame->GetPlayer()->HasMovedThisBeat() == false)
		m_pGame->NextBeat();

	//Set the player moving to false to prep for next beat.
	m_pGame->GetPlayer()->SetMoved(false);
}

void Level::LoadLevelData(const char* name)
{
	//for now we're just drawing a map of nothing but floor. We'd want to load a file here
		//m_TileMap = new Tile*[level_dimensions];

		//Using a default beat timer for now at 188bpm.
		m_BeatTimer = new Timer(BPM, true);
		m_BeatTimer->Start();

		//std::string animation_name = std::string(name) + "_Variant_";

		AtlasChild** variant_one = ImageManager::UseAnimation("Floor_1_Variant_1")->FetchActiveSprite();
		AtlasChild** variant_two = ImageManager::UseAnimation("Floor_1_Variant_2")->FetchActiveSprite();

		Sprite* atlas = ImageManager::UseImage("Floor_1");

	for (unsigned int i = 0; i < level_dimensions; i++)
	{
		int d = (i % LEVEL_TILE_DIMENSIONS.x);
		int f = (i / LEVEL_TILE_DIMENSIONS.x);

		float x = (float)d * TILE_SIZE.x;
		float y = (float)f * TILE_SIZE.y;

		//if (d % 2 == 0 && f % 2 == 0)
		//	m_TileMap[i] = new Tile(m_pGame, m_TileMesh, "Floor_1_Variant_1");
		//else if (d % 2 == 1 && f % 2 == 1)
		//	m_TileMap[i] = new Tile(m_pGame, m_TileMesh, "Floor_1_Variant_1");
		//else
		//	m_TileMap[i] = new Tile(m_pGame, m_TileMesh, "Floor_1_Variant_2");

		if (d % 2 == 0 && f % 2 == 0)
			m_TileMap[i] = TileData(vec2(x, y), variant_one);
		else if (d % 2 == 1 && f % 2 == 1)
			m_TileMap[i] = TileData(vec2(x, y), variant_one);
		else
			m_TileMap[i] = TileData(vec2(x, y), variant_two);

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
			{
				m_TileMap[i].m_Wall = new Wall(m_pGame, m_WallMesh, "Wall_1");
				m_TileMap[i].m_Wall->SetPosition(i);
			}
		}
		
		//m_TileMap[i]->SetPosition(i);
	}
}

void Level::SwapFloorSprite()
{
	AnimatedSprite* floor = ImageManager::UseAnimation("Floor_1_Variant_1");
	floor->NextFrame();

	floor = ImageManager::UseAnimation("Floor_1_Variant_2");
	floor->NextFrame();
}

void Level::SwapCanvas()
{
	if (m_ActiveCanvas == m_Canvas)
		m_ActiveCanvas = m_CanvasVariant;
	else
		m_ActiveCanvas = m_Canvas;
}

void Level::CreateCanvas(Canvas* canvas)
{
	//Clear in case VBO exists.
	canvas->Clear();

	//Iterate through all tiles and create a singular. We're drawing in a S pattern going upwards.
	for (unsigned int i = 0; i < level_dimensions; i++)
	{
		if (i / LEVEL_TILE_DIMENSIONS.x % 2 == 0)
			//Get our verts. Forward direction drawing.
			canvas->AddVerts(&m_TileMap[i], m_WalkingTile);
		else
		{
			//Our index is 1 row higher here.
			int index =  ((i / LEVEL_TILE_DIMENSIONS.x) + 1) * LEVEL_TILE_DIMENSIONS.x;

			//Our offset will be taking away the 40s from i + 1 (so we stay on the same row).
			int offset = (i - (i / LEVEL_TILE_DIMENSIONS.x * LEVEL_TILE_DIMENSIONS.x) + 1);

			//Get our verts. Backward direction drawing.
			canvas->AddVerts(&m_TileMap[index - offset], m_WalkingTile, true);
		}
	}

	//Generate a new VBO.
	canvas->GenerateCanvas();
}
