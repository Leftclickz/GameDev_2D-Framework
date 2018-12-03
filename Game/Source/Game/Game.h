#ifndef __Game_H__
#define __Game_H__

class Mesh;
class Slime;
class Enemy;
class Skeleton;
class Player;
class PlayerController;
class AI_Patterns;
class TextObject;
class TextMesh;
class Level;

class Game : public GameCore
{
public:
    

protected:

	//Shaders
    ShaderProgram* m_TextureShader;
	ShaderProgram* m_DebugShader;
	ShaderProgram* m_CanvasShader;

	//Meshes
	Mesh* m_MeshTile;
	Mesh* m_WallMesh;
	TextMesh* m_TextMeshTest;

	//Game objects
    Player* m_pPlayer;
	Level* m_TestLevel;
	TextObject* m_TestText;

	//Controller
    PlayerController* m_pPlayerController;

	//AI Pathfinding
	AI_Patterns* m_AI;


public:
    Game(Framework* pFramework);
    virtual ~Game();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void LoadContent();

    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltatime);
    virtual void Draw();

	virtual Level* GetActiveLevel() { return m_TestLevel; }
	virtual void NextBeat();
	virtual Player* GetPlayer() { return m_pPlayer; }

    bool CheckForCollisions(int index);
};

#endif //__Game_H__
