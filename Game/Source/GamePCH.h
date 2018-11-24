#ifndef __GamePCH_H__
#define __GamePCH_H__

#include "../../Libraries/Framework/Source/FrameworkPCH.h"

#include "Game/Game_Consts.h"

//audio library
#include <xaudio2.h>
#pragma comment(lib, "Xaudio2")

//Textures
#include "ImageManager/ImageManager.h"
#include "ImageManager/SpriteDataStructures.h"

//Sounds
#include "AudioManager/AudioDataStructures.h"
#include "AudioManager/AudioEngine.h"
#include "AudioManager/AudioManager.h"

//Tools
#include "Timer/Timer.h"
#include "AI Patterns/AI_Patterns.h"

//Base class objects
#include "GameObjects/TexutreObject.h"
#include "GameObjects/AtlasObject.h"
#include "GameObjects/AnimatedObject.h"

//Level objects
#include "Level/Level.h"

//Level environment objects
#include "GameObjects/Level Objects/Environment/Tile.h"
#include "GameObjects/Level Objects/Environment/Wall.h"

//Level organic objects
#include "GameObjects/Level Objects/Enemies/Enemy.h"
#include "GameObjects/Level Objects/Enemies/Slime.h"

#if ANDROID
#include "android_native_app_glue.h"
#include "UtilityAndroid.h"
#endif

#endif //__GamePCH_H__
