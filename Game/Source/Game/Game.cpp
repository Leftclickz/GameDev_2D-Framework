#include "GamePCH.h"

#include "Game/Game.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"
#include "ImageManager/ImageManager.h"
#include "AudioManager/AudioManager.h"


Game::Game(Framework* pFramework)
: GameCore( pFramework, new EventManager() )
{
    m_TextureShader = 0;
	m_DebugShader = 0;

    m_pPlayer = 0;
    m_SlimeTest = 0;
	m_BatTest = 0;

    m_pPlayerController = 0;
}

Game::~Game()
{
    delete m_pPlayerController;

    delete m_pPlayer;
    delete m_SlimeTest;
	delete m_BatTest;
	delete m_SkeletonIdleTest;
	delete m_SkeletonAttackingTest;

	delete m_MeshTile;
	delete m_WallMesh;

	delete m_TestLevel;

    delete m_TextureShader;
	delete m_DebugShader;

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
    m_TextureShader = new ShaderProgram( "Data/Shaders/Moving.vert", "Data/Shaders/Moving.frag" );
	m_DebugShader = new ShaderProgram("Data/Shaders/Color.vert", "Data/Shaders/Color.frag");

	//Create mesh
	m_MeshTile = new Mesh(TILE::TILE_MESH, TILE::TILE_VERT_COUNT, m_TextureShader, m_DebugShader, GL_TRIANGLE_FAN);
	//m_MeshTile->SetDrawDebugLines(true);
	m_WallMesh = new Mesh(TILE::WALL_MESH, TILE::TILE_VERT_COUNT, m_TextureShader, m_DebugShader, GL_TRIANGLE_FAN);
	//m_WallMesh->SetDrawDebugLines(true);
	
	//Initialize sounds
	AudioManager::Initialize();
	AudioManager::Reserve();

	//Test load a sound
	AudioManager::LoadFromPath("Floor_1");

	//Initialize image manager and reserve some space
	ImageManager::Initialize();
	ImageManager::Reserve(10);

	//Load some images
	ImageManager::LoadImageAtlas("Bomberman", "SpriteTool");
	ImageManager::LoadImageAtlas("Player", "SpriteTool");
	ImageManager::LoadImageAtlas("Bat", "SpriteTool");
	ImageManager::LoadImageAtlas("SlimeGreen", "SpriteTool");
	ImageManager::LoadImageAtlas("SkeletonWhite", "SpriteTool");
	ImageManager::LoadImageAtlas("Floor_1", "SpriteTool");
	ImageManager::LoadImageAtlas("Wall_1", "SpriteTool");
	ImageManager::LoadImageData("Default");
	ImageManager::LoadImageData("Miku");
	ImageManager::LoadImageData("Pixel_Miku");

	//Create a test animation
	AnimatedSprite* animation = ImageManager::CreateAnimation("BomberMan_WalkingDown", "Bomberman");
	animation->CreateAnimationUsingAtlas(0, 1);

	//create player animation
	animation = ImageManager::CreateAnimation("Player_Idle", "Player");
	animation->CreateAnimationUsingAtlas(0, 3);

	//create player animation
	animation = ImageManager::CreateAnimation("Bat_Idle", "Bat");
	animation->CreateAnimationUsingAtlas(0, 3);

	//create player animation
	animation = ImageManager::CreateAnimation("SlimeGreen_Idle", "SlimeGreen");
	animation->CreateAnimationUsingAtlas(0, 3);

	//create player animation
	animation = ImageManager::CreateAnimation("SkeletonWhite_Idle", "SkeletonWhite");
	animation->CreateAnimationUsingAtlas(4, 7);

	//create player animation
	animation = ImageManager::CreateAnimation("SkeletonWhite_Attacking", "SkeletonWhite");
	animation->CreateAnimationUsingAtlas(0, 3);

	//create player animation
	animation = ImageManager::CreateAnimation("Floor_1_Variant_1", "Floor_1");
	animation->CreateAnimationUsingAtlas(0, 1);
	animation->SetAnimatedWithUpdates(false);

	//create player animation
	animation = ImageManager::CreateAnimation("Floor_1_Variant_2", "Floor_1");
	animation->CreateAnimationUsingAtlas(2, 3);
	animation->SetAnimatedWithUpdates(false);
	animation->NextFrame();


	//Create our level
	m_TestLevel = new Level(this, m_MeshTile, m_WallMesh, "Floor_1");

    // Create our player.
    m_pPlayer = new Player( this, m_MeshTile);

	//Testing animated sprite atlas
    m_SlimeTest = new Slime( this, m_MeshTile);
	m_BatTest = new Enemy(this, m_MeshTile, "BomberMan_WalkingDown");
	m_SkeletonAttackingTest = new Enemy(this, m_MeshTile, "SkeletonWhite_Idle");
	m_SkeletonIdleTest = new Enemy(this, m_MeshTile, "SkeletonWhite_Attacking");

    // Assign our controller.
    m_pPlayerController = new PlayerController(m_pPlayer);
    m_pPlayer->SetPlayerController( m_pPlayerController );

	m_SlimeTest->SetPosition(vec2(200.0f, 50.0f));
	m_BatTest->SetPosition(vec2(100.0f, 50.0f));
	m_SkeletonAttackingTest->SetPosition(vec2(50.0f, 100.0f));
	m_SkeletonIdleTest->SetPosition(vec2(100.0f, 100.0f));



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
	
	//Update player for inputs
	m_pPlayer->Update(deltatime);

	//Update level
	m_TestLevel->Update(deltatime);

    CheckForCollisions();
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
	m_TestLevel->Draw(m_pPlayer->GetPosition(), 1 / HALF_SCREEN);


	m_pPlayer->Draw(m_pPlayer->GetPosition(), 1 / HALF_SCREEN);
	m_SlimeTest->Draw(m_pPlayer->GetPosition(), 1 / HALF_SCREEN);
	m_BatTest->Draw(m_pPlayer->GetPosition(), 1 / HALF_SCREEN);
	m_SkeletonIdleTest->Draw(m_pPlayer->GetPosition(), 1 / HALF_SCREEN);
	m_SkeletonAttackingTest->Draw(m_pPlayer->GetPosition(), 1 / HALF_SCREEN);


	//m_TestLevel->Draw(HALF_LEVEL * TILE_SIZE, 1 / (HALF_LEVEL * TILE_SIZE));
	//m_pPlayer->Draw(HALF_LEVEL * TILE_SIZE, 1 / (HALF_LEVEL * TILE_SIZE));
	//m_SlimeTest->Draw(HALF_LEVEL * TILE_SIZE, 1 / (HALF_LEVEL * TILE_SIZE));
	//m_BatTest->Draw(HALF_LEVEL * TILE_SIZE, 1 / (HALF_LEVEL * TILE_SIZE));
	//m_SkeletonIdleTest->Draw(HALF_LEVEL * TILE_SIZE, 1 / (HALF_LEVEL * TILE_SIZE));
	//m_SkeletonAttackingTest->Draw(HALF_LEVEL * TILE_SIZE, 1 / (HALF_LEVEL * TILE_SIZE));

    CheckForGLErrors();
}

void Game::NextBeat()
{
	m_SlimeTest->DoNextMove();
	
}

void Game::CheckForCollisions()
{
    if( m_SlimeTest->IsColliding( m_pPlayer ) )
    {
        m_pPlayer->OnCollision( m_SlimeTest );
        m_SlimeTest->OnCollision( m_pPlayer );
    }
}
