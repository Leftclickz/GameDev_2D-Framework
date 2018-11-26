#ifndef __GameObject_H__
#define __GameObject_H__

class Mesh;

class GameObject
{
protected:
    GameCore* m_pGame;

    Mesh* m_pMesh;
    float m_Radius;

	WorldTransform m_Transform;

	std::string m_Tag;

	virtual std::string GetDebugTag() { return "Default"; }

public:
    GameObject(GameCore* pGame, Mesh* pMesh);
    virtual ~GameObject();

    virtual void OnEvent(Event* pEvent) {}
	virtual void Draw();

    virtual bool IsColliding(GameObject* pOtherGameObject);
    virtual void OnCollision(GameObject* pOtherGameObject);

    // Getters.
    vec2 GetPosition() { return m_Transform.object_position; }
	int GetPositionByIndex() { return (int)((m_Transform.object_position.y / TILE_SIZE.y) * LEVEL_TILE_DIMENSIONS.x + (m_Transform.object_position.x / TILE_SIZE.x)); }
    float GetAngle() { return m_Transform.angle; }
    float GetRadius() { return m_Radius; }
	vec2 GetAnchor() { return m_Transform.object_anchor; }

    // Setters.
    void SetPosition(vec2 pos) { m_Transform.object_position = pos; }
	virtual void SetPosition(int index) {

		float x = (abs(index) % LEVEL_TILE_DIMENSIONS.x) * TILE_SIZE.x;
		float y = (index / LEVEL_TILE_DIMENSIONS.x) * TILE_SIZE.y;
		SetPosition(vec2(x, y));
	}

	void SetAnchor(vec2 anchor) { m_Transform.object_anchor = anchor; }

    void SetAngle(float angle) { m_Transform.angle = angle; }
    void SetRadius(float radius) { m_Radius = radius; }
};

#endif //__GameObject_H__
