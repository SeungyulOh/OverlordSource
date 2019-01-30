// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "UC_HUD_ReSpawn.h"

#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "Network/RTSManager.h"

void UUC_HUD_ReSpawn::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAni("Appear");

	if (IsValid(Button_QuickSpawn))
	{
		Button_QuickSpawn->OnClicked.RemoveDynamic(this, &UUC_HUD_ReSpawn::OnClick_QuickSpawn);
		Button_QuickSpawn->OnClicked.AddDynamic(this, &UUC_HUD_ReSpawn::OnClick_QuickSpawn);
	}

	if (IsValid(Button_SpawnZone))
	{
		Button_SpawnZone->OnClicked.RemoveDynamic(this, &UUC_HUD_ReSpawn::OnClick_SpawnZone);
		Button_SpawnZone->OnClicked.AddDynamic(this, &UUC_HUD_ReSpawn::OnClick_SpawnZone);
	}

	ReSetData();
}

void UUC_HUD_ReSpawn::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HUD_ReSpawn::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ActiveReSpawnTimer)
	{
		RemainTime -= InDeltaTime;

		if (RemainTime <= 0.0f)
		{
			RemainTime = 0.0f;
			ActiveReSpawnTimer = false;

			if (IsValid(TextBlock_RemainTime))
			{
				TextBlock_RemainTime->SetText(FText::AsNumber(0));
			}

			if (IsValid(ProgressBar_ReSpawnTime))
			{
				ProgressBar_ReSpawnTime->SetPercent(1.0f);
			}
		}
		else
		{
			if (IsValid(TextBlock_RemainTime))
			{
				TextBlock_RemainTime->SetText(FText::AsNumber(FMath::CeilToInt(RemainTime)));
			}

			if (IsValid(ProgressBar_ReSpawnTime))
			{
				float TimePercent = 1.0f - (RemainTime / MaxTime);
				ProgressBar_ReSpawnTime->SetPercent(TimePercent);
			}
		}
	}
}

void UUC_HUD_ReSpawn::ReSetData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	ActiveReSpawnTimer = false;
	MaxTime = 0.0f;
	RemainTime = 0.0f;

	auto TableInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("BattleGround_ReSpawnTime")));
	if (TableInfo == nullptr)
		return;
	if (false == TableInfo->GetValue(MaxTime))
		return;

	ActiveReSpawnTimer = true;
	RemainTime = MaxTime;

	if (IsValid(TextBlock_RemainTime))
	{
		TextBlock_RemainTime->SetText(FText::AsNumber(FMath::RoundToInt(MaxTime)));
	}

	if (IsValid(ProgressBar_ReSpawnTime))
	{
		ProgressBar_ReSpawnTime->SetPercent(0.0f);
	}

	if (IsValid(TextBlock_KillerName))
	{
		FText KillerName;
		FBattleRecord* BattleRecord = RGameInstance->BattleRecordManager->GetBattleRecord(RGameInstance->RealTimeModManager->MyKillPlayerUK);
		if (BattleRecord)
		{
			if (BattleRecord->NickName.IsEmpty())
			{
				FString RealTimeUserName = RGameInstance->RealTimeModManager->GetPlayerNameByKID(BattleRecord->UserKID);
				if (RealTimeUserName.IsEmpty())
				{
					KillerName = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Npc, BattleRecord->CharacterID);
				}
				else
				{
					KillerName = FText::FromString(RealTimeUserName);
				}
			}
			else
			{
				KillerName = FText::FromString(BattleRecord->NickName);
			}
		}
		TextBlock_KillerName->SetText(KillerName);
	}
}

void UUC_HUD_ReSpawn::OnClick_SpawnZone()
{
	if (ActiveReSpawnTimer)
		return;

	if (false == URTSManager::GetInstancePtr()->IsConnected())
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
		if (IsValid(RGameMode))
		{
			RGameMode->DelayNextStage(EGameModeStateEnum::VE_Ready, 0.5f);
		}

		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_ReSpawn);
	}
}

void UUC_HUD_ReSpawn::OnClick_QuickSpawn()
{
	if (ActiveReSpawnTimer)
		return;

	if (false == URTSManager::GetInstancePtr()->IsConnected())
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
		if (IsValid(RGameMode))
		{
			RGameMode->DelayNextStage(EGameModeStateEnum::VE_Ready, 0.5f);
		}

		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_ReSpawn);
	}
}