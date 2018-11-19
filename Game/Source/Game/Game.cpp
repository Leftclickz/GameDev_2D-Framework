#include "GamePCH.h"

#include "Game/Game.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"
#include "ImageManager/ImageManager.h"


Game::Game(Framework* pFramework)
: GameCore( pFramework, new EventManager() )
{

	//char* str;

	//Create
	/*
	{
		cJSON* Root = cJSON_CreateObject();

		cJSON* GameObject = cJSON_CreateObject();

		cJSON_AddNumberToObject(GameObject, "x", 10.5f);
		cJSON_AddNumberToObject(GameObject, "y", -12.5f);

		cJSON_AddItemToObject(Root, "Game Object", GameObject);

		str = cJSON_Print(Root);

		cJSON_Delete(Root);
	}
	//Load
	{
		cJSON* Root = cJSON_Parse(str);
		cJSON* obj = cJSON_GetObjectItem(Root, "Game Object");

		float x = (float)cJSON_GetObjectItem(obj, "x")->valuedouble;
		float y = (float)cJSON_GetObjectItem(obj, "y")->valuedouble;

		cJSON_Delete(Root);
	}
	*/
	//free(str);

    m_TextureShader = 0;
	m_DebugShader = 0;

    m_pPlayer = 0;
    m_pBall = 0;

    m_pPlayerController = 0;
}

Game::~Game()
{
    delete m_pPlayerController;

    delete m_pPlayer;
    delete m_pBall;

	delete m_MeshTile;
	delete m_TestLevel;

    delete m_TextureShader;
	delete m_DebugShader;

	ImageManager::Release();
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
	m_MeshTile->SetDrawDebugLines(true);

	//Initialize image manager and reserve some space
	ImageManager::Initialize();
	ImageManager::Reserve(10);

	//Load some images
	ImageManager::LoadImageAtlas("Bomberman", "SpriteTool");
	ImageManager::LoadImageData("Default");
	ImageManager::LoadImageData("Miku");
	ImageManager::LoadImageData("Pixel_Miku");

	//Create a test animation
	ImageManager::CreateAnimation("BomberMan_WalkingDown", "Bomberman");
	AnimatedSprite* animation = ImageManager::UseAnimation("BomberMan_WalkingDown");
	animation->BuildAnimationArray(2);
	animation->UseFrame("BM_WalkDown2");
	animation->UseFrame("BM_WalkDown3");
	animation->SetFramerate(4);

    // Create our player.
    m_pPlayer = new Player( this, m_MeshTile, "Pixel_Miku");

	//Testing animated sprite atlas
    m_pBall = new Enemy( this, m_MeshTile, "BomberMan_WalkingDown" );

    // Assign our controller.
    m_pPlayerController = new PlayerController(m_pPlayer);
    m_pPlayer->SetPlayerController( m_pPlayerController );

	m_pBall->SetPosition(vec2(50.0f, 50.0f));

	m_TestLevel = new Level(this, m_MeshTile, "test");

    CheckForGLErrors();
}


void Game::OnEvent(Event* pEvent)
{
    m_pPlayerController->OnEvent( pEvent );
}

void Game::Update(float deltatime)
{
    m_pPlayer->Update( deltatime );
    m_pBall->Update( deltatime );

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

    // Draw our game objects.
	m_TestLevel->Draw(m_pPlayer->GetPosition(), 1 / HALF_SCREEN);
    m_pPlayer->Draw( m_pPlayer->GetPosition(), 1/ HALF_SCREEN);
    m_pBall->Draw(m_pPlayer->GetPosition(), 1/ HALF_SCREEN);

    CheckForGLErrors();
}

void Game::CheckForCollisions()
{
    if( m_pBall->IsColliding( m_pPlayer ) )
    {
        m_pPlayer->OnCollision( m_pBall );
        m_pBall->OnCollision( m_pPlayer );
    }
}
