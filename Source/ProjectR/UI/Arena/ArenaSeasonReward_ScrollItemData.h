// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "ArenaSeasonReward_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UArenaSeasonReward_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32	OrderIndex;

	UPROPERTY()
	bool	Top10Reward;

	UPROPERTY()
	FText	RewardKindText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ArenaSeasonReward_ScrollItemData)
	FITEM_ICON_INFO	RewardItemData1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ArenaSeasonReward_ScrollItemData)
	FITEM_ICON_INFO	RewardItemData2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ArenaSeasonReward_ScrollItemData)
	FITEM_ICON_INFO	RewardItemData3;

	UFUNCTION(BlueprintCallable, Category = ArenaSeasonReward_ScrollItemData)
	void SetSwitcherSeasonRewardDivision(UWidgetSwitcher* division_mark);

	UFUNCTION(BlueprintCallable, Category = ArenaSeasonReward_ScrollItemData)
	void SetTextSeasonRewardKind(UTextBlock* reward_kind, UWidgetSwitcher* reward_mark);

	UFUNCTION()
	void SetSeasonRewardKind(int32 index);
};