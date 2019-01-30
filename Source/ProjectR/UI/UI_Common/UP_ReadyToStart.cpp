// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UtilFunctionIntegrated.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "GlobalIntegrated.h"

#include "UP_ReadyToStart.h"


void UUP_ReadyToStart::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

	StartCountAnim = GetAni("start_count");
	StartPhaseAnim = GetAni("start_Phase");
	if (IsValid(StartCountAnim))
	{
		StartCountAnim->OnAnimationFinished.RemoveDynamic(this, &UUP_ReadyToStart::BP_ReadyToStartEvent);
		StartCountAnim->OnAnimationFinished.AddDynamic(this, &UUP_ReadyToStart::BP_ReadyToStartEvent);
	}

	if (IsValid(StartPhaseAnim))
	{
		StartPhaseAnim->OnAnimationFinished.RemoveDynamic(this, &UUP_ReadyToStart::BP_ReadyToStartEvent);
		StartPhaseAnim->OnAnimationFinished.AddDynamic(this, &UUP_ReadyToStart::BP_ReadyToStartEvent);
	}

	RGameInstance = RGAMEINSTANCE(this);
	if (false == RGameInstance.IsValid())
		return;
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (IsValid(BaseStageInfo))
		CheckGameMode = BaseStageInfo->GetGameMode();

	RGameInstance->EventManager->OnUI_ReadyToStart.RemoveDynamic(this, &UUP_ReadyToStart::OnReadyToStartEvent);
	RGameInstance->EventManager->OnUI_ReadyToStart.AddDynamic(this, &UUP_ReadyToStart::OnReadyToStartEvent);

	RGameInstance->EventManager->OnPhaseEvent.RemoveDynamic(this, &UUP_ReadyToStart::OnPhaseEvent);
	RGameInstance->EventManager->OnPhaseEvent.AddDynamic(this, &UUP_ReadyToStart::OnPhaseEvent);
}

void UUP_ReadyToStart::NativeDestruct()
{
	RGameInstance = RGAMEINSTANCE(this);
	if (false == RGameInstance.IsValid())
		return;

	RGameInstance->EventManager->OnUI_ReadyToStart.RemoveDynamic(this, &UUP_ReadyToStart::OnReadyToStartEvent);
	RGameInstance->EventManager->OnPhaseEvent.RemoveDynamic(this, &UUP_ReadyToStart::OnPhaseEvent);

	Super::NativeDestruct();
}

void UUP_ReadyToStart::OnReadyToStartEvent()
{
	PlayAni(TEXT("start_count"));
}

void UUP_ReadyToStart::OnPhaseEvent(FString StringKey)
{
	PhaseText->SetText(FText::FromString(StringKey));
	PlayAni(TEXT("start_Phase"));

}

void UUP_ReadyToStart::BP_ReadyToStartEvent()
{
	RGameInstance->EventManager->OnGameMode_ReadyToStart.Broadcast();
}