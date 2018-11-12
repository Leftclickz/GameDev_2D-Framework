#ifndef __Ball_H__
#define __Ball_H__

class Ball : public GameObject
{
    //const float Ball_START_X_POSITION_PCT = 0.5f;
    //const float Ball_START_Y_POSITION_PCT = 0.5f;
    //const float Ball_SPEED_WHEN_HIT       = 30.0f; // Units per second

protected:
    float m_Speed;

public:
    Ball(GameCore* pGame, Mesh* pMesh);
    virtual ~Ball();

    virtual void Update(float deltatime);
    virtual void Draw(vec2 camPos, vec2 projScale);
    
    virtual void OnCollision(GameObject* pOtherGameObject);
};

#endif //__Ball_H__
