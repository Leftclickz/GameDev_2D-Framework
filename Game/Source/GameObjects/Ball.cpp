#include "GamePCH.h"

#include "Game/Game.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Ball.h"

Ball::Ball(GameCore* pGame, Mesh* pMesh)
: GameObject( pGame, pMesh )
, m_Speed( 0 )
{
    // Get the world width and height.
    float worldWidth = Game::Game_WORLD_SIZE_X;
    float worldHeight = Game::Game_WORLD_SIZE_Y;

    // Set the Ball's initial position.
    m_Position.Set( worldWidth * Ball_START_X_POSITION_PCT, worldHeight * Ball_START_Y_POSITION_PCT );
}
    
Ball::~Ball()
{
}

void Ball::Update(float deltatime)
{
    //GameObject::Update( deltatime );

    // Calculate the Ball's velocity based on the speed and the angle.
    float anglerad = m_Angle * PI / 180;
    vec2 velocity = m_Speed * vec2( cosf(anglerad), sinf(anglerad) );
        
    // Apply the velocity to the Ball's position.
    m_Position += velocity * deltatime;
    
    // Get the world width and height.
    float worldWidth = Game::Game_WORLD_SIZE_X;
    float worldHeight = Game::Game_WORLD_SIZE_Y;

    // Vertical bounds check
    if( m_Position.y + GetRadius() > worldHeight )
    {
        // Calculate the Ball's reflected angle.
        m_Angle = atan2f( -velocity.y, velocity.x ) * 180 / PI;
            
        // Make sure the Ball isn't still intersecting with the wall.
        m_Position.y = worldHeight - GetRadius() - 1.0f;
    }
    else if( m_Position.y - GetRadius() < 0.0f )
    {
        // Calculate the Ball's reflected angle.
        m_Angle = atan2f( -velocity.y, velocity.x ) * 180 / PI;

        //Make sure the Ball isn't still intersecting with the wall.
        m_Position.y = GetRadius() + 1.0f;
    }

    // Horizontal bounds check.
    if( m_Position.x - GetRadius() < 0.0f )
    {
        // Calculate the Ball's reflected angle.
        m_Angle = atan2f( velocity.y, -velocity.x ) * 180 / PI;
            
        // Make sure the Ball isn't still intersecting with the wall.
        m_Position.x = GetRadius() + 1.0f;
    }
    else if( m_Position.x + GetRadius() > worldWidth )
    {
        // Calculate the Ball's reflected angle.
        m_Angle = atan2f(velocity.y, -velocity.x) * 180 / PI;
            
        // Make sure the Ball isn't still intersecting with the wall.
        m_Position.x = worldWidth - GetRadius() - 1.0f;
    }

    // Slow down the Ball.
    m_Speed *= 0.99f;
}

void Ball::Draw(vec2 camPos, vec2 projScale)
{
    m_pMesh->Draw( m_Position, 0, 1, camPos, projScale );
}

void Ball::OnCollision(GameObject* pOtherGameObject)
{
    vec2 dir = m_Position - pOtherGameObject->GetPosition();
    dir.Normalize();

    m_Angle = atan2( dir.y, dir.x ) / PI * 180;
    m_Speed = Ball_SPEED_WHEN_HIT;
}
