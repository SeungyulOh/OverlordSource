// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Global/EventManager.h"
#include "Global/RGameInstance.h"

void ABaseLobbyStateActor::EnterSublevel()
{
	// TODO: provisional
	CurrentSubstate = ETowerLobbySubstateEnum::NormalView;
}

void ABaseLobbyStateActor::LeaveSublevel()
{
	CurrentSubstate = ETowerLobbySubstateEnum::None;
}

void ABaseLobbyStateActor::OnLeaveSublevelComplete()
{
	ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	ABaseLobbyStateActor* BaseLSA = CurrentMode->GetLobbyStateActor(CurrentMode->CurrentLobbyState);
	if(BaseLSA)
		BaseLSA->SetVisibilityAll(true);
	// resume sequence
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	RGameInstance->EventManager->OnLobbyEnterUIEvent.Broadcast();
}

void ABaseLobbyStateActor::BeginSublevel()
{
	ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// Show the main widget

	//ReceiveBeginSublevel();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<AActor>(CurrentMode->OutdoorLobbyStateAct));
}

void ABaseLobbyStateActor::EndSublevel()
{
	// Remove the main widget
	ReceiveEndSublevel();
}

void ABaseLobbyStateActor::SetVisibilityAll(bool IsVisible)
{
	TArray<class ULevelStreaming*> LSArray = RGAMEINSTANCE(GEngine)->GetWorld()->StreamingLevels;
	for (size_t i = 0; i < LSArray.Num(); ++i)
	{
		LSArray[i]->bShouldBeVisible = IsVisible;
	}
}

FLatentActionInfo ABaseLobbyStateActor::InitLatentActionInfo(FName executionFunction, UObject* callbackTarget, int32 uid, int32 linkage)
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.ExecutionFunction = executionFunction;
	LatentActionInfo.CallbackTarget = callbackTarget;
	LatentActionInfo.UUID = uid;
	LatentActionInfo.Linkage = linkage;

	return LatentActionInfo;
}



void ABaseLobbyStateActor::Nop()
{
	// Nop
}
