// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_SeasonInfo.h"
#include "UI/Guild/GuildRaid/UP_GuildRaidMain.h"
#include "UI/Common/UC_Item_Icons.h"


#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/GuildInfo.h"

#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"




void UUC_GuildRaid_SeasonInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_GuildRaid_SeasonInfo::NativeDestruct()
{
	//StopTimer();

	Super::NativeDestruct();
}

void UUC_GuildRaid_SeasonInfo::Init(TAssetPtr<UUP_GuildRaidMain> InRaidMain)
{
	GuildRaidMain = InRaidMain;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		LastReward			= RGameInstance->GuildInfo->RaidSeasonStatus.LastReward;
		LastLevel			= RGameInstance->GuildInfo->RaidSeasonStatus.LastLevel;
		CurrentReward		= RGameInstance->GuildInfo->RaidSeasonStatus.Reward;
		CurrentMaxPoint		= RGameInstance->GuildInfo->RaidSeasonStatus.MaxPoint;
		CurrentPoint		= RGameInstance->GuildInfo->RaidSeasonStatus.Point;
		CurrentLevel		= RGameInstance->GuildInfo->RaidSeasonStatus.Level;		
		IsAlreadyReward		= RGameInstance->GuildInfo->RaidSeasonStatus.IsAlreadyReward;
		RemainTime			= RGameInstance->GuildInfo->RaidSeasonStatus.RemainTime;
	}

	GuildRaidRewardTableInfo = UTableManager::GetInstancePtr()->GetGuildRaidRewardTableRow(FName(TEXT("1")));
		
	SetProgress();
	SetRewardItemIcon();
	SetRemainFinishTime();
	SetActivateRewardButton();

}

void UUC_GuildRaid_SeasonInfo::Refresh()
{
	SetProgress();
	SetRewardItemIcon();
	SetRemainFinishTime();
	SetActivateRewardButton();
}

void UUC_GuildRaid_SeasonInfo::UpdateRemainTime()
{
	RemainTime -= 1;

	int32 Hour = RemainTime / 3600;
	int32 Min = (RemainTime % 3600) / 60;
	int32 Sec = RemainTime % 60;

	if (IsValid(Text_Hour))
		Text_Hour->SetText(FText::AsNumber(Hour));

	if (IsValid(Text_Minute))
		Text_Minute->SetText(FText::AsNumber(Min));

	if (IsValid(Text_Second))
		Text_Second->SetText(FText::AsNumber(Sec));

// 	if( RemainTime <= 0)
// 		StopTimer();
}

void UUC_GuildRaid_SeasonInfo::OnClick_Reward()
{
//	UPacketFunctionLibrary::GUILD_RAID_REWARD_RQ(this);
}

void UUC_GuildRaid_SeasonInfo::OnClick_ItemIcon(bool bLeft)
{
// 	int32 rewardIndex = GetRewardIndex();
// 	if (bLeft)
// 	{		
// 		if (GuildRaidMain.IsValid())
// 			GuildRaidMain.Get()->ShowSeasonRewardPopup(rewardIndex, false);
// 	}
// 	else
// 	{
// 		rewardIndex += 1;
// 
// 		if (GuildRaidMain.IsValid())
// 			GuildRaidMain.Get()->ShowSeasonRewardPopup(rewardIndex, true);
// 	}
// 	
}


void UUC_GuildRaid_SeasonInfo::SetProgress()
{
	if(GuildRaidRewardTableInfo == nullptr)
		return;

	if (IsValid(Progress_Season))
	{
		float Ratio = (float)CurrentPoint / (float)CurrentMaxPoint;
		Progress_Season->SetPercent(Ratio);
	}	

}

void UUC_GuildRaid_SeasonInfo::SetRewardItemIcon()
{
	if (GuildRaidRewardTableInfo == nullptr)
		return;
	
	if (LastReward.IsNone())
	{
		if (IsValid(CurrentItemIcon))
			CurrentItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{		
		if (IsValid(CurrentItemIcon))
		{
			CurrentItemIcon->SetVisibility(ESlateVisibility::Visible);

			TArray<FITEM_ICON_INFO> RewardsItemIconInfo;
			UUIFunctionLibrary::GetRewardsIconInfoByPreview(LastReward, RewardsItemIconInfo);

			if (RewardsItemIconInfo.Num() > 0)
				CurrentItemIcon->SetItemIconStructure(RewardsItemIconInfo[0]);
		}

	}

	
	if (CurrentReward.IsNone())
	{
		if (IsValid(NextItemIcon))
			NextItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (IsValid(NextItemIcon))
		{
			NextItemIcon->SetVisibility(ESlateVisibility::Visible);

			TArray<FITEM_ICON_INFO> RewardsItemIconInfo;
			UUIFunctionLibrary::GetRewardsIconInfoByPreview(CurrentReward, RewardsItemIconInfo);

			if (RewardsItemIconInfo.Num() > 0)
				NextItemIcon->SetItemIconStructure(RewardsItemIconInfo[0]);
		}

	}
}

void UUC_GuildRaid_SeasonInfo::SetRemainFinishTime()
{
	int32 Hour = RemainTime / 3600;
	int32 Min = (RemainTime % 3600) / 60;
	int32 Sec = RemainTime % 60;
		
	if (IsValid(Text_Hour))
		Text_Hour->SetText(FText::AsNumber(Hour));
		
	if (IsValid(Text_Minute))
		Text_Minute->SetText(FText::AsNumber(Min));
		
	if (IsValid(Text_Second))
		Text_Second->SetText(FText::AsNumber(Sec));
		

	//StartTimer();

}

void UUC_GuildRaid_SeasonInfo::SetActivateRewardButton()
{
	if(  !IsValid( Button_Reward) )
		return;

	if( IsAlreadyReward)
 		Button_Reward->SetIsEnabled(false);
	else
		Button_Reward->SetIsEnabled(true);

}

int32 UUC_GuildRaid_SeasonInfo::GetRewardIndex()
{
	
	for (int32 reqIndex = GuildRaidRewardTableInfo->requiredGuildRaidPoint.Num() - 1; reqIndex >= 0; --reqIndex)
	{
		if (CurrentPoint >= GuildRaidRewardTableInfo->requiredGuildRaidPoint[reqIndex])
		{
			return reqIndex;
		}
	}
	
	return -1;
}

void UUC_GuildRaid_SeasonInfo::StopTimer()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		RGameInstance->TimerManager->ClearTimer(TimerHandle_RemainTime);
	}

}

void UUC_GuildRaid_SeasonInfo::StartTimer()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		RGameInstance->TimerManager->SetTimer(TimerHandle_RemainTime, this, &UUC_GuildRaid_SeasonInfo::UpdateRemainTime, 1.0f, true);
	}
}
