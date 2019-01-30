// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Skip.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "TimerManager.h"

void UUP_Skip::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Skip)
	{
		Button_Skip->OnClicked.RemoveDynamic(this, &UUP_Skip::OnClick_Skip);
		Button_Skip->OnClicked.AddDynamic(this, &UUP_Skip::OnClick_Skip);
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (RGameInstance->IsAutoPlayMode())
			GetWorld()->GetTimerManager().SetTimer(AutoSkipTimeHandler, this, &UUP_Skip::OnClick_Skip, 1.0f, false);
	}

}

void UUP_Skip::NativeDestruct()
{
	SkipSequenceName = FName();

	Super::NativeDestruct();
}

void UUP_Skip::OnClick_Skip()
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