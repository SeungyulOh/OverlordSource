// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_MediaFrameWorkPlayer.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"

#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "Runtime/MediaAssets/Public/FileMediaSource.h"




#include "UI/Dialogue/UP_Dialogue.h"

#include "UI/UI_Common/UP_CommonPopup.h"

#include "TimerManager.h"


void UUP_MediaFrameWorkPlayer::NativeConstruct()
{
	Super::NativeConstruct();

	if (NativeMediaPlayer)
	{
		NativeMediaPlayer->OnMediaOpened.AddUniqueDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayerMediaOpend);
		NativeMediaPlayer->OnMediaOpenFailed.AddUniqueDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayerMediaOpendFailed);

		NativeMediaPlayer->OnPlaybackResumed.AddUniqueDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayStart);
		NativeMediaPlayer->OnEndReached.AddUniqueDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayEnd);
		NativeMediaPlayer->OnMediaClosed.AddUniqueDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaClosed);
	}

	if (SkipButton)
	{
		SkipButton->OnClicked.AddDynamic(this, &UUP_MediaFrameWorkPlayer::OnSkipButtonClick);
	}

	CheckAutoPlay();
}

void UUP_MediaFrameWorkPlayer::NativeDestruct()
{
	if (NativeMediaPlayer)
	{
		NativeMediaPlayer->OnMediaOpened.RemoveDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayerMediaOpend);
		NativeMediaPlayer->OnMediaOpenFailed.RemoveDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayerMediaOpendFailed);

		NativeMediaPlayer->OnPlaybackResumed.RemoveDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayStart);
		NativeMediaPlayer->OnEndReached.RemoveDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaPlayEnd);
		NativeMediaPlayer->OnMediaClosed.RemoveDynamic(this, &UUP_MediaFrameWorkPlayer::CallbackMediaClosed);

		NativeMediaPlayer->Close();
	}

	if (SkipButton)
	{
		SkipButton->OnClicked.RemoveDynamic(this, &UUP_MediaFrameWorkPlayer::OnSkipButtonClick);
	}

	Super::NativeDestruct();
}

void UUP_MediaFrameWorkPlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsSequenceType)
		return;

	if(IsPlaying == false || IsReadySubtitle == false)
		return;

	PlaybackEllapsedTime += InDeltaTime;

	if (SubtitleInfos.Num() > 0)
	{
		if (PlaybackEllapsedTime >= SubtitleInfos[0].StartTime)
		{
			CurrentSubtitleInfo = SubtitleInfos[0];
			SubtitleInfos.RemoveAt(0);
			ShowSubtitle();
		}
	}

	if (CurrentSubtitleInfo.IsValid)
	{
		if (PlaybackEllapsedTime >= CurrentSubtitleInfo.EndTime)
			CloseSubtitle();
	}
	else
	{
		if (SubtitleInfos.Num() == 0)
			IsReadySubtitle = false;
	}
	

	
}

bool UUP_MediaFrameWorkPlayer::OpenUrl(FString url, bool visibleSkipBtn)
{
	IsPlaying = false;

	if (IsValid(SkipButton))
	{
		if(visibleSkipBtn)
			SkipButton->SetVisibility(ESlateVisibility::Visible);
		else
			SkipButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (NewUrl == url)
	{
		if (IsValid(NativeMediaPlayer))
		{
			if (!NativeMediaPlayer->Rewind())
			{
				UE_LOG(LogUI, Warning, TEXT("%s Media rewind failed.........."), *NewUrl);
			}

			if (!NativeMediaPlayer->IsPlaying())
			{
				if (!NativeMediaPlayer->Play())
				{					
					UE_LOG(LogUI, Warning, TEXT("%s Media file play failed.........."), *NewUrl);
					IsPlaying = false;

					return false;
				}
			}

			IsPlaying = true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (NativeFileMediaSource && NativeMediaPlayer)
		{
			if (NativeMediaPlayer->IsPlaying())
			{
				OnSkipButtonClick();
			}

			NewUrl = url;
			NativeFileMediaSource->SetFilePath(NewUrl);
			
			if (!NativeMediaPlayer->OpenSource(NativeFileMediaSource))			
			{
				IsPlaying = false;
				UE_LOG(LogUI, Warning, TEXT("%s Media file open failed.........."), *NewUrl);

				return false;
			}
		}		
	}

	return true;
}


void UUP_MediaFrameWorkPlayer::OnSkipButtonClick()
{
	StopMedia();
}

void UUP_MediaFrameWorkPlayer::StopMedia()
{
	if (NativeMediaPlayer)
	{
		NativeMediaPlayer->Pause();
		NativeMediaPlayer->Close();
		CallbackMediaPlayEnd();
	}

}

void UUP_MediaFrameWorkPlayer::CallbackMediaPlayStart()
{
	UE_LOG(LogUI, Log, TEXT("Media Play start...."));

	IsPlaying = true;
	SetVisibility(ESlateVisibility::Visible);

	if (SubtitleInfos.Num() > 0)
		IsReadySubtitle = true;


	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (IsValid(RGameInstance->EventManager))
		RGameInstance->EventManager->OnMediaFrameWorkMediaPlay.Broadcast(NewUrl);
}

void UUP_MediaFrameWorkPlayer::CallbackMediaPlayEnd()
{
	UE_LOG(LogUI, Log, TEXT("Media Play end..."));

	IsPlaying = false;
	SetVisibility(ESlateVisibility::Collapsed);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->EventManager))
		return;

	RGameInstance->EventManager->OnMediaFrameWorkMediaStop.Broadcast(NewUrl);
	if (RGAMEINSTANCE(this)->EventManager)
		RGAMEINSTANCE(this)->EventManager->OnEndMediaEvent.Broadcast();

	CallbackMediaClosed();
}

void UUP_MediaFrameWorkPlayer::CallbackMediaClosed()
{
	UUtilFunctionLibrary::ResumeBGSound();
	UE_LOG(LogUI, Log, TEXT("Media close....."), *NewUrl);
	IsPlaying = false;
	SetVisibility(ESlateVisibility::Collapsed);
	ClearSubtitleInfos();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->EventManager))
		return;

	RGameInstance->EventManager->OnMediaFrameWorkMediaClose.Broadcast(NewUrl);
}

void UUP_MediaFrameWorkPlayer::CallbackMediaPlayerMediaOpend(FString url)
{
	UE_LOG(LogUI, Log, TEXT("%s file open success!!!!!!!"), *url);

	IsPlaying = true;

	UUtilFunctionLibrary::PauseBGSound();
		

	if (NativeMediaPlayer)
 	{
		if (!NativeMediaPlayer->Play())
		{
			IsPlaying = false;
			ClearSubtitleInfos();

			UE_LOG(LogUI, Warning, TEXT("%s Media file play failed.........."), *NewUrl);
		}
	}
	
}

void UUP_MediaFrameWorkPlayer::CallbackMediaPlayerMediaOpendFailed(FString url)
{
	IsPlaying = false;
	UE_LOG(LogUI, Log, TEXT("%s file open failed"), *url);
}

void UUP_MediaFrameWorkPlayer::SetSubtitleInfo(float startTime, float endTime, FName stringID)
{
	if( startTime == 0.0f || endTime == 0.0f)
		return;

	FSubtitleInfo info;
	info.IsValid	= true;
	info.StartTime	= startTime;
	info.EndTime	= endTime;
	info.Subtitle	= UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Subtitle, stringID);

	SubtitleInfos.Emplace(info);

	IsReadySubtitle = true;
}

void UUP_MediaFrameWorkPlayer::ClearSubtitleInfos()
{
	SubtitleInfos.Reset();
	IsReadySubtitle = false;
	PlaybackEllapsedTime = 0;
	CurrentSubtitleInfo = FSubtitleInfo();
	if (IsValid(Text_Subtitle))
		Text_Subtitle->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_MediaFrameWorkPlayer::ShowSubtitle()
{
	if (IsValid(Text_Subtitle))
	{
		Text_Subtitle->SetVisibility(ESlateVisibility::Visible);
		Text_Subtitle->SetText(CurrentSubtitleInfo.Subtitle);	
	}
}

void UUP_MediaFrameWorkPlayer::CloseSubtitle()
{
	CurrentSubtitleInfo = FSubtitleInfo();
	if (IsValid(Text_Subtitle))
	{
		Text_Subtitle->SetVisibility(ESlateVisibility::Collapsed);		
	}
}

void UUP_MediaFrameWorkPlayer::ChangeSequenceSubtitleType()
{
	//IsSequenceType = true;
	IsPlaying = true;
	IsReadySubtitle = true;

	if(Image_BG)
		Image_BG->SetVisibility(ESlateVisibility::Collapsed);

	if (Image_MovieViewer)
		Image_MovieViewer->SetVisibility(ESlateVisibility::Collapsed);

	if (SkipButton)
	{
		SkipButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_MediaFrameWorkPlayer::SetSequenceSubtitle(FName InSubtitle)
{
	IsSequenceType = true;

	if (InSubtitle.IsNone())
	{
		CloseSubtitle();
	}
	else
	{
		FSubtitleTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetSubtitleTableRow(InSubtitle);
		CurrentSubtitleInfo.Subtitle = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Subtitle, TableInfo->StringKey);
		ShowSubtitle();
	}
}

void UUP_MediaFrameWorkPlayer::CheckAutoPlay()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (RGameInstance->IsAutoPlayMode())
			GetWorld()->GetTimerManager().SetTimer(AutoSkipTimeHandler, this, &UUP_MediaFrameWorkPlayer::OnSkipButtonClick, 1.0f, false);
	}
}