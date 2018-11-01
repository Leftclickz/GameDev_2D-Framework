#ifndef __GameObject_H__
#define __GameObject_H__

class Mesh;

class GameObject
{
protected:
    GameCore* m_pGame;

    Mesh* m_pMesh;

    vec2 m_Position;
    float m_Angle;

    float m_Radius;

public:
    GameObject(GameCore* pGame, Mesh* pMesh);
    virtual ~GameObject();

    virtual void OnEvent(Event* pEvent) {}
    virtual void Update(float deltatime) {}
    virtual void Draw(vec2 camPos, vec2 projScale);

    virtual bool IsColliding(GameObject* pOtherGameObject);
    virtual void OnCollision(GameObject* pOtherGameObject);

    // Getters.
    vec2 GetPosition() { return m_Position; }
    float GetAngle() { return m_Angle; }
    float GetRadius() { return m_Radius; }

    // Setters.
    void SetPosition(vec2 pos) { m_Position = pos; }
    void SetAngle(float angle) { m_Angle = angle; }
    void SetRadius(float radius) { m_Radius = radius; }
};

#endif //__GameObject_H__
