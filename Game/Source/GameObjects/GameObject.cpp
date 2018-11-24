#include "GamePCH.h"

#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"

GameObject::GameObject(GameCore* pGame, Mesh* pMesh)
: m_pGame( pGame )
, m_pMesh( pMesh )
, m_Radius(10)
{
	m_Transform.object_position = vec2(0.0f, 0.0f);
	m_Transform.angle = 0.0f;
	m_Transform.object_scale = vec2(1.0f,1.0f);
	m_Transform.object_anchor = vec2(0.0f, 0.0f);

}

GameObject::~GameObject()
{
}

void GameObject::Draw(vec2 camPos, vec2 projScale)
{
	m_Transform.cam_pos = camPos;
	m_Transform.proj_scale = projScale;

	if (m_pMesh->IsDebugging())
		m_pMesh->SetDebugColor(GetDebugTag());
}

bool GameObject::IsColliding(GameObject* pOtherGameObject)
{
    return false;
}

void GameObject::OnCollision(GameObject* pOtherGameObject)
{
}