#ifndef __GamePCH_H__
#define __GamePCH_H__

#include "../../Libraries/Framework/Source/FrameworkPCH.h"

#include "Game/Game_Consts.h"

//Textures
#include "ImageManager/ImageManager.h"
#include "ImageManager/SpriteDataStructures.h"

//Tools
#include "Timer/Timer.h"

//Base class objects
#include "GameObjects/TexutreObject.h"
#include "GameObjects/AtlasObject.h"
#include "GameObjects/AnimatedObject.h"

//Level objects
#include "Level/Level.h"

//Level environment objects
#include "GameObjects/Level Objects/Environment/Tile.h"


#if ANDROID
#include "android_native_app_glue.h"
#include "UtilityAndroid.h"
#endif

#endif //__GamePCH_H__
