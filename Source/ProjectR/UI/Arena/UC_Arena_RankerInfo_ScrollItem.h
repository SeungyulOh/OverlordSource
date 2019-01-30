// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollItem.h"
#include "UC_Arena_RankerInfo_ScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Arena_RankerInfo_ScrollItem : public URScrollItem
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
	UTextBlock*			Text_AttackPower = nullptr;

	UPROPERTY()
	UTextBlock*			Text_ArenaPoint = nullptr;

	UPROPERTY()
	UUserWidget*		UC_Arena_Tier = nullptr;

	UPROPERTY()
	UWidgetSwitcher*	WidgetSwitcher_Rank = nullptr;

	UPROPERTY()
	UTextBlock*			Text_Rank = nullptr;
	
};
