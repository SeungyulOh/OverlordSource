// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "KillCounter.h"
#include "ProjectRGameMode.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"

#include "Public/Components/TextBlock.h"


void UKillCounter::NativeConstruct()
{
	Super::NativeConstruct();
	
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
		return;

	RGameInstance->EventManager->OnSetUpSpawner.RemoveDynamic(this, &UKillCounter::Init);
	RGameInstance->EventManager->OnSetUpSpawner.AddDynamic(this, &UKillCounter::Init);
	//Init();
}


void UKillCounter::NativeDestruct()
{
	Super::NativeDestruct();
	
	/*auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
		return;

// 	RGameInstance->EventManager->OnGameMode_DeadBossSpawnTrigger.RemoveDynamic(this, &UKillCounter::SetKillCounter);
// 	RGameInstance->EventManager->OnSetUpSpawner.RemoveDynamic(this, &UKillCounter::Init);
	*/
}

void UKillCounter::Init()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
		return;

// 	RGameInstance->EventManager->OnGameMode_DeadBossSpawnTrigger.RemoveDynamic(this, &UKillCounter::SetKillCounter);
// 	RGameInstance->EventManager->OnGameMode_DeadBossSpawnTrigger.AddDynamic(this, &UKillCounter::SetKillCounter);

	this->SetVisibility(ESlateVisibility::Visible);

	SetKillCounter();
}

void UKillCounter::SetKillCounter()
{
	auto GamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
	if (IsValid(GamePlayMode) == false)
		return;

	//auto StageSpawner = GamePlayMode->GetStageSpawner();
	//if (IsValid(StageSpawner) == false)
	//	return;

	//auto BossConditionCount			= StageSpawner->GetBossSpawnConditionCount();
	//auto CurrentBossCOnditionCount	= StageSpawner->GetCurrentBossSpawnConditionCOunt();

	//if (CurrentCounter)
	//{
	//	CurrentCounter->SetText(FText::AsNumber(CurrentBossCOnditionCount));
	//}

	//if (NeedCounter)
	//{
	//	NeedCounter->SetText(FText::AsNumber(BossConditionCount));
	//}

	//if (BossConditionCount == CurrentBossCOnditionCount)
	//{
	//	this->SetVisibility(ESlateVisibility::Hidden);
	//}
}