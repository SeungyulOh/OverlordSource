
#pragma once
#include "EngineMinimal.h"

//#include "UpdateService/Struct/OnlineAccountInfo.h"

UENUM(BlueprintType)
enum class ELoginProcess : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_NeedLoginId			UMETA(DisplayName = "NeedLoginId"),
	VE_NeedNickName			UMETA(DisplayName = "NeedNickName"),
	VE_NeedReTry			UMETA(DisplayName = "NeedReTry"),
	VE_NeedPlatformLogin	UMETA(DisplayName = "NeedPlatformLogin"),
	VE_NeedCheckAccount		UMETA(DisplayName = "NeedCheckAccount"),
	VE_OK					UMETA(DisplayName = "OK"),
};


UENUM(BlueprintType)
enum class EPublisher : uint8
{
	Kokomo = 0,
	Facebook = 1,
	Google = 2,
	IOS = 3,
	NZIN = 4,
	Guest = 5
};


#define PUBLISHERID_GOOGLE		TEXT("Google")
#define PUBLISHERID_GOOGLEPLAY	TEXT("GooglePlay")

#define PUBLISHERID_GOOGLEPLUS	TEXT("googleplus")
#define PUBLISHERID_IOS			TEXT("IOS")			//todo - need check
#define PUBLISHERID_FACEBOOK	TEXT("Facebook")	//todo - need check
#define PUBLISHERID_NZIN		TEXT("nzine")		//none
#define PUBLISHERID_DEV			TEXT("kokomo")
