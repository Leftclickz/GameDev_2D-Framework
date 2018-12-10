#ifndef __Game_H__
#define __Game_H__

class Mesh;
class Slime;
class Enemy;
class Skeleton;
class Player;
class PlayerController;
class AI_Patterns;
class HUD_ScoreDisplay;
class HUD_Sprite;
class HUD_Text;
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

	//Game objects
    Player* m_pPlayer;
	Level** m_TestLevel;
	HUD_ScoreDisplay* m_ScoreText;
	HUD_Text* m_VersionText;
	HUD_Sprite* m_TestSprite;

	unsigned int LEVEL_INDEX;
	bool RESETTING_LEVEL;

	//Controller
    PlayerController* m_pPlayerController;

	//AI Pathfinding
	AI_Patterns* m_AI;


public:
    Game(Framework* pFramework);
    virtual ~Game();

	//I/O
    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void LoadContent();
	virtual bool AttemptToLoadSave();

	//Game loop
    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltatime);
    virtual void Draw();

	//increase game score, directly helps the HUD
	virtual void IncreaseScore(float score);

	//level management functions
	virtual Level* GetActiveLevel() { return m_TestLevel[LEVEL_INDEX]; }
	virtual void SetNextLevel();
	virtual void ResetLevel();

	//beat control function
	virtual void NextBeat();

	//player fetching
	virtual Player* GetPlayer() { return m_pPlayer; }

	//deprecated
    bool CheckForCollisions(int index);
};

#endif //__Game_H__
