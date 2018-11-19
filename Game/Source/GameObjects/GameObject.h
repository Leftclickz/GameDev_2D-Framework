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
    virtual void Update(float deltatime) {}
	virtual void Draw(vec2 camPos, vec2 projScale);

    virtual bool IsColliding(GameObject* pOtherGameObject);
    virtual void OnCollision(GameObject* pOtherGameObject);

    // Getters.
    vec2 GetPosition() { return m_Transform.object_position; }
    float GetAngle() { return m_Transform.angle; }
    float GetRadius() { return m_Radius; }

    // Setters.
    void SetPosition(vec2 pos) { m_Transform.object_position = pos; }
    void SetAngle(float angle) { m_Transform.angle = angle; }
    void SetRadius(float radius) { m_Radius = radius; }
};

#endif //__GameObject_H__
