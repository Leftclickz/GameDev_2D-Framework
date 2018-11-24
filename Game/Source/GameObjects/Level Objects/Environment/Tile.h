#pragma once
#include "../../AnimatedObject.h"


class Wall;

class Tile : public AnimatedObject
{
public:

	Tile();
	Tile(GameCore* pGame, Mesh* pMesh, const char* pName);
	virtual ~Tile();

	virtual void Draw(vec2 camPos, vec2 projScale) override;

	void AddWall(Wall* aWall);

	virtual void SetPosition(int index) override;
	virtual bool IsWalkable() { return (m_Wall != nullptr) ? false : true; }

protected:

	virtual std::string GetDebugTag() override { return "Tile"; }

	Wall* m_Wall;

};