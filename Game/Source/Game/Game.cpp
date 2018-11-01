#include "GamePCH.h"

#include "Game/Game.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/Ball.h"
#include "GameObjects/PlayerController.h"

Game::Game(Framework* pFramework)
: GameCore( pFramework, new EventManager() )
{
    m_pShader = 0;
    m_pMeshTriangle = 0;
    m_pMeshCircle = 0;

    m_pPlayer = 0;
    m_pBall = 0;

    m_pPlayerController = 0;
}

Game::~Game()
{
    delete m_pPlayerController;

    delete m_pPlayer;
    delete m_pBall;

    delete m_pMeshTriangle;
    delete m_pMeshCircle;

    delete m_pShader;
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
    m_pShader = new ShaderProgram( "Data/Shaders/Moving.vert", "Data/Shaders/Moving.frag" );

    // Create out meshes.
    m_pMeshTriangle = new Mesh();
    m_pMeshTriangle->SetShader( m_pShader );
    m_pMeshTriangle->GenerateTriangle();

    m_pMeshCircle = new Mesh();
    m_pMeshCircle->SetShader( m_pShader );
    m_pMeshCircle->GenerateCircle();

    // Create our GameObjects.
    m_pPlayer = new Player( this, m_pMeshTriangle );
    m_pBall = new Ball( this, m_pMeshCircle );

    // Assign our controllers.
    m_pPlayerController = new PlayerController();
    m_pPlayer->SetPlayerController( m_pPlayerController );

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

    vec2 halfWorldSize = vec2( Game_WORLD_SIZE_X/2.0f, Game_WORLD_SIZE_Y/2.0f );

    // Draw our game objects.
    m_pPlayer->Draw( halfWorldSize, 1/halfWorldSize );
    m_pBall->Draw( halfWorldSize, 1/halfWorldSize );

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
