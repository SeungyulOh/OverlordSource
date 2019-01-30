// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"

#include "NotiDialogueTrigger.h"

void UNotiDialogueTrigger::CheckTrigger()
{/*
	auto BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	AActor* TargetActor = BasePlayerController->GetSelectedCharacter();
	FVector Source = TargetActor->GetActorLocation();
	Source.Z = 0;
	FVector Dest = Location;
	Dest.Z = 0;
	float Dist = FVector::Dist(Source, Dest);
	if (Dist >= 100)
	{
		//if (IsOnTrigger)
		//	return;

		//on dialog
	}
	else
	{
		//if (IsOnTrigger == false)
		//	return;

		//off dialog
	}*/
}