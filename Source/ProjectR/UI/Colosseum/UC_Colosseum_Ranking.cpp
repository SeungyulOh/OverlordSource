// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Colosseum_Ranking.h"



void URColosseum_Ranker_Item::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Colosseum_Ranking::NativeDestruct()
{
	Super::NativeDestruct();
}

void URColosseum_Ranker_ItemData::SetSwitcherArenRankerDivision(UWidgetSwitcher* division_mark)
{
	if (IsValid(division_mark))
	{
		division_mark->SetActiveWidgetIndex(OrderIndex % 2);
	}
}

void URColosseum_Ranker_ItemData::SetSwitcherArenRankerMark(UWidgetSwitcher* rank_mark, UWidgetSwitcher* WidgetSwitcher_Rank, UTextBlock* Text_Rank)
{
 	if (rank_mark)
 	{
		if (10 >= RankerData.rank)
		{
			WidgetSwitcher_Rank->SetActiveWidgetIndex(0);
			rank_mark->SetActiveWidgetIndex(RankerData.rank - 1);
		}
		else
		{
			WidgetSwitcher_Rank->SetActiveWidgetIndex(1);
			rank_mark->SetActiveWidgetIndex(0);
			Text_Rank->SetText(FText::AsNumber(RankerData.rank));
		}
	}
}

void URColosseum_Ranker_ItemData::SetTextArenRankerInfo(UTextBlock* level, UTextBlock* nick, UTextBlock* guild)
{
	if (IsValid(level) && IsValid(nick) && IsValid(guild))
	{
		FText info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(RankerData.level));
		level->SetText(info_text);

		info_text = FText::FromString(RankerData.nick);
		nick->SetText(info_text);

		info_text = FText::FromString(RankerData.guild);
		guild->SetText(info_text);
	}
}

void URColosseum_Ranker_ItemData::SetTextArenRankPowerPoint(UTextBlock* point)
{
	if (IsValid(point))
	{
		FText info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(RankerData.point));
		point->SetText(info_text);
	}
}

void URColosseum_Ranker_Item::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	URColosseum_Ranker_ItemData* RankingItemData = Cast<URColosseum_Ranker_ItemData>(item);

	if (IsValid(RankingItemData))
	{
		RankingItemData->SetSwitcherArenRankerDivision(Ranking_WidgetSwitcher);
		RankingItemData->SetTextArenRankerInfo(Text_AccountLevel, Text_AccountName, Text_GuildName);
		UWidgetSwitcher* WidgetSwitcher = Cast<UWidgetSwitcher>(UC_Arena_Tier->GetWidgetFromName(TEXT("WidgetSwitcher_0")));
		if (IsValid(WidgetSwitcher))
			RankingItemData->SetSwitcherArenRankerMark(WidgetSwitcher, WidgetSwitcher_Ranking, Text_Ranking);

		RankingItemData->SetTextArenRankPowerPoint(Text_ArenaPoint);
	}
}

void UUC_Colosseum_Ranking::NativeConstruct()
{
	Super::NativeConstruct();
	ColosseumRank.Reset();

	if (IsValid(BP_ScrollView) == false)
		return;

	if (!BP_ScrollView->ScrollItemList.Num())
		BP_ScrollView->Init_NoClear(MAXSCROLLITEM);

	if (!CachedItemDataList.Num())
	{
		for (size_t i = 0; i < MAXSCROLLITEMDATA; ++i)
		{
			URColosseum_Ranker_ItemData* ItemData = Cast<URColosseum_Ranker_ItemData>(BP_ScrollView->CreateScrollItemData(URColosseum_Ranker_ItemData::StaticClass()));
			ItemData->WidgetSize = BP_ScrollView->GetItemInitSize();
			CachedItemDataList.Emplace(ItemData);
		}
	}
}


void UUC_Colosseum_Ranking::SetRank(TArray<FCOLOSSEUM_RANK_USER>& InArenaRank)
{
	ColosseumRank = InArenaRank;

	BP_ScrollView->ClearScrollItemData();

	URColosseum_Ranker_ItemData*		reward_data = nullptr;
	int32 RankCount = ColosseumRank.Num();
	for (int32 n = 0; n < RankCount; ++n)
	{
		if (n >= MAXSCROLLITEMDATA)
			break;

		if (CachedItemDataList.IsValidIndex(n))
		{
			reward_data = CachedItemDataList[n];
			reward_data->OrderIndex = n;
			reward_data->RankerData = ColosseumRank[n];

			BP_ScrollView->ItemDataList.Emplace(reward_data);
		}
	}
	BP_ScrollView->InvalidateData();
}
