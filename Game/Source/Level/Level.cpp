#include "GamePCH.h"
#include "../Game/Game.h"
#include "Level.h"
#include <fstream>
#include <sstream>

Level::Level(GameCore* game, const char** name, unsigned int index)
{
	m_pGame = (Game*)game;
	m_BeatTimer = nullptr;

	//Canvases
	m_Canvas = new Canvas(level_dimensions, ImageManager::UseImage(&TEXTURE_NAMES::FLOOR));
	m_CanvasVariant = new Canvas(level_dimensions, ImageManager::UseImage(&TEXTURE_NAMES::FLOOR));

	//Tilemap
	m_TileMap = new TileData[level_dimensions];

	//enemy vector
	m_EnemyVector = new std::vector<Enemy*>;

	//Test properties
	m_WalkingTile = new TileProperties;
	m_WalkingTile->tile_mesh = MESHES::TILEMESH_DEFAULT_SIZE;

	//Set level audio
	m_Audio = AudioManager::GetAudio( &AUDIO_NAMES::FLOOR_1);
	m_Audio->SetDoesLoop(true);
	m_Audio->SetVolume(0.15f);
	
	//Load the tile data
	LoadLevelData(name, index);

	//create both canvas variants
	CreateCanvas(m_Canvas);
	SwapFloorSprite();
	CreateCanvas(m_CanvasVariant);

	//Set canvas 1 active
	m_ActiveCanvas = m_Canvas;
}

Level::~Level()
{
	//delete the tilemap
	delete[] m_TileMap;

	//delete the beat timer
	delete m_BeatTimer;

	//delete tile properties
	delete m_WalkingTile;

	//delete canvases
	delete m_Canvas;
	delete m_CanvasVariant;

	//delete all enemies
	for (unsigned int i = 0; i < m_EnemyVector->size(); i++)
		if(m_EnemyVector->at(i) != nullptr)
			delete m_EnemyVector->at(i);
	delete m_EnemyVector;
}

void Level::Draw()
{
	//Draw the tile map
	m_ActiveCanvas->DrawCanvas();

	//We check tiles for walls and then draw those too.
	for (int i = level_dimensions - 1; i >= 0; i--)
		if (m_TileMap[i].m_Wall != nullptr)
			m_TileMap[i].m_Wall->Draw();

	//Draw game objects.
	for (unsigned int i = 0; i < m_EnemyVector->size(); i++)
		if(m_EnemyVector->at(i) != nullptr)
			m_EnemyVector->at(i)->Draw();
}

void Level::BeginPlay()
{
	m_Audio->Play();
}

void Level::StopPlay()
{
	m_Audio->Stop();
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

bool Level::CheckForCollisionsAt(int index, AnimatedObject* ObjectThatCalledThis)
{
	bool loop_complete = false;
	int i = 0;

	if (m_EnemyVector->size() == 0)
		return false;

	//check if any enemies collide with the index given.
	while (loop_complete == false)
	{
		if (m_EnemyVector->at(i) != nullptr)
		{
			//check for collisions and do damage if the object calling the function is a player (enemies dont hurt each other)
			if (m_EnemyVector->at(i)->GetPositionByIndex() == index)
			{
				if (ObjectThatCalledThis->GetType() == PLAYER)
					m_EnemyVector->at(i)->TakeDamage(ObjectThatCalledThis->GetDamageCount());

				return true;
			}
		}
		//increment
		i++;

		//escape clause
		if (i == m_EnemyVector->size())
			loop_complete = true;
	}

	//check if the player will be collided on (this part only runs if an enemy calls the function)
	if (ObjectThatCalledThis->GetType() == ENEMY)
	{
		if (m_pGame->GetPlayer()->GetPositionByIndex() == index)
		{
			m_pGame->GetPlayer()->TakeDamage(ObjectThatCalledThis->GetDamageCount());
			return true;
		}
	}

	return false;
}

void Level::DestroyEntity(AnimatedObject * object)
{
	//search for the entity to destroy
	for (unsigned int i = 0; i < m_EnemyVector->size(); i++)
	{
		if (m_EnemyVector->at(i) != nullptr)
		{
			if (object->GetPositionByIndex() == m_EnemyVector->at(i)->GetPositionByIndex())
			{
				//increase player score then destory the enemy
				m_pGame->IncreaseScore(m_EnemyVector->at(i)->GetScore());
				delete m_EnemyVector->at(i);
				m_EnemyVector->at(i) = nullptr;
			}
		}
	}

	//if there are no enemies move to the next floor
	if (m_EnemyVector->size() == 0)
	{
		StopPlay();
		m_pGame->SetNextLevel();
	}

	bool completed = true;

	//check if all enemies are nullptrs (2nd safe check)
	for (unsigned int i = 0; i < m_EnemyVector->size(); i++)
		if (m_EnemyVector->at(i) != nullptr)
			completed = false;

	//if we're out of here then stop the music and set the next level
	if (completed)
	{
		StopPlay();
		m_pGame->SetNextLevel();
	}
}

void Level::AddBeat()
{
	//Swap the canvas every baet
	SwapCanvas();

	//If the player didnt move during the beat, too bad. They lose their movement and the enemies get a free move.
	if (m_pGame->GetPlayer()->HasMovedThisBeat() == false)
		DoEnemyMoves();

	//Set the player moving to false to prep for next beat.
	m_pGame->GetPlayer()->SetMoved(false);
}

void Level::DoEnemyMoves()
{
	for (unsigned int i = 0; i < m_EnemyVector->size(); i++)
	{
		if (m_EnemyVector->at(i) != nullptr)
			m_EnemyVector->at(i)->DoNextMove();
	}

	if (m_pGame->GetPlayer()->GetHealth() <= 0.0f)
		m_pGame->ResetLevel();
}


void Level::LoadLevelData(const char** name, unsigned int index)
{
	//for now we're just drawing a map of nothing but floor. We'd want to load a file here
		//m_TileMap = new Tile*[level_dimensions];

		//Using a default beat timer for now at 188bpm.
	m_BeatTimer = new Timer(BPM, true);
	m_BeatTimer->Start();


	//copy data from file
	const unsigned char* BASE_MAP = LoadMap(name, index);

	//copy level data into temporary space	
	unsigned char levelData[LEVEL_DIMENSIONS];
	for (unsigned int i = 0; i < level_dimensions; i++)
		levelData[i] = BASE_MAP[i];

	//delete the old one
	delete[] BASE_MAP;

	//flip rows
	for (int row = 0; row < LEVEL_TILE_DIMENSIONS.y / 2; row++)
	{
		int offset = LEVEL_TILE_DIMENSIONS.y - 1 - row;
		for (int column = 0; column < LEVEL_TILE_DIMENSIONS.x; column++)
		{
			unsigned char* a = &levelData[column + (row * LEVEL_TILE_DIMENSIONS.x)];
			unsigned char* b = &levelData[column + (offset * LEVEL_TILE_DIMENSIONS.x)];
			unsigned char temp = *a;
			*a = *b;
			*b = temp;
		}
	}

	//get our tile variations
	AtlasChild** variant_one = ImageManager::UseAnimation(&ANIMATION_NAMES::F1_V1)->FetchActiveSprite();
	AtlasChild** variant_two = ImageManager::UseAnimation(&ANIMATION_NAMES::F1_V1)->FetchActiveSprite();

	//and the base sprite involved
	Sprite* atlas = ImageManager::UseImage(&TEXTURE_NAMES::FLOOR);

	for (unsigned int i = 0; i < level_dimensions; i++)
	{
		int d = (i % LEVEL_TILE_DIMENSIONS.x);
		int f = (i / LEVEL_TILE_DIMENSIONS.x);

		//creates a checkerboard pattern of lit/unlit tiles
		if (d % 2 == 0 && f % 2 == 0)
			m_TileMap[i] = TileData(m_WalkingTile, i, VARIANT_ONE);
		else if (d % 2 == 1 && f % 2 == 1)
			m_TileMap[i] = TileData(m_WalkingTile, i, VARIANT_ONE);
		else
			m_TileMap[i] = TileData(m_WalkingTile, i, VARIANT_TWO);

		//bitmasking for walls
		if ((levelData[i] & LEVEL_DATA::WALL_MASK) == LEVEL_DATA::WALL)
		{
			m_TileMap[i].m_Wall = new Wall(m_pGame, MESHES::TILEMESH_WALL_SIZE, &TEXTURE_NAMES::WALL);
			m_TileMap[i].m_Wall->SetPosition(i);
		}

		//bitmasking for enemies
		int enemy = levelData[i] & LEVEL_DATA::ENEMY_MASK;

		switch (enemy)
		{
		case LEVEL_DATA::SLIME:
			m_EnemyVector->push_back(new Slime(m_pGame, MESHES::TILEMESH_DEFAULT_SIZE));
			m_EnemyVector->at(m_EnemyVector->size() - 1)->SetPosition(i);
			break;
		case LEVEL_DATA::SKELETON:
			m_EnemyVector->push_back(new Skeleton(m_pGame, MESHES::TILEMESH_DEFAULT_SIZE));
			m_EnemyVector->at(m_EnemyVector->size() - 1)->SetPosition(i);
			break;
		}
	}
}

const unsigned char* Level::LoadMap(const char ** name, unsigned int index)
{
	//prepare to read
	std::ifstream reader;
	unsigned char* map_data = new unsigned char[LEVEL_DIMENSIONS];

	std::string path = "Data/Level_Maps/" + std::string(*name) + std::to_string(index) + ".txt";
	reader.open(path, std::ifstream::in);

	unsigned int i = 0;

	//read all lines of data
	for (int x = 0; x < LEVEL_TILE_DIMENSIONS.y; x++)
	{
		std::string input, segment;

		//read next line
		getline(reader, input);

		//parse each piece of data delimited by ,
		std::stringstream input_stream(input);

		while (getline(input_stream, segment, ',') && i < level_dimensions)
		{
			map_data[i] = (unsigned char)strtoul(segment.c_str(), NULL, 0);
			i++;
		}
	}

	//return our raw map data
	return map_data;
}

void Level::SwapFloorSprite()
{
	AnimatedSprite* floor = ImageManager::UseAnimation(&ANIMATION_NAMES::F1_V1);
	floor->NextFrame();

	floor = ImageManager::UseAnimation(&ANIMATION_NAMES::F1_V2);
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
			canvas->AddVerts(&m_TileMap[i]);
		else
		{
			//Our index is 1 row higher here.
			int index =  ((i / LEVEL_TILE_DIMENSIONS.x) + 1) * LEVEL_TILE_DIMENSIONS.x;

			//Our offset will be taking away the 40s from i + 1 (so we stay on the same row).
			int offset = (i - (i / LEVEL_TILE_DIMENSIONS.x * LEVEL_TILE_DIMENSIONS.x) + 1);

			//Get our verts. Backward direction drawing.
			canvas->AddVerts(&m_TileMap[index - offset], true);
		}
	}

	//Generate a new VBO.
	canvas->GenerateCanvas();
}
