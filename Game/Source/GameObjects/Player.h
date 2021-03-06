#ifndef __Player_H__
#define __Player_H__

class TextureObject;
class Mesh;
class PlayerController;

class Player : public TextureObject
{
    const float PLAYER_START_X_POSITION_PCT = 0.5f;
    const float PLAYER_START_Y_POSITION_PCT = 0.3f;
    const float PLAYER_SPEED_FORWARD        = 150.0f; // Units per second
    const float PLAYER_SPEED_REVERSE        = 75.0f;  // Units per second
    const float PLAYER_TURNING_SPEED        = 180.0f; // Degrees per second

protected:
    PlayerController* m_pPlayerController;

    float m_Speed;
    float m_TurningSpeed;

public:
    Player(GameCore* pGame, Mesh* pMesh, const char* pName);
    virtual ~Player();

    virtual void Update(float deltatime);
    virtual void Draw(vec2 camPos, vec2 projScale);

    void SetPlayerController(PlayerController* controller) { m_pPlayerController = controller; }
    void SetSpeed(float speed) { m_Speed = speed; }
    void SetTurningSpeed(float speed) { m_TurningSpeed = speed; }
};

#endif //__Player_H__
