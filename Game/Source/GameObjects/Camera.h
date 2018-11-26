#pragma once
#include "GameObject.h"

class Camera : public GameObject
{

public:

	Camera(GameCore* m_Game, Mesh* mesh = nullptr);


	vec2 GetPosition() { if (m_Owner != nullptr) return m_Owner->GetPosition(); else return vec2(0.0f, 0.0f); }
	void SetOwner(GameObject* object) { m_Owner = object; }

	vec2 GetProjection() { return m_ProjectionScale; }
	void SetProjection(vec2 projection) { m_ProjectionScale = projection; }

protected:

	vec2 m_ProjectionScale;
	GameObject* m_Owner;
};