#ifndef __GamePCH_H__
#define __GamePCH_H__

#include "../../Libraries/Framework/Source/FrameworkPCH.h"

#include "Game/Game_Consts.h"
#include "ImageManager/ImageManager.h"
#include "GameObjects/TexutreObject.h"
#include "GameObjects/AtlasObject.h"




#if ANDROID
#include "android_native_app_glue.h"
#include "UtilityAndroid.h"
#endif

#endif //__GamePCH_H__
