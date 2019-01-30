// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
//#include "CafeSDKPluginManager.h"
//
// #include "GlobalIntegrated.h"
// 
// 
// #if USE_CAFESDK_PLUGIN
// #include "CafeSDKPlugin.h"
// #include "CafeSdkBlueprintLibrary.h"
// #include "CafeSdkStatisticsBlueprintLibrary.h"
// #include "NaverIdLoginBlueprintLibrary.h"
// #endif
// 
// #define CAFESEK_FOR_DOMESTIC 0
// 
// void UCafeSDKPluginManager::Initialize(/*URGameInstance* InGameInstance*/)
// {
// 	//RGameInstance = InGameInstance;
// 
// #if USE_CAFESDK_PLUGIN
// 	#if CAFESEK_FOR_DOMESTIC
// 		InitDomestic();
// 	#else
// 		InitGlobal();
// 	#endif
// 
// 	UseCafeSDK = true;
// #else
// 	UseCafeSDK = false;
// #endif
// }
// 
// void UCafeSDKPluginManager::InitDomestic()
// {
// 	 
// }
// 
// void UCafeSDKPluginManager::InitGlobal()
// {	
// #if USE_CAFESDK_PLUGIN
// 	UCafeSdkBlueprintLibrary::InitGlobal("iWJfVMF_nvgbBjoTiwTq", 1013493);
// 	InitTheme();
// 	InitCallbackFunctions();
// #endif
// }
// 
// void UCafeSDKPluginManager::InitTheme()
// {
// #if USE_CAFESDK_PLUGIN
// 	/*	
// 	* Use css color format, but don't use alpha vlaue.
// 	*/
// 	UCafeSdkBlueprintLibrary::SetThemeColor("#00c73c", "#44484e");
// 	UCafeSdkBlueprintLibrary::SyncGameUserId("SyncGameUserId");
// 
// 	//UCafeSdkBlueprintLibrary::ShowWidgetWhenUnloadSdk(false);
// #endif
// }
// 
// /*
// * channel code
// public static final String KOREAN = "ko";
// public static final String ENGLISH = "en";
// public static final String JAPANESE = "ja";
// public static final String CHINESE_SIMPLIFIED = "zh_CN";
// public static final String CHINESE_TRADITIONAL = "zh_TW";
// public static final String THAI = "th";
// public static final String SPANISH = "es";
// public static final String GERMAN = "de";
// public static final String FRENCH = "fr";
// public static final String RUSSIAN = "ru";
// public static final String INDONESIAN = "id";
// public static final String VIETNAMESE = "vi";
// public static final String ITALIAN = "it";
// public static final String PORTUGUESE = "pt";
// public static final String TURKISH = "tr";
// */
// 
// void UCafeSDKPluginManager::InitChannel()
// {
// #if USE_CAFESDK_PLUGIN
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		FString ChannelCode = "en";
// 		ELanguageType LangageType = UGameOptionManager::GetLanguageType();
// 		switch (LangageType)
// 		{
// 		case ELanguageType::VE_KOR:
// 			ChannelCode = "kr";
// 			break;
// 		case ELanguageType::VE_ENG:
// 			ChannelCode = "en";
// 			break;
// 		case ELanguageType::VE_CHN:
// 			ChannelCode = "zh_CN";
// 			break;
// 		case ELanguageType::VE_TWN:
// 			ChannelCode = "zh_TW";
// 			break;
// 		}
// 
// 		UCafeSdkBlueprintLibrary::SetChannelCode(ChannelCode);
// 	}
// #endif
// }
// 
// void UCafeSDKPluginManager::InitCallbackFunctions()
// {
// #if USE_CAFESDK_PLUGIN
// 	GEngine->GameViewport->OnScreenshotCaptured().AddUObject(this, &UCafeSDKPluginManager::OnScreenshotCaptured);
// #endif
// }
// 
// 
// 
// #include "HighResScreenshot.h"
// #include "ImageUtils.h"
// void UCafeSDKPluginManager::OnScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Colors)
// {
// #if USE_CAFESDK_PLUGIN
// 	auto Bitmap = TArray<FColor>(Colors);
// 	
// 	FString ScreenShotName = FScreenshotRequest::GetFilename();
// 	UE_LOG(LogUI, Log, TEXT("Screenshot saved = %s"), *(ScreenShotName));
// 	GetHighResScreenshotConfig().MergeMaskIntoAlpha(Bitmap);
// 
// 	FIntRect SourceRect(0, 0, GScreenshotResolutionX, GScreenshotResolutionY);
// 	if (GIsHighResScreenshot)
// 	{
// 		SourceRect = GetHighResScreenshotConfig().CaptureRegion;
// 	}
// 
// 	if (!FPaths::GetExtension(ScreenShotName).IsEmpty())
// 	{
// 		ScreenShotName = FPaths::GetBaseFilename(ScreenShotName, false);
// 		ScreenShotName += TEXT(".png");
// 	}
// 
// 	// Save the contents of the array to a png file.
// 	TArray<uint8> CompressedBitmap;
// 	FImageUtils::CompressImageArray(Width, Height, Bitmap, CompressedBitmap);
// 	FFileHelper::SaveArrayToFile(CompressedBitmap, *ScreenShotName);
// 	
// #if PLATFORM_ANDROID
// 
// 	extern FString GExternalFilePath;
// 	FString BasePath = GExternalFilePath;	
// 	UE_LOG(LogUI, Log, TEXT("BasePath = %s"), *(BasePath));
// 	FString Result = ScreenShotName;
// 	Result.ReplaceInline(TEXT("../"), TEXT(""));
// 	Result.ReplaceInline(TEXT(".."), TEXT(""));
// 	FString ScreenshotPath = FPaths::ConvertRelativePathToFull(BasePath, Result);
// 	UE_LOG(LogUI, Log, TEXT("ScreenshotPath = %s"), *(ScreenshotPath));
// 	UCafeSdkBlueprintLibrary::StartImageWrite(TEXT("file://") + ScreenshotPath);
// #elif PLATFORM_IOS
// 	FString Result = ScreenShotName;
// 	Result.ReplaceInline(TEXT("../"), TEXT(""));
// 	Result.ReplaceInline(TEXT(".."), TEXT(""));
// 	Result.ReplaceInline(FPlatformProcess::BaseDir(), TEXT(""));
// 
// 	FString WritePathBase = FString([NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]) + TEXT("/");
// 	UCafeSdkBlueprintLibrary::StartImageWrite(WritePathBase + Result);
// #endif
// 
// #endif // USE_CAFESDK_PLUGIN
// }
// 
