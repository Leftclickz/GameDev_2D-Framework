#include "GamePCH.h"

#include "Game/Game.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"

Player::Player(GameCore* pGame, Mesh* pMesh) 
: AnimatedObject(pGame,pMesh, &ANIMATION_NAMES::PLAYER_IDLE)
, m_pPlayerController( 0 )
{
	m_Moved = false;
	SetAnchor(vec2(0.0f, TILE_SIZE.y * 0.5f));

	m_Damage = 1.0f;
	m_Life = 3.0f;

	m_Type = PLAYER;

	m_AttackSounds[0] = AudioManager::GetAudio(&AUDIO_NAMES::PLAYER_MELEE_1);
	m_AttackSounds[1] = AudioManager::GetAudio(&AUDIO_NAMES::PLAYER_MELEE_2);
	m_AttackSounds[2] = AudioManager::GetAudio(&AUDIO_NAMES::PLAYER_MELEE_3);
	m_AttackSounds[3] = AudioManager::GetAudio(&AUDIO_NAMES::PLAYER_MELEE_4);

	m_SwordClash = AudioManager::GetAudio(&AUDIO_NAMES::SWORD_CLASH);
}
    
Player::~Player()
{
}

void Player::Update(float deltatime)
{

	int new_pos = 0;

    if( m_pPlayerController )
    {
        if( m_pPlayerController->IsForwardHeld() )
        {
			m_pPlayerController->SetUp();
			new_pos += LEVEL_TILE_DIMENSIONS.x;
        }

        if( m_pPlayerController->IsReverseHeld() )
        {
			m_pPlayerController->SetDown();
			new_pos -= LEVEL_TILE_DIMENSIONS.x;
        }

        if( m_pPlayerController->IsTurnLeftHeld() )
        {
			m_pPlayerController->SetLeft();
			new_pos -= 1;
        }

        if( m_pPlayerController->IsTurnRightHeld() )
        {
			m_pPlayerController->SetRight();
			new_pos += 1;
        }
    }

	if (new_pos != 0 && m_Moved == false)
	{
		Game* game = (Game*)m_pGame;
		
		//Attempt movement
		AttemptMovement(new_pos);

		//Now that we have attempted a move command, set our flag to true and make the game move enemies early.
		m_Moved = true;
		game->NextBeat();
	}
}

void Player::Draw()
{
	AnimatedObject::Draw();
}

void Player::AttemptMovement(int index)
{
	Game* game = (Game*)m_pGame;

	//convert index into a position
	int x = index % LEVEL_TILE_DIMENSIONS.x;
	int y = index / LEVEL_TILE_DIMENSIONS.x;
	vec2 new_pos = vec2((float)x, (float)y) * TILE_SIZE;

	//check for walls
	if (game->GetActiveLevel()->GetTileAtPosition(GetPosition() + (new_pos))->IsWalkable() == false)
		return;

	//check for enemies
	if (game->GetActiveLevel()->CheckForCollisionsAt(index + GetPositionByIndex(), this) == true)
	{
		int random = rand() % 4;

		//play a hit sound as well as a player attack sound
		m_SwordClash->Stop();
		m_SwordClash->Play();
		m_AttackSounds[random]->Stop();
		m_AttackSounds[random]->Play();

		return;
	}

	//move otherwise
	m_Transform.object_position += new_pos;
}
