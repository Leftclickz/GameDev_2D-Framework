#include "GamePCH.h"
#include <thread>
#include "Game.h"
#include <fstream>

Game::Game(Framework* pFramework)
: GameCore( pFramework, new EventManager() )
{
    m_TextureShader = 0;
	m_DebugShader = 0;

    m_pPlayer = 0;

    m_pPlayerController = 0;
	m_AI = 0;

	RESETTING_LEVEL = false;
}

Game::~Game()
{
    delete m_pPlayerController;

    delete m_pPlayer;
	delete m_ScoreText;

	delete m_MeshTile;
	delete m_WallMesh;
	delete m_VersionText;
	delete m_TestSprite;

	for (unsigned int i = 0; i < LEVEL_DATA::TOTAL_LEVELS; i++)
		delete m_TestLevel[i];
	delete[] m_TestLevel;

	SHADERS::DestroyShaders();
	RENDERER::DestroyRenderer();
	CAMERA::DestroyCameras();
	MESHES::DestroyMeshes();

	delete m_AI;

	ImageManager::Release();
	AudioManager::Release();
}

void Game::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    // Set OpenGL to draw to the entire window.
    glViewport( 0, 0, width, height );
}

void Game::LoadContent()
{
#if WIN32
	// Turn on v-sync.
	wglSwapInterval(1);
#endif

	// Turn on depth buffer testing.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Turn on alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create our shaders.
	SHADERS::LoadShaders();

	// Load meshes
	MESHES::CreateDefaultMeshes();

	//create our audio manager and load our data
	{
		//Initialize sounds
		AudioManager::Initialize();
		AudioManager::Reserve();

		//Load audio
		AudioManager::LoadFromPath(AUDIO_NAMES::FLOOR_1);
		AudioManager::LoadFromPath(AUDIO_NAMES::PLAYER_MELEE_1);
		AudioManager::LoadFromPath(AUDIO_NAMES::PLAYER_MELEE_2);
		AudioManager::LoadFromPath(AUDIO_NAMES::PLAYER_MELEE_3);
		AudioManager::LoadFromPath(AUDIO_NAMES::PLAYER_MELEE_4);
		AudioManager::LoadFromPath(AUDIO_NAMES::SLIME_HIT);
		AudioManager::LoadFromPath(AUDIO_NAMES::SKELE_HIT);
		AudioManager::LoadFromPath(AUDIO_NAMES::SWORD_CLASH);

		//create sound voices
		AudioManager::CreateAudio(&AUDIO_NAMES::FLOOR_1);
		AudioManager::CreateAudio(&AUDIO_NAMES::PLAYER_MELEE_1);
		AudioManager::CreateAudio(&AUDIO_NAMES::PLAYER_MELEE_2);
		AudioManager::CreateAudio(&AUDIO_NAMES::PLAYER_MELEE_3);
		AudioManager::CreateAudio(&AUDIO_NAMES::PLAYER_MELEE_4);
		AudioManager::CreateAudio(&AUDIO_NAMES::SLIME_HIT);
		AudioManager::CreateAudio(&AUDIO_NAMES::SKELE_HIT);
		AudioManager::CreateAudio(&AUDIO_NAMES::SWORD_CLASH);
	}

	//Create our image manager and load our data
	{
		//Initialize image manager and reserve some space
		ImageManager::Initialize();
		ImageManager::Reserve(10);

		//Load some images
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::PLAYER, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::BAT, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::SLIME, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::SKELETON, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::FLOOR, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::WALL, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::FONT, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::HEARTS, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageAtlas(&TEXTURE_NAMES::DUST, &JSON_PARSING_METHOD::JIMMY);
		ImageManager::LoadImageData(&TEXTURE_NAMES::DEFAULT);
	}

	// Load our particle renderer
	RENDERER::CreateDefaultRenderer();
	RENDERER::PARTICLE_RENDERER->AddSprite(ImageManager::UseImage(&TEXTURE_NAMES::DUST));

	//Create animations
	{
		//create player animation
		AnimatedSprite* animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::PLAYER_IDLE, &TEXTURE_NAMES::PLAYER);
		animation->CreateAnimationUsingAtlas(0, 3);

		//create bat animation
		animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::BAT_IDLE, &TEXTURE_NAMES::BAT);
		animation->CreateAnimationUsingAtlas(0, 3);

		//create player animation
		animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::SLIME_IDLE, &TEXTURE_NAMES::SLIME);
		animation->CreateAnimationUsingAtlas(0, 3);

		//create skeleton attacking animation
		animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::SKELE_ATK, &TEXTURE_NAMES::SKELETON);
		animation->CreateAnimationUsingAtlas(0, 3);

		//create skeleton idle animation
		animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::SKELE_IDLE, &TEXTURE_NAMES::SKELETON);
		animation->CreateAnimationUsingAtlas(4, 7);

		//create dust particle animation
		animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::DUST, &TEXTURE_NAMES::DUST);
		animation->CreateAnimationUsingAtlas(0, 6);

		//create floor animation
		animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::F1_V1, &TEXTURE_NAMES::FLOOR);
		animation->CreateAnimationUsingAtlas(0, 1);
		animation->SetAnimatedWithUpdates(false);

		//create floor 2 animation
		animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::F1_V2, &TEXTURE_NAMES::FLOOR);
		animation->CreateAnimationUsingAtlas(2, 3);
		animation->SetAnimatedWithUpdates(false);
		animation->NextFrame();
	}

	//Create mesh
	m_MeshTile = new Mesh(TILE::TILE_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
	//m_MeshTile->SetDrawDebugLines(true);
	m_WallMesh = new Mesh(TILE::WALL_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
	//m_WallMesh->SetDrawDebugLines(true);

    // Create our player.
    m_pPlayer = new Player( this, MESHES::TILEMESH_DEFAULT_SIZE);

	//Create our cameras
	CAMERA::CreateCameras(this);

	//Create our level
	m_TestLevel = new Level*[LEVEL_DATA::TOTAL_LEVELS];
	for (unsigned int i = 0; i < LEVEL_DATA::TOTAL_LEVELS; i++)
		m_TestLevel[i] = new Level(this, &TEXTURE_NAMES::FLOOR, i + 1);
	
	//Create AI
	m_AI = new AI_Patterns(this);

	//score text
	m_ScoreText = new HUD_ScoreDisplay(this, new TextMesh());
	m_ScoreText->SetText("SCORE: ");

	//version text
	m_VersionText = new HUD_Text(this, new TextMesh());
	m_VersionText->SetText("VERSION: " + std::string(VERSION_CONTROL::CURRENT_VERSION));
	m_VersionText->SetAllignment(ALLIGNMENT_CENTER);
	m_VersionText->SetPosition(vec2((float)(SCREEN_SIZE_X / 2),(float) (SCREEN_SIZE_Y - 50.0f)));

	//Testing sprite hud
	m_TestSprite = new HUD_Sprite(this, MESHES::TILEMESH_DEFAULT_SIZE, &TEXTURE_NAMES::HEARTS);
	m_TestSprite->SetPosition(vec2(19.0f * TILE_SIZE.x, 11.0f * TILE_SIZE.y));

    // Assign our controller.
    m_pPlayerController = new PlayerController(m_pPlayer);
    m_pPlayer->SetPlayerController( m_pPlayerController );

	//Load file or use default values
	if (AttemptToLoadSave() == false)
	{
		LEVEL_INDEX = 0;
		m_pPlayer->SetHealth(3.0f);
		m_ScoreText->SetScore(0.0f);
	}

	//Set AI's pathfinding
	m_AI->SetLevel(m_TestLevel[LEVEL_INDEX]);

	//Start our level
	m_TestLevel[LEVEL_INDEX]->BeginPlay();

	//we always start here for some reason
	m_pPlayer->SetPosition(170);

	//score text
	m_ScoreText->SetPosition(0);

    CheckForGLErrors();
}

bool Game::AttemptToLoadSave()
{
	//Load file, put into cJSON object and delete the old array.
	std::string filepath = "Data/Save/Save.json";
	long length = 0;
	char* loaded_file = LoadCompleteFile(filepath.c_str(), &length);
	cJSON* root = cJSON_Parse(loaded_file);
	delete[] loaded_file;

	//get out of here if we failed to load anything
	if (length == 0)
		return false;

	//used saved values to set the game state forward
	m_pPlayer->SetHealth((float)cJSON_GetObjectItem(root, "health")->valuedouble);
	LEVEL_INDEX = cJSON_GetObjectItem(root, "level")->valueint;
	m_ScoreText->SetScore((float)cJSON_GetObjectItem(root, "score")->valuedouble);

	return true;
}


void Game::OnEvent(Event* pEvent)
{
    m_pPlayerController->OnEvent( pEvent );
}

void Game::Update(float deltatime)
{
	if (RESETTING_LEVEL == true)
	{
		//extremely bad way to restart a level
		m_TestLevel[LEVEL_INDEX]->StopPlay();
		delete m_TestLevel[LEVEL_INDEX];

		//Recreate the level
		m_TestLevel[LEVEL_INDEX] = new Level(this, &TEXTURE_NAMES::FLOOR, LEVEL_INDEX + 1);
		m_AI->SetLevel(m_TestLevel[LEVEL_INDEX]);
		m_TestLevel[LEVEL_INDEX]->BeginPlay();

		//Reset positions
		m_pPlayer->SetPosition(170);
		m_pPlayer->SetHealth(3.0f);
		m_ScoreText->SetScore(0.0f);

		//turn this hellish flag off
		RESETTING_LEVEL = false;
	}
	else
	{
		//Update all animation sequences subscribed to delta time.
		ImageManager::Update(deltatime);

		//Update player first
		m_pPlayer->Update(deltatime);

		//Update level
		m_TestLevel[LEVEL_INDEX]->Update(deltatime);
	}
}

void Game::Draw()
{
    // Setup the values we will clear to, then actually clear the color and depth buffers.
    glClearColor( 0.0f, 0.0f, 0.4f, 0.0f ); // dark blue
#if WIN32
    glClearDepth( 1 ); // 1 is maximum depth
#endif
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Draw our game objects.
	m_TestLevel[LEVEL_INDEX]->Draw();
	RENDERER::PARTICLE_RENDERER->Draw();
	m_pPlayer->Draw();
    m_ScoreText->Draw();
	m_VersionText->Draw();
	m_TestSprite->Draw();

    CheckForGLErrors();
}

void Game::IncreaseScore(float score)
{
	m_ScoreText->IncreaseScore(score);
}

void Game::SetNextLevel()
{
	LEVEL_INDEX++;

	if (LEVEL_INDEX > LEVEL_DATA::TOTAL_LEVELS)
		LEVEL_INDEX = 0;

	//update ai for next level and push the player down
	m_AI->SetLevel(m_TestLevel[LEVEL_INDEX]);
	m_TestLevel[LEVEL_INDEX]->BeginPlay();
	m_pPlayer->SetPosition(170);

	//save progress
	{
		//create root
		cJSON* root = cJSON_CreateObject();

		//create health and level values
		cJSON_AddNumberToObject(root, "health", (double)m_pPlayer->GetHealth());
		cJSON_AddNumberToObject(root, "level", LEVEL_INDEX);
		cJSON_AddNumberToObject(root, "score", m_ScoreText->GetScore());

		const char* filedata = cJSON_Print(root);
		std::ofstream output;

		output.open("Data/Save/Save.json", std::ofstream::out | std::ofstream::binary);
		if (output.is_open() == true)
		{
			output.write(filedata, strlen(filedata));
			output.close();
		}

		//free memory
		cJSON_Delete(root);
		free((void*)filedata); 
	}
}

void Game::ResetLevel()
{
	RESETTING_LEVEL = true;
}

void Game::NextBeat()
{
	GetActiveLevel()->DoEnemyMoves();
}


bool Game::CheckForCollisions(int index)
{
	//return GetActiveLevel()->CheckCollisionsAt(index);
	return true;
}
