#include "GamePCH.h"
#include <thread>
#include "Game.h"

Game::Game(Framework* pFramework)
: GameCore( pFramework, new EventManager() )
{
    m_TextureShader = 0;
	m_DebugShader = 0;

    m_pPlayer = 0;

    m_pPlayerController = 0;
	m_AI = 0;
}

Game::~Game()
{
    delete m_pPlayerController;

    delete m_pPlayer;
	delete m_TestText;

	delete m_MeshTile;
	delete m_WallMesh;
	delete m_TextMeshTest;

	delete m_TestLevel;

	SHADERS::DestroyShaders();
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
    wglSwapInterval( 1 );
#endif

    // Turn on depth buffer testing.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    // Turn on alpha blending.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Create our shaders.
	SHADERS::LoadShaders();

	// Load meshes
	MESHES::CreateDefaultMeshes();
	
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
	ImageManager::LoadImageData(&TEXTURE_NAMES::DEFAULT);

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

	//create floor animation
	animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::F1_V1, &TEXTURE_NAMES::FLOOR);
	animation->CreateAnimationUsingAtlas(0, 1);
	animation->SetAnimatedWithUpdates(false);

	//create floor 2 animation
	animation = ImageManager::CreateAnimation(&ANIMATION_NAMES::F1_V2, &TEXTURE_NAMES::FLOOR);
	animation->CreateAnimationUsingAtlas(2, 3);
	animation->SetAnimatedWithUpdates(false);
	animation->NextFrame();
	
	//Create mesh
	m_MeshTile = new Mesh(TILE::TILE_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
	//m_MeshTile->SetDrawDebugLines(true);
	m_WallMesh = new Mesh(TILE::WALL_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
	//m_WallMesh->SetDrawDebugLines(true);

	//test Text
	m_TextMeshTest = new TextMesh("IF TRAVIS READS THIS HE IS GAY");
	//m_TextMeshTest->SetDrawDebugLines();

    // Create our player.
    m_pPlayer = new Player( this, MESHES::TILEMESH_DEFAULT_SIZE);

	//Create our cameras
	CAMERA::CreateCameras(this);

	//Create our level
	m_TestLevel = new Level(this, &TEXTURE_NAMES::FLOOR);

	//Create AI
	m_AI = new AI_Patterns(this);
	m_AI->SetLevel(m_TestLevel);

	//Testing text
	m_TestText = new TextObject(this, m_TextMeshTest);

    // Assign our controller.
    m_pPlayerController = new PlayerController(m_pPlayer);
    m_pPlayer->SetPlayerController( m_pPlayerController );

	m_TestText->SetPosition(2);
	m_pPlayer->SetPosition(46);

    CheckForGLErrors();
}


void Game::OnEvent(Event* pEvent)
{
    m_pPlayerController->OnEvent( pEvent );
}

void Game::Update(float deltatime)
{
	//Update all animation sequences subscribed to delta time.
	ImageManager::Update(deltatime);

	//Update player first
	m_pPlayer->Update(deltatime);

	//Update level
	m_TestLevel->Update(deltatime);
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
	m_TestLevel->Draw();
	m_pPlayer->Draw();
    m_TestText->Draw();

    CheckForGLErrors();
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
