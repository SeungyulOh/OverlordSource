// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_RewardsSplash.h"

#include "Table/CurrencyPropertyTableInfo.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "UI/Common/UC_ItemIcon.h"

void UUC_RewardsSplash::RefreshRewards(const TArray<FREWARD>& Rewards, EItemInventoryRewardOrigin RewardOrigin)
{
	ensure(Rewards.Num() <= ItemIcons.Num());

	for (UUC_GenericItemIcon* ItemIcon : ItemIcons)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 i = 0; i < Rewards.Num(); i++)
	{
		if (i >= ItemIcons.Num())
			break;

		const FREWARD& Reward = Rewards[i];
		UUC_GenericItemIcon* ItemIcon = ItemIcons[i];

		ItemIcon->SetVisibility(ESlateVisibility::Visible);

		switch (Reward.type)
		{
		case EREWARD_TYPE::ITEM:
			ItemIcon->RefreshEquipmentIcon(Reward.rewardId.ToString());
			break;
		case EREWARD_TYPE::WALLET:
		{
			FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(Reward.rewardId);
			// WARNING: passing rewarded amount as OwnedAmount...
			ItemIcon->RefreshWalletIcon(Reward.rewardId, WalletInfo, Reward.amt);
		}
		break;
		default:
			UE_LOG(LogUI, Log, TEXT("UUC_RewardsSplash::RefreshRewards: unhandled REWARD.type"));
			break;
		}
	}

	// description text
	if (RewardOrigin == EItemInventoryRewardOrigin::Nonspecific)
	{
		Switcher_Description->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Switcher_Description->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		Switcher_Description->SetActiveWidgetIndex((int32)RewardOrigin);
	}
}