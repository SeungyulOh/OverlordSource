// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollView.h"
#include "UI/UIDefs.h"
#include "Rewards_Scrollview.generated.h"

/**
 * BP: UC_RewardsScrollView
 * embedded in several widgets
 * * UC_Campaign_ClearPopup
 * * UC_StageInfo
 * * UP_Raid_StageSelect
 */
UCLASS()
class PROJECTR_API URewards_Scrollview : public URScrollView
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
	
	void InvalidateByRewardKey(const FName& InRewardKey);
	void InvalidateByRewardsArray(const TArray<FITEM_ICON_INFO>& InRewardsIconInfoArray);

private:
	void CreateRewardItemData(int32 Count);
};
