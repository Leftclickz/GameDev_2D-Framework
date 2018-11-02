#ifndef __Game_H__
#define __Game_H__

class Ball;
class Mesh;
class GameObject;
class Player;
class PlayerController;

class Game : public GameCore
{
public:
    static const int Game_WORLD_SIZE_X = 50;
    static const int Game_WORLD_SIZE_Y = 50;

protected:
    ShaderProgram* m_pShader;
    Mesh* m_pMeshTriangle;
    Mesh* m_pMeshCircle;

    Player* m_pPlayer;
    TextureObject* m_pBall;

    PlayerController* m_pPlayerController;

public:
    Game(Framework* pFramework);
    virtual ~Game();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void LoadContent();

    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltatime);
    virtual void Draw();

    void CheckForCollisions();
};

#endif //__Game_H__
