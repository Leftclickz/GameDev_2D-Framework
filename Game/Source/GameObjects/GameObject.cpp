#include "GamePCH.h"

#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"

GameObject::GameObject(GameCore* pGame, Mesh* pMesh)
: m_pGame( pGame )
, m_pMesh( pMesh )
, m_Position( 0, 0 )
, m_Angle( 0 )
, m_Radius( 10 )
{
}

GameObject::~GameObject()
{
}

void GameObject::Draw(vec2 camPos, vec2 projScale)
{
    if( m_pMesh != 0 )
        m_pMesh->Draw( m_Position, m_Angle, 1, camPos, projScale );
}

bool GameObject::IsColliding(GameObject* pOtherGameObject)
{
    return false;
}

void GameObject::OnCollision(GameObject* pOtherGameObject)
{
}