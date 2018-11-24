#include "GamePCH.h"

Tile::Tile() : AnimatedObject(nullptr, nullptr, nullptr)
{
}

Tile::Tile(GameCore* pGame, Mesh* pMesh, const char* pName) : AnimatedObject(pGame, pMesh, pName)
{
	m_Wall = nullptr;
}

Tile::~Tile()
{
	if (m_Wall != nullptr)
	{
		delete m_Wall;
		m_Wall = nullptr;
	}
}

void Tile::Draw(vec2 camPos, vec2 projScale)
{
	AnimatedObject::Draw(camPos, projScale);
	
	if (m_Wall != nullptr)
		m_Wall->Draw(camPos, projScale);
}

void Tile::AddWall(Wall* aWall)
{
	m_Wall = aWall;
}

void Tile::SetPosition(int index)
{
	GameObject::SetPosition(index);

	if (m_Wall != nullptr)
		m_Wall->SetPosition(index);
}

