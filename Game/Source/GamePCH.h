#ifndef __GamePCH_H__
#define __GamePCH_H__

#include "../../Libraries/Framework/Source/FrameworkPCH.h"

#include "Game/Game_Consts.h"
#include "Game/Game.h"

//audio library
#include <xaudio2.h>
#pragma comment(lib, "Xaudio2")

//Textures
#include "ImageManager/ImageManager.h"
#include "ImageManager/SpriteDataStructures.h"

//Meshes
#include "Mesh/Mesh.h"
#include "Mesh/TextMesh.h"
#include "Mesh/Canvas.h"
#include "Mesh/ParticleRenderer.h"

//Sounds
#include "AudioManager/AudioDataStructures.h"
#include "AudioManager/AudioEngine.h"
#include "AudioManager/AudioManager.h"

//Tools
#include "Timer/Timer.h"
#include "AI Patterns/AI_Patterns.h"
#include "AI Patterns/AStarPathFinding.h"
#include "GameObjects/PlayerController.h"

//Base class objects
#include "GameObjects/TexutreObject.h"
#include "GameObjects/AtlasObject.h"
#include "GameObjects/AnimatedObject.h"
#include "GameObjects/TextObject.h"
#include "GameObjects/Camera.h"

//interfaces
#include "GameObjects/HUD_Element.h"
#include "GameObjects/ParticleEmitter.h"

//Level objects
#include "Level/Level.h"

//Level environment objects
#include "GameObjects/Level Objects/Environment/Tile.h"
#include "GameObjects/Level Objects/Environment/Wall.h"

//Level organic objects
#include "GameObjects/Level Objects/Enemies/Enemy.h"
#include "GameObjects/Level Objects/Enemies/Slime.h"
#include "GameObjects/Level Objects/Enemies/Skeleton.h"
#include "GameObjects/Player.h"

//HUD objects
#include "GameObjects/HUD/HUD_Text.h"
#include "GameObjects/HUD/HUD_Sprite.h"
#include "GameObjects/HUD/HUD_ScoreDisplay.h"

#if ANDROID
#include "android_native_app_glue.h"
#include "UtilityAndroid.h"
#endif

#endif //__GamePCH_H__
