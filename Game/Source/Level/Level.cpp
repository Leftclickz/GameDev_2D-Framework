#include "GamePCH.h"
#include "../Game/Game.h"
#include "Level.h"

Level::Level(GameCore* game, const char** name)
{
	m_pGame = (Game*)game;
	m_BeatTimer = nullptr;

	//Canvases
	m_Canvas = new Canvas(level_dimensions, ImageManager::UseImage(&TEXTURE_NAMES::FLOOR));
	m_CanvasVariant = new Canvas(level_dimensions, ImageManager::UseImage(&TEXTURE_NAMES::FLOOR));
	//m_Canvas->SetDrawDebugLines();
	//m_CanvasVariant->SetDrawDebugLines();

	//Tilemap
	m_TileMap = new TileData[level_dimensions];

	//enemy vector
	m_EnemyVector = new std::vector<Enemy*>;

	//Test properties
	m_WalkingTile = new TileProperties;
	m_WalkingTile->tile_mesh = MESHES::TILEMESH_DEFAULT_SIZE;

	//Set level audio
	m_Audio = AudioManager::GetAudio(&AUDIO_NAMES::FLOOR_1);
	m_Audio->SetDoesLoop(true);
	m_Audio->SetVolume(0.15f);
	m_Audio->Play();
	
	//Load the tile data
	LoadLevelData(name);

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

	for (unsigned int i = 0; i < m_EnemyVector->size(); i++)
	{
		if (m_EnemyVector->at(i) != nullptr)
		{
			if (object->GetPositionByIndex() == m_EnemyVector->at(i)->GetPositionByIndex())
			{
				delete m_EnemyVector->at(i);
				m_EnemyVector->at(i) = nullptr;
			}
		}
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
}

void Level::LoadLevelData(const char** name)
{
	//for now we're just drawing a map of nothing but floor. We'd want to load a file here
		//m_TileMap = new Tile*[level_dimensions];

		//Using a default beat timer for now at 188bpm.
		m_BeatTimer = new Timer(BPM, true);
		m_BeatTimer->Start();

		//std::string animation_name = std::string(name) + "_Variant_";

		AtlasChild** variant_one = ImageManager::UseAnimation(&ANIMATION_NAMES::F1_V1)->FetchActiveSprite();
		AtlasChild** variant_two = ImageManager::UseAnimation(&ANIMATION_NAMES::F1_V1)->FetchActiveSprite();

		Sprite* atlas = ImageManager::UseImage(&TEXTURE_NAMES::FLOOR);

	for (unsigned int i = 0; i < level_dimensions; i++)
	{
		int d = (i % LEVEL_TILE_DIMENSIONS.x);
		int f = (i / LEVEL_TILE_DIMENSIONS.x);

		//if (d % 2 == 0 && f % 2 == 0)
		//	m_TileMap[i] = new Tile(m_pGame, m_TileMesh, "Floor_1_Variant_1");
		//else if (d % 2 == 1 && f % 2 == 1)
		//	m_TileMap[i] = new Tile(m_pGame, m_TileMesh, "Floor_1_Variant_1");
		//else
		//	m_TileMap[i] = new Tile(m_pGame, m_TileMesh, "Floor_1_Variant_2");

		if (d % 2 == 0 && f % 2 == 0)
			m_TileMap[i] = TileData(m_WalkingTile, i, VARIANT_ONE);
		else if (d % 2 == 1 && f % 2 == 1)
			m_TileMap[i] = TileData(m_WalkingTile, i, VARIANT_ONE);
		else
			m_TileMap[i] = TileData(m_WalkingTile, i, VARIANT_TWO);

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
				m_TileMap[i].m_Wall = new Wall(m_pGame, MESHES::TILEMESH_WALL_SIZE, &TEXTURE_NAMES::WALL);
				m_TileMap[i].m_Wall->SetPosition(i);
			}
		}
		
		//m_TileMap[i]->SetPosition(i);
	}


	m_EnemyVector->push_back(new Skeleton(m_pGame, MESHES::TILEMESH_DEFAULT_SIZE));
	m_EnemyVector->at(0)->SetPosition(42);

	m_EnemyVector->push_back(new Slime(m_pGame, MESHES::TILEMESH_DEFAULT_SIZE));
	m_EnemyVector->at(1)->SetPosition(47);

	m_EnemyVector->push_back(new Slime(m_pGame, MESHES::TILEMESH_DEFAULT_SIZE));
	m_EnemyVector->at(2)->SetPosition(48);

	m_EnemyVector->push_back(new Slime(m_pGame, MESHES::TILEMESH_DEFAULT_SIZE));
	m_EnemyVector->at(3)->SetPosition(49);

	m_EnemyVector->push_back(new Slime(m_pGame, MESHES::TILEMESH_DEFAULT_SIZE));
	m_EnemyVector->at(4)->SetPosition(50);
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
