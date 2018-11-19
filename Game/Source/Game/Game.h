#ifndef __Game_H__
#define __Game_H__

class Mesh;


class Enemy;
class Player;
class PlayerController;


class Level;

class Game : public GameCore
{
public:
    

protected:

	//Shaders
    ShaderProgram* m_TextureShader;
	ShaderProgram* m_DebugShader;

	//Meshes
	Mesh* m_MeshTile;

	//Game objects
    Player* m_pPlayer;
    Enemy* m_pBall;
	Level* m_TestLevel;

	//Controller
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
