// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Arena_ranking_list.h"

#include "UI/Arena/RArenaRanking_ScrollItemData.h"

const int32 MAXSCROLLITEM = 10;
const int32 MAXSCROLLITEMDATA = 30;

void UUC_Arena_ranking_list::NativeConstruct()
{
	Super::NativeConstruct();
	ArenaRank.Reset();


	if (!BP_ScrollView->ItemDataList.Num())
		BP_ScrollView->Init_NoClear(MAXSCROLLITEM);

	if (!CachedItemDataList.Num())
	{
		for (size_t i = 0; i < MAXSCROLLITEMDATA; ++i)
		{
			URArenaRanking_ScrollItemData* ItemData = Cast<URArenaRanking_ScrollItemData>(BP_ScrollView->CreateScrollItemData(URArenaRanking_ScrollItemData::StaticClass()));
			ItemData->WidgetSize = BP_ScrollView->GetItemInitSize();
			CachedItemDataList.Emplace(ItemData);
		}
	}
}

void UUC_Arena_ranking_list::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Arena_ranking_list::SetRank(TArray<FARENA_RANK_USER>& InArenaRank)
{
	ArenaRank = InArenaRank;

	if (IsValid(BP_ScrollView) == false)
		return;

	BP_ScrollView->ClearScrollItemData();

	int32 RankCount = ArenaRank.Num();
	for (int32 n = 0; n < RankCount; ++n)
	{
		if (CachedItemDataList.IsValidIndex(n))
		{
			URArenaRanking_ScrollItemData*		ArenaRankData = Cast<URArenaRanking_ScrollItemData>(CachedItemDataList[n]);

			ArenaRankData->OrderIndex = n;
			ArenaRankData->RankerData = ArenaRank[n];

			BP_ScrollView->ItemDataList.Emplace(ArenaRankData);
		}
	}

	BP_ScrollView->InvalidateData();
}
