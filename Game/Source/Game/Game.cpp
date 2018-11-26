#include "GamePCH.h"

Game::Game(Framework* pFramework)
: GameCore( pFramework, new EventManager() )
{
    m_TextureShader = 0;
	m_DebugShader = 0;

    m_pPlayer = 0;
    m_SlimeTest = 0;
	m_BatTest = 0;

    m_pPlayerController = 0;
	m_AI = 0;
}

Game::~Game()
{
    delete m_pPlayerController;

    delete m_pPlayer;
    delete m_SlimeTest;
	delete m_BatTest;
	delete m_SkeletonTest;
	delete m_TestText;

	delete m_MeshTile;
	delete m_WallMesh;
	delete m_TextMeshTest;

	delete m_TestLevel;

	SHADERS::DestroyShaders();
	CAMERA::DestroyCameras();

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
	
	//Initialize sounds
	AudioManager::Initialize();
	AudioManager::Reserve();

	//Test load a sound
	AudioManager::LoadFromPath("Floor_1");

	//Initialize image manager and reserve some space
	ImageManager::Initialize();
	ImageManager::Reserve(10);

	//Load some images
	ImageManager::LoadImageAtlas("Player", "SpriteTool");
	ImageManager::LoadImageAtlas("Bat", "SpriteTool");
	ImageManager::LoadImageAtlas("SlimeGreen", "SpriteTool");
	ImageManager::LoadImageAtlas("SkeletonWhite", "SpriteTool");
	ImageManager::LoadImageAtlas("Floor_1", "SpriteTool");
	ImageManager::LoadImageAtlas("Wall_1", "SpriteTool");
	ImageManager::LoadImageAtlas("Text/DefaultFont_White", "SpriteTool");
	ImageManager::LoadImageData("Default");
	ImageManager::LoadImageData("Miku");
	ImageManager::LoadImageData("Pixel_Miku");

	//create player animation
	AnimatedSprite* animation = ImageManager::CreateAnimation("Player_Idle", "Player");
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
	
	//Create mesh
	m_MeshTile = new Mesh(TILE::TILE_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
	m_MeshTile->SetDrawDebugLines(true);
	m_WallMesh = new Mesh(TILE::WALL_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
	m_WallMesh->SetDrawDebugLines(true);

	//test Text
	m_TextMeshTest = new TextMesh("PAUL IS GAY OMEGALUL KAPPA123");
	m_TextMeshTest->SetDrawDebugLines();

    // Create our player.
    m_pPlayer = new Player( this, m_MeshTile);

	//Create our cameras
	CAMERA::CreateCameras(this);

	//Create our level
	m_TestLevel = new Level(this, m_MeshTile, m_WallMesh, "Floor_1");

	//Create AI
	m_AI = new AI_Patterns(this);
	m_AI->SetLevel(m_TestLevel);

	//Testing animated sprite atlas
    m_SlimeTest = new Slime( this, m_MeshTile);
	m_BatTest = new Enemy(this, m_MeshTile, "Bat_Idle");
	m_SkeletonTest = new Skeleton(this, m_MeshTile);

	m_TestText = new TextObject(this, m_TextMeshTest);

    // Assign our controller.
    m_pPlayerController = new PlayerController(m_pPlayer);
    m_pPlayer->SetPlayerController( m_pPlayerController );



	m_SlimeTest->SetPosition(vec2(200.0f, 50.0f));
	m_BatTest->SetPosition(vec2(100.0f, 50.0f));
	m_SkeletonTest->SetPosition(vec2(300, 100.0f));

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
	m_TestLevel->Draw();


	m_pPlayer->Draw();
	m_SlimeTest->Draw();
	m_BatTest->Draw();
	m_SkeletonTest->Draw();

	m_TestText->Draw();

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
	m_SkeletonTest->DoNextMove();
}

void Game::CheckForCollisions()
{
    if( m_SlimeTest->IsColliding( m_pPlayer ) )
    {
        m_pPlayer->OnCollision( m_SlimeTest );
        m_SlimeTest->OnCollision( m_pPlayer );
    }
}
