#include "GamePCH.h"

#include "Game/Game.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/Ball.h"
#include "GameObjects/PlayerController.h"

Player::Player(GameCore* pGame, Mesh* pMesh, const char* pName) 
: TextureObject(pGame,pMesh,pName)
, m_pPlayerController( 0 )
, m_Speed( 0 )
, m_TurningSpeed( 0 )
{
    // Get the world width and height.
    unsigned int worldWidth = SCREEN_SIZE_X;
    unsigned int worldHeight = SCREEN_SIZE_Y;

    // Set the Player's initial position.
    //m_Position.Set( worldWidth * PLAYER_START_X_POSITION_PCT, worldHeight * PLAYER_START_Y_POSITION_PCT );
}
    
Player::~Player()
{
}

void Player::Update(float deltatime)
{
    //GameObject::Update( deltatime );

    SetSpeed( 0 );
    SetTurningSpeed( 0 );

	vec2 new_pos = vec2(0.0f,0.0f);

    if( m_pPlayerController )
    {
        if( m_pPlayerController->IsForwardHeld() )
        {
            //SetSpeed( PLAYER_SPEED_FORWARD );
			m_pPlayerController->SetUp();
			new_pos.y += 1.0f;
        }

        if( m_pPlayerController->IsReverseHeld() )
        {
            //SetSpeed( -PLAYER_SPEED_REVERSE );
			m_pPlayerController->SetDown();
			new_pos.y -= 1.0f;
        }

        if( m_pPlayerController->IsTurnLeftHeld() )
        {
            //SetTurningSpeed( PLAYER_TURNING_SPEED );
			m_pPlayerController->SetLeft();
			new_pos.x -= 1.0f;
        }

        if( m_pPlayerController->IsTurnRightHeld() )
        {
            //SetTurningSpeed( -PLAYER_TURNING_SPEED );
			m_pPlayerController->SetRight();
			new_pos.x += 1.0f;
        }
    }
	new_pos *= TILE::TILE_LENGTH;
	m_Position += new_pos;
    //float anglerad = (m_Angle + 90) / 180.0f * PI;
    //vec2 dir( cos(anglerad), sin(anglerad) );

   // m_Position += dir * m_Speed * deltatime;
    //m_Angle += m_TurningSpeed * deltatime;

    //MyClamp( m_Position.x, 0.0f, (float)SCREEN_SIZE_X );
    //MyClamp( m_Position.y, 0.0f, (float)SCREEN_SIZE_Y );
}

void Player::Draw(vec2 camPos, vec2 projScale)
{
	TextureObject::Draw(camPos, projScale);
    //m_pMesh->Draw( m_Position, m_Angle, 1, camPos, projScale );
}
