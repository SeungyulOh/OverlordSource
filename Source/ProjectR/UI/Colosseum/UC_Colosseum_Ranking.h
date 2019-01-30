// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"

#include "UC_Colosseum_Ranking.generated.h"

UCLASS()
class PROJECTR_API URColosseum_Ranker_ItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32	OrderIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = URColosseum_Ranker_ItemData)
	FCOLOSSEUM_RANK_USER RankerData;

	UFUNCTION(BlueprintCallable, Category = URColosseum_Ranker_ItemData)
	void SetSwitcherArenRankerDivision(UWidgetSwitcher* division_mark);

	UFUNCTION(BlueprintCallable, Category = URColosseum_Ranker_ItemData)
	void SetSwitcherArenRankerMark(UWidgetSwitcher* rank_mark, UWidgetSwitcher* WidgetSwitcher_Rank, UTextBlock* Text_Rank);

	UFUNCTION(BlueprintCallable, Category = URColosseum_Ranker_ItemData)
	void SetTextArenRankerInfo(UTextBlock* level, UTextBlock* nick, UTextBlock* guild);

	UFUNCTION(BlueprintCallable, Category = URColosseum_Ranker_ItemData)
	void SetTextArenRankPowerPoint(UTextBlock* point);
};

UCLASS()
class PROJECTR_API URColosseum_Ranker_Item : public URScrollItem
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	
private:
	UPROPERTY()
	UWidgetSwitcher*	Ranking_WidgetSwitcher = nullptr;

	UPROPERTY()
	UTextBlock*			Text_AccountLevel = nullptr;

	UPROPERTY()
	UTextBlock*			Text_AccountName = nullptr;

	UPROPERTY()
	UTextBlock*			Text_GuildName = nullptr;

	UPROPERTY()
	UTextBlock*			Text_ArenaPoint = nullptr;

	UPROPERTY()
	UUserWidget*		UC_Arena_Tier = nullptr;

	UPROPERTY()
	UWidgetSwitcher*	WidgetSwitcher_Ranking = nullptr;

	UPROPERTY()
	UTextBlock*			Text_Ranking = nullptr;
};

UCLASS()
class PROJECTR_API UUC_Colosseum_Ranking : public URBaseWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	URScrollView* BP_ScrollView = nullptr;

	TArray<FCOLOSSEUM_RANK_USER> ColosseumRank;

	void		NativeConstruct() override;
	void		NativeDestruct() override;
	void		SetRank(TArray<FCOLOSSEUM_RANK_USER>& InArenaRank);

	UPROPERTY()
	TArray<URColosseum_Ranker_ItemData*> CachedItemDataList;

	const int32 MAXSCROLLITEM = 10;
	const int32 MAXSCROLLITEMDATA = 30;
};