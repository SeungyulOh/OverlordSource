// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "Global/ArenaManager.h"
#include "RArenaRanking_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URArenaRanking_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32	OrderIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ArenaRaking_ScrollItemData)
	FARENA_RANK_USER RankerData;

	UFUNCTION(BlueprintCallable, Category = URArenaRanking_ScrollItemData)
	void SetSwitcherArenRankerDivision(UWidgetSwitcher* division_mark);

	UFUNCTION(BlueprintCallable, Category = URArenaRanking_ScrollItemData)
	void SetSwitcherArenRankerMark(UWidgetSwitcher* rank_mark, UWidgetSwitcher* WidgetSwitcher_Rank, UTextBlock* Text_Rank);

	UFUNCTION(BlueprintCallable, Category = URArenaRanking_ScrollItemData)
	void SetTextArenRankerInfo(UTextBlock* level, UTextBlock* nick, UTextBlock* guild);

	UFUNCTION(BlueprintCallable, Category = URArenaRanking_ScrollItemData)
	void SetTextArenRankPowerPoint(UTextBlock* power, UTextBlock* point);
};