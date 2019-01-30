
#pragma once

// Needed because AndroidFile platform only lets access to external storage; public

#if PLATFORM_ANDROID
#include "UpdateService/Android/AndroidPlatformAuth.h"
typedef FAndroidPlatformAuth FPlatformAuth;
//#elif PLATFORM_IOS
//#include "UpdateService/Android/AndroidPlatformAuth.h"
//typedef FAndroidPlatformAuth FPlatformAuth;
#else
#include "UpdateService/Generic/GenericPlatformAuth.h"
typedef FGenericPlatformAuth FPlatformAuth;
#endif
