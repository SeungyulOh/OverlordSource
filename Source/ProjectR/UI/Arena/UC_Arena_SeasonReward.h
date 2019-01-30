// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "UC_Arena_SeasonReward.generated.h"

class UUC_Item_Icons;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Arena_SeasonReward : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	bool Initialize() override;
	void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	
	void SetItem(UUC_Item_Icons* _itemicon, FITEM_ICON_INFO& _iteminfo);
	
private:
	UPROPERTY()
	UWidgetSwitcher*	SeasonRewardSwitcher;

	UPROPERTY()
	UTextBlock*			RewardText;

	UPROPERTY()
	UUserWidget*		UC_Arena_Tier;

	UPROPERTY()
	TArray<UUC_Item_Icons*>		IconsArray;
	
};
