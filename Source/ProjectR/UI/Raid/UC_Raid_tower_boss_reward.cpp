// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Raid_tower_boss_reward.h"


void UUC_TowerBossRewardItem_Data::SetIcon(FName InIconKey, int32 InCount)
{
	IconKey		=	InIconKey;
	Count		=	InCount;
}

void UUC_TowerBossRewardItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_TowerBossRewardItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_TowerBossRewardItem::SetIcon(FName InIconKey, int32 InCount)
{
	if (InIconKey.IsNone() || InIconKey.IsValid() == false)
		return;
	UC_MaterialIcon->Refresh(InIconKey, InCount, false, 0);
}

void UUC_TowerBossRewardItem::InvalidateData()
{
	//Super::InvalidateData();
	UUC_TowerBossRewardItem_Data* RewardItem = Cast<UUC_TowerBossRewardItem_Data>(ItemData);
	if (RewardItem)
	{
		SetIcon(RewardItem->IconKey, RewardItem->Count);
	}
}