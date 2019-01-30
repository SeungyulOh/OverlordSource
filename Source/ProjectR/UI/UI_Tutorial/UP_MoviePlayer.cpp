// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
//todo : Build.cs include...
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "Runtime/MediaAssets/Public/MediaPlaylist.h"

#include "GlobalIntegrated.h"

#include "TimerManager.h"

#include "UtilFunctionIntegrated.h"
#include "UP_MoviePlayer.h"




void UUP_MoviePlayer::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Skip) //move Playtime?
	{
		Btn_Skip->OnClicked.RemoveDynamic(this, &UUP_MoviePlayer::End);
		Btn_Skip->OnClicked.AddDynamic(this, &UUP_MoviePlayer::End);
	}

	UpdateMediaPlayerInfo();

	CheckAutoPlay();
}

void UUP_MoviePlayer::CheckAutoPlay()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// #if !UE_BUILD_SHIPPING
// 	if ( IsValid(RGameInstance->AutoPlayManager))
// 	{
// 		if (RGameInstance->AutoPlayManager->GetAutoPlayMode() != EAutoplayModeTypeEnum::VE_None)
// 			GetWorld()->GetTimerManager().SetTimer( AutoSkipTimeHandler, this, &UUP_MoviePlayer::End, 1.0f, false);
// 	}
// #endif

	
	{
		if (RGameInstance->IsAutoPlayMode())
			GetWorld()->GetTimerManager().SetTimer(AutoSkipTimeHandler, this, &UUP_MoviePlayer::AutoPlaySkipMovie, 1.0f, false);
	}
}
/*
void UUP_MoviePlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUP_MoviePlayer::NativeDestruct()
{
	Super::NativeDestruct();
}
*/

//void UUP_MoviePlayer::MoviePlayerState(bool InIsHide)
//{
//	if (InIsHide)
//	{
//		Image_BlockScreen->SetVisibility(ESlateVisibility::Collapsed);
//		Image_MovieScreen->SetVisibility(ESlateVisibility::Collapsed);
//		//this->SetZOrder(this, -100);
//	}
//	else
//	{
//		Image_BlockScreen->SetVisibility(ESlateVisibility::Visible);
//		//Image_MovieScreen->SetVisibility(ESlateVisibility::Visible);
//		//this->SetZOrder(this, 100);
//	}
//}



//void UUP_MoviePlayer::Prepare()
//{
//	//IsCache = false;
//
//	MediaPlayer->OnMediaOpened.RemoveDynamic(this, &UUP_MoviePlayer::CheckOpen);
//	MediaPlayer->OnMediaOpened.AddDynamic(this, &UUP_MoviePlayer::CheckOpen);
//	MediaPlayer->OnMediaOpenFailed.RemoveDynamic(this, &UUP_MoviePlayer::CheckFail);
//	MediaPlayer->OnMediaOpenFailed.AddDynamic(this, &UUP_MoviePlayer::CheckFail);
//	
//
//	MediaPlayer->OpenPlaylistIndex(MoviePlayList, MoviePlayListIndex);
//	//MoviePlayerState(true);
//
//	UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial Movie player Prepare  %s"), *(MediaPlayer->GetPlayerName().ToString()));
//}

void UUP_MoviePlayer::Start()
{
	UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial Movie player start  %s"), *(MediaPlayer->GetPlayerName().ToString()));
	/*
	if (MediaPlayer && MediaPlayer->IsPlaying())
	{
		UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial Movie is aleady playing"));
		return;
	}*/

	if (IsCheckRewind == false)
		MediaPlayer->Rewind();

	//IsCheckRewind = false;
	//IsCache = false;
	//MoviePlayerState(false);

	// test......
	//if(!MediaPlayer->IsPlaying())
	if (MediaPlayer->IsReady())
	{
		Play();
		//IsAlreadyBound
		MediaPlayer->OnEndReached.RemoveDynamic(this, &UUP_MoviePlayer::End);
		MediaPlayer->OnEndReached.AddDynamic(this, &UUP_MoviePlayer::End);
	}
	else
	{
		IsLoadingMovieState = true;
		//IsCheckFail = true;
		//RGameInstance->TimerManager->SetTimerForNextTick(this, &UUP_MoviePlayer::Start);
	}
}

//void UUP_MoviePlayer::Process()
//{
//	
//}

void UUP_MoviePlayer::End()
{
	UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial Movie player end  %s"), *(MediaPlayer->GetPlayerName().ToString()));
	
	MediaPlayer->OnMediaOpened.RemoveDynamic(this, &UUP_MoviePlayer::CheckOpen);
	MediaPlayer->OnMediaOpenFailed.RemoveDynamic(this, &UUP_MoviePlayer::CheckFail);
	MediaPlayer->OnEndReached.RemoveDynamic(this, &UUP_MoviePlayer::End);
	MediaPlayer->Close();

	Btn_Skip->SetVisibility(ESlateVisibility::Collapsed);
	IsPlayState = false;
	IsCheckLoading = false;
	//MoviePlayerState(true);
	IsLoadingMovieState = false;
	RestartCount = 0;
	IsCheckFail = false;
}

void UUP_MoviePlayer::Play()
{
	UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial  UUP_MoviePlayer::Play"));

	if (IsCache)
	{
		MediaPlayer->Play();

		Image_MovieScreen->SetVisibility(ESlateVisibility::Visible);
		Btn_Skip->SetVisibility(ESlateVisibility::Visible);
		IsPlayState = true;
		IsCheckLoading = true;

		USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), MovieSound);

		//UGameplayStatics::PlaySound2D(GetWorld(), MovieSound);

		//UpdatePlayList();
		//UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial  UUP_MoviePlayer::Play Success"));
	}
	else
	{
		if (IsCheckFail)
		{
			CheckFail(MediaPlayer->GetUrl());
			return;
		}

		++RestartCount;
		if (RestartCount <= 2)
		{
			UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial  UUP_MoviePlayer::Play Restart"));

			auto RGameInstance = RGAMEINSTANCE(this);
			RGameInstance->TimerManager->SetTimerForNextTick(this, &UUP_MoviePlayer::Start);
			return;
		}
		else
		{
			UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial  UUP_MoviePlayer::Play OpenERROR"));
			CheckFail(MediaPlayer->GetUrl());
		}
	}
}

//bool UUP_MoviePlayer::GetFinishMovie()
//{
//	if (IsCheckFail)
//	{
//		CheckFail(MediaPlayer->GetUrl());
//		return true;
//	}
//
//	/*if (IsCache == false)
//	{
//		++RestartCount;
//		Prepare();
//		auto RGameInstance = RGAMEINSTANCE(this);
//		RGameInstance->TimerManager->SetTimerForNextTick(this, &UUP_MoviePlayer::Start);
//		return false;
//	}*/
//
//	if (IsLoadingMovieState)
//	{
//		++RestartCount;
//		IsLoadingMovieState = false;
//		if (RestartCount <= 2)
//		{
//			UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial  UUP_MoviePlayer::GetFinishMovie Restart"));
//
//			Start();
//		}
//		else
//		{
//			UE_LOG(LogTutorial, Log, TEXT(">>>> MediaFrameWork Tutorial  UUP_MoviePlayer::GetFinishMovie CheckFail"));
//			//End();
//			CheckFail(MediaPlayer->GetUrl());
//			return true;
//		}
//		return false;
//	}
//
//	if (IsPlayState == false
//		&& IsLoadingMovieState == false)
//		//&& IsCheckLoading)
//		return true;
//
//	return false;
//}

//bool UUP_MoviePlayer::GetCacheMovie()
//{
//	return IsCache;
//}

//void UUP_MoviePlayer::SetMediaPlayerList()
//{
//	//MediaPlayer->GetSoundWave();
//
//	UpdateMediaPlayerInfo();
//}

//void UUP_MoviePlayer::UpdatePlayList()
//{
//	++MoviePlayListIndex;
//	if (MoviePlayListIndex >= MaxMoviePlayList)
//		MoviePlayListIndex = 0;
//}

void UUP_MoviePlayer::UpdateMediaPlayerInfo()
{
	if (MoviePlayList)
	{
		MaxMoviePlayList = MoviePlayList->Num();
	}
}


void UUP_MoviePlayer::CheckOpen(FString InUrl)
{
	IsCache = true;
	IsCheckRewind = false;
}

void UUP_MoviePlayer::CheckFail(FString InUrl)
{
	//UE_LOG();

	IsCheckFail = true;

	End();
}

void UUP_MoviePlayer::AutoPlaySkipMovie()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->SequenceManager))
	{
		if (RGameInstance->SequenceManager->IsPlaying())
		{
			RGameInstance->SequenceManager->StopSequence();
			RGameInstance->SequenceManager->CleanUpSequence();
		}
		else
		{
			FString strName = UUtilFunctionLibrary::GetPlayingMatineeActorName(this);
			UUtilFunctionLibrary::SkipMatineeActor(this, strName);
		}
	}
}
