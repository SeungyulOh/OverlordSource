// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_ProgressAchievement.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "Network/RTSManager.h"

void UUC_HUD_ProgressAchievement::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, Switcher_Progress, TEXT("Switcher_Progress"));
	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, Switcher_BossKill, TEXT("Switcher_BossKill"));

	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
		return;
//
//	OnModRoomGuildRaidProgress();
//
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnModRoomGuildRaidProgress.RemoveDynamic(this, &UUC_HUD_ProgressAchievement::OnModRoomGuildRaidProgress);
// 		RGameInstance->EventManager->OnModRoomGuildRaidProgress.AddDynamic(this, &UUC_HUD_ProgressAchievement::OnModRoomGuildRaidProgress);
// 	}
}

void UUC_HUD_ProgressAchievement::NativeDestruct()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnModRoomGuildRaidProgress.RemoveDynamic(this, &UUC_HUD_ProgressAchievement::OnModRoomGuildRaidProgress);
// 	}

	Super::NativeDestruct();
}

// void UUC_HUD_ProgressAchievement::OnModRoomGuildRaidProgress()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	TArray<int32>& GuildRaidProgress = RGameInstance->RealTimeModManager->DeadBossProgress;
// 	int32 CheckIndex = 0;
// 	for (auto Progress : Switcher_Progress)
// 	{
// 		if (IsValid(Progress))
// 		{
// 			if (GuildRaidProgress.IsValidIndex(CheckIndex))
// 			{
// 				Progress->SetActiveWidgetIndex(1);
// 			}
// 			else
// 			{
// 				Progress->SetActiveWidgetIndex(0);
// 			}
// 		}
// 
// 		++CheckIndex;
// 	}
// 
// 	CheckIndex = 0;
// 	for (auto BossKill : Switcher_BossKill)
// 	{
// 		if (IsValid(BossKill))
// 		{
// 			if (GuildRaidProgress.IsValidIndex(CheckIndex))
// 			{
// 				BossKill->SetActiveWidgetIndex(1);
// 			}
// 			else
// 			{
// 				BossKill->SetActiveWidgetIndex(0);
// 			}
// 		}
// 
// 		++CheckIndex;
// 	}
// }