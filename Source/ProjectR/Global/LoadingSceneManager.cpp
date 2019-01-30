// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LoadingSceneManager.h"

#include "UI/UP_LoadingScene.h"
#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "UtilFunctionIntegrated.h"


void ULoadingSceneManager::ShowLoadingScene(EGamePlayModeTypeEnum InContentType)
{
//#if !WITH_EDITOR
//	return;
//#endif
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (InContentType == EGamePlayModeTypeEnum::VE_None)
		return;

	if (!IsValid(Widget))
	{
		FWidgetBluePrintTableInfo* Result = UTableManager::GetInstancePtr()->GetWidgetBluePrintRow(LoadingScreenWidgetKey);
		Widget = CreateWidget<UUP_LoadingScene>(RGAMEINSTANCE(this)->GetWorld(), Result->GetWidgetClass());
	}

	auto UI_Loading = Cast<UUP_LoadingScene>(Widget);
	if (!IsValid(UI_Loading))
		return;

	if (IsValid(Widget) && Widget->IsInViewport() == false)
	{
		Widget->AddToViewport( 10000 );		
	}

	if (CurrentContentType != InContentType)
	{
		UI_Loading->MatchLoadingType(InContentType);
		CurrentContentType = InContentType;
	}
	else
	{
		UI_Loading->SetLoadingImage(LoadingImageID);
		UI_Loading->SetLoadingText(LoadingTextID);
	}
}
 
void ULoadingSceneManager::HideLoadingScene(float fTime)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (fTime > 0.0f)
	{
		RGAMEINSTANCE(this)->TimerManager->SetTimer(OnHideLoadingSceneHandler, this, &ULoadingSceneManager::OnHideLoadingScene, fTime, false);
	}
	else
	{
		OnHideLoadingScene();
	}

	CurrentContentType = EGamePlayModeTypeEnum::VE_None;
}

void ULoadingSceneManager::OnHideLoadingScene()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	RGAMEINSTANCE(this)->TimerManager->ClearTimer(OnHideLoadingSceneHandler);

	//EndLoadingScreen();

	if (IsValid(Widget))
	{
		Widget->RemoveFromViewport();
		/*auto UI_Loading = Cast<UUP_LoadingScene>(Widget);
		if(UI_Loading)
			UI_Loading->Temp();*/
		//Widget->BeginDestroy();
		Widget = nullptr;
	}
}

// void ULoadingSceneManager::CacheForAsync()
// {
// 	if (RGAMEINSTANCE(this) == nullptr)
// 		return;
// 
// 	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
// 
// 	if (!IsValid(Widget))
// 	{
// 		FWidgetBluePrintTableInfo* Result = UTableManager::GetInstancePtr()->GetWidgetBluePrintRow(LoadingScreenWidgetKey);
// 		Widget = CreateWidget<UUP_LoadingScene>(RGAMEINSTANCE(this)->GetWorld(), Result->GetWidgetClass());
// 	}
// 
// 	auto MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(BaseStageInfo->MapTableKey);
// 	if (MapTableInfo == nullptr)
// 		return;
// 
// 	//LoadingTableKeys = MapTableInfo->LoadingTableKeys;
// 	int32 ImageSelectedNumber = FMath::RandHelper(MapTableInfo->LoadingTableKeys.Num());
// 	auto LoadingTableInfo = UTableManager::GetInstancePtr()->GetLoadingRow(MapTableInfo->LoadingTableKeys[ImageSelectedNumber]);
// 	if (LoadingTableInfo == nullptr)
// 	{
// 		return;
// 	}
// 	LoadingImageID = LoadingTableInfo->TextureID;
// 	CurrentContentType = EGamePlayModeTypeEnum::VE_PlayTest;
// }

void ULoadingSceneManager::SetProgressBarPercent(float InValue)
{
	if (IsValid(Widget))
	{
		auto UI_Loading = Cast<UUP_LoadingScene>(Widget);
		if (!IsValid(UI_Loading))
			return;

		UI_Loading->SetProgressBarPercent(InValue);
	}
}

void ULoadingSceneManager::Initialize(void)
{
	//RGameInstance = InRGameInstance;

	//WidgetBlueprint'/Game/UMG/Loading/UI_Loading.UI_Loading'

//#if !WITH_EDITOR
//	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULoadingSceneManager::BeginLoadingScreen);
//	FCoreUObjectDelegates::PostLoadMap.AddUObject(this, &ULoadingSceneManager::EndLoadingScreen);
//#endif
}

void ULoadingSceneManager::BeginLoadingScreen(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenMovie;
	LoadingScreenMovie.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreenMovie.bMoviesAreSkippable = true;
	LoadingScreenMovie.MinimumLoadingScreenDisplayTime = -1;
	LoadingScreenMovie.MoviePaths.Emplace(TEXT("loading")); /*This is the movie name*/
	LoadingScreenMovie.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	//SNew(SImage)
	//LoadingScreenMovie.bWaitForManualStop = true;
	LoadingScreenMovie.PlaybackType = EMoviePlaybackType::MT_LoadingLoop;

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenMovie);
	//GetMoviePlayer()->PlayMovie();
}

void ULoadingSceneManager::EndLoadingScreen()
{
	GetMoviePlayer()->StopMovie();
}