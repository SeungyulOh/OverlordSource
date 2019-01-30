// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UIMediaFrameWorkManager.h"
#include "UI/MediaFrameWork/UP_MediaFrameWorkPlayer.h"
//#include "UI/MediaFrameWork/MediaFrameWorkAmbientSound.h"

#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"


#include "UI/UIDefs.h"


UUIMediaFrameWorkManager::UUIMediaFrameWorkManager()
{
// 	static ConstructorHelpers::FObjectFinder<AAmbientSound> ambientSound(TEXT("CurveFloat'/Game/Data/GameSetting/Curve_ActorVibration.Curve_ActorVibration'"));
// 	if (Curve.Object)
// 	{
// 		CurveVibration = (UCurveFloat*)Curve.Object;
// 	}

}

//#include "Runtime/Core/Public/Android/AndroidFile.h"


void UUIMediaFrameWorkManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;
}

bool UUIMediaFrameWorkManager::IsPlaying()
{
	if (!IsValid(MediaPlayerWidget))
	{
		return false;
	}

	return MediaPlayerWidget->IsPlaying;
}

void UUIMediaFrameWorkManager::ShowMedia(FName  MediaName, bool IsVisibleSkipButton)
{
	if (!MediaName.IsValid())
		return;
	
	if (!IsValid(MediaPlayerWidget))
	{
		if( !CreateUIWidget())
			return;
	}

	if (!MediaPlayerWidget->IsInViewport())
	{
		MediaPlayerWidget->AddToViewport(MEDIA_ZOEDER);
	}

	
	FMediaTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetMediaTableRow(MediaName);
	
	if(TableInfo == nullptr)
		return;
	
	CurrentUrl = TableInfo->MediaUrl;

	if(CurrentUrl.IsEmpty())
		return;

	CheckAmbientSound();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager))
	{
//		RGameInstance->EventManager->OnMediaFrameWorkRequestToPlay.Broadcast(CurrentUrl);

		if (RGameInstance->EventManager->OnMediaFrameWorkMediaClose.IsBound() == false)
		{
			RGameInstance->EventManager->OnMediaFrameWorkMediaClose.AddDynamic(this, &UUIMediaFrameWorkManager::TestEnd);
		}
	}

	bool bOpened = MediaPlayerWidget->OpenUrl(CurrentUrl, IsVisibleSkipButton);

	if (bOpened)
	{
		if ( RGameInstance->TimerManager != nullptr)
		{
			//UE_LOG(LogUI, Log, TEXT("UUIMediaFrameWorkManager::ShowMedia => SetTimer"));
			//RGameInstance->TimerManager->SetTimer(TimerHandle_MoviePlayingCheck, this, &UUIMediaFrameWorkManager::CallbackTimer, RGameInstance->GameEnvironmentVar.Movie_Callback_WaitTime);

			if (TableInfo->StartSubtitle != 0)
			{
				SetSubtitles(MediaName);
			}
		}
		else
		{
			bOpened = false;
		}
	}

	if (!bOpened)
	{
		UE_LOG(LogUI, Log, TEXT("UUIMediaFrameWorkManager::ShowMedia => MediaFile open failed"));

		CloseMedia();
	}
	
}

void UUIMediaFrameWorkManager::CloseMedia()
{
	if (!IsValid(MediaPlayerWidget))
		return; 
	
	//MediaPlayerWidget->Stop();
	if (MediaPlayerWidget->IsInViewport())
	{
		MediaPlayerWidget->RemoveFromViewport();
	}
	MediaPlayerWidget = nullptr;
}

void UUIMediaFrameWorkManager::TestEnd(FString InUrl)
{
	CallbackTimer();
}

void UUIMediaFrameWorkManager::CallbackTimer()
{
	CloseMedia();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->HttpClient->OnEndIntroMovieDelegate.Broadcast();
	//RGameInstance->EventManager->OnMediaFrameWorkMediaClose.Broadcast(CurrentUrl);
	return;
	UE_LOG(LogUI, Log, TEXT("UUIMediaFrameWorkManager::CallbackTimer"));

	ClearTimerHandle();
	
	if (IsValid( RGameInstance ))
	{
		if (IsValid(MediaPlayerWidget) && MediaPlayerWidget->IsPlaying == false)
		{
			UE_LOG(LogUI, Log, TEXT("UUIMediaFrameWorkManager::CallbackTimer => RGameInstance->EventManager->OnMediaFrameWorkStop.Broadcast"));

			CloseMedia();
			RGameInstance->EventManager->OnMediaFrameWorkMediaStop.Broadcast(CurrentUrl);
		}
	}
}

void UUIMediaFrameWorkManager::ClearTimerHandle()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( RGameInstance->TimerManager)
	{
		if(RGameInstance->TimerManager->IsTimerActive(TimerHandle_MoviePlayingCheck))
			RGameInstance->TimerManager->ClearTimer(TimerHandle_MoviePlayingCheck);
	}
}

void UUIMediaFrameWorkManager::CheckAmbientSound()
{		
	TArray<AActor*> OutActors;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UGameplayStatics::GetAllActorsOfClass(RGameInstance->GetWorld(), AAmbientSound::StaticClass(), OutActors);

	if (OutActors.Num())
	{
		return;
	}
	else
	{
		FResourceCustomActorTableInfo* CustomActorTableInfo = UTableManager::GetInstancePtr()->GetResourceCustomActorRow(MediaAmbientSoundName);
		if (CustomActorTableInfo)
		{			
			AActor* AmbientSoundActor = RGameInstance->GetWorld()->SpawnActor<AActor>(CustomActorTableInfo->GetBlueprintClass(), FVector::ZeroVector , FRotator::ZeroRotator);
			if (!IsValid(AmbientSoundActor))
			{
				UE_LOG(LogUI, Warning, TEXT("UUIMediaFrameWorkManager::CheckAmbientSound => AmbientSound failed Spawn..............."));

			}
		}
	}
}

void UUIMediaFrameWorkManager::SetSubtitles(FName MediaName)
{
	if (IsValid(MediaPlayerWidget))
	{
		MediaPlayerWidget->ClearSubtitleInfos();
		
		FMediaTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetMediaTableRow(MediaName);
		int32 EndSubtitle = TableInfo->EndSubtitle;
		for (int32 i = TableInfo->StartSubtitle; i <= EndSubtitle; ++i)
		{
			FString Test = FString::FromInt(i);
			FName Element = FName(*Test);
			FSubtitleTableInfo* SubtitleInfo = UTableManager::GetInstancePtr()->GetSubtitleTableRow(Element);
			if (SubtitleInfo)
			{
				MediaPlayerWidget->SetSubtitleInfo(SubtitleInfo->StartTime, SubtitleInfo->EndTime, SubtitleInfo->StringKey);
			}
		}

		/*TArray<FName> tableInfoIDs = UTableManager::GetInstancePtr()->GetSubtitleTableRowsNames();
		if (tableInfoIDs.Num() > 0)
		{
			for (auto ID : tableInfoIDs)
			{
				FSubtitleTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetSubtitleTableRow(ID);
				if (TableInfo && TableInfo->MediaID == MediaName)
				{
					MediaPlayerWidget->SetSubtitleInfo( TableInfo->StartTime, TableInfo->EndTime, TableInfo->StringKey);
				}
			}
		}*/
	}
}

bool UUIMediaFrameWorkManager::CreateUIWidget()
{
	if (!IsValid(MediaPlayerWidget))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		UUserWidget* Widget = UUtilFunctionLibrary::CreateUserWidget(RGameInstance->GetWorld(), WIDGETID_MEDIAFRAMEWORKUI);
		if (Widget)
		{
			MediaPlayerWidget = Cast<UUP_MediaFrameWorkPlayer>(Widget);
			MediaPlayerWidget->AddToViewport(MEDIA_ZOEDER);
			
			return true;
		}
	}

	return false;
}
