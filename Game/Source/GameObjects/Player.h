#ifndef __Player_H__
#define __Player_H__

class AnimatedObject;
class Mesh;
class PlayerController;
class Timer;

class Player : public AnimatedObject
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
	bool m_Moved;

	Timer* m_BeatTimer;

	virtual std::string GetDebugTag() override { return "Player"; }

public:
    Player(GameCore* pGame, Mesh* pMesh);
    virtual ~Player();

    virtual void Update(float deltatime);
    virtual void Draw();

    void SetPlayerController(PlayerController* controller) { m_pPlayerController = controller; }

	void Move(vec2 direction);
	bool HasMovedThisBeat() { return m_Moved; }

    void SetSpeed(float speed) { m_Speed = speed; }
    void SetTurningSpeed(float speed) { m_TurningSpeed = speed; }
	void SetMoved(bool moved) { m_Moved = moved; }
};

#endif //__Player_H__
