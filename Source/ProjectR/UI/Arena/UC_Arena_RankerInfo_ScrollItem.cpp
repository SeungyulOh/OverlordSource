// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Arena_RankerInfo_ScrollItem.h"
#include "UI/Arena/RArenaRanking_ScrollItemData.h"
#include "UI/Arena/UC_Arena_RankerInfo_ScrollItem.h"



void UUC_Arena_RankerInfo_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Arena_RankerInfo_ScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	URArenaRanking_ScrollItemData* RankingItemData = Cast<URArenaRanking_ScrollItemData>(item);

// 	if()
// 		//WidgetSwitcher_Rank

	if (IsValid(RankingItemData))
	{
		RankingItemData->SetSwitcherArenRankerDivision(Ranking_WidgetSwitcher);
		RankingItemData->SetTextArenRankerInfo(Text_AccountLevel, Text_AccountName, Text_GuildName);
		UWidgetSwitcher* WidgetSwitcher = Cast<UWidgetSwitcher>(UC_Arena_Tier->GetWidgetFromName(TEXT("WidgetSwitcher_0")));
		if (IsValid(WidgetSwitcher))
			RankingItemData->SetSwitcherArenRankerMark(WidgetSwitcher, WidgetSwitcher_Rank, Text_Rank);

		RankingItemData->SetTextArenRankPowerPoint(Text_AttackPower, Text_ArenaPoint);
	}
}
