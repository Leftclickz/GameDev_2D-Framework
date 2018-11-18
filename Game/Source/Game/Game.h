#ifndef __Game_H__
#define __Game_H__

class Ball;
class Mesh;
class GameObject;
class AnimatedObject;
class Player;
class PlayerController;
class Level;

class Game : public GameCore
{
public:
    

protected:
    ShaderProgram* m_pShader;
    Mesh* m_pMeshTriangle;
    Mesh* m_pMeshCircle;
	Mesh* m_MeshTile;
	Mesh* m_EmptyMesh;

    Player* m_pPlayer;
    AnimatedObject* m_pBall;

	Level* m_TestLevel;

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
