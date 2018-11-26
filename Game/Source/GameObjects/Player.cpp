#include "GamePCH.h"

#include "Game/Game.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"

Player::Player(GameCore* pGame, Mesh* pMesh) 
: AnimatedObject(pGame,pMesh,"Player_Idle")
, m_pPlayerController( 0 )
, m_Speed( 0 )
, m_TurningSpeed( 0 )
{
	m_Moved = false;
	SetAnchor(vec2(0.0f, TILE_SIZE.y * 0.5f));

	//m_BeatTimer = new Timer(((Game*)m_pGame)->GetActiveLevel()->GetBPM(), true);
	//m_BeatTimer->Start();
}
    
Player::~Player()
{
}

void Player::Update(float deltatime)
{

    SetSpeed( 0 );
    SetTurningSpeed( 0 );


	//m_BeatTimer->

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

	if (new_pos != vec2(0.0f, 0.0f) && m_Moved == false)
	{
		Game* game = (Game*)m_pGame;

		//check to see if there's a wall
		if (game->GetActiveLevel()->GetTileAtPosition(GetPosition() + (new_pos * TILE_SIZE))->IsWalkable())
			Move(new_pos);

		//Now that we have attempted a move command, set our flag to true and make the game move enemies early.
		m_Moved = true;
		game->NextBeat();
	}
}

void Player::Draw()
{
	AnimatedObject::Draw();
}

void Player::Move(vec2 direction)
{
	m_Transform.object_position += (direction * TILE::TILE_LENGTH);	
}
