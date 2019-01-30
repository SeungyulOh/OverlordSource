// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Common/UC_ItemIcon.h"

#include "UC_Raid_tower_boss_reward.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTR_API UUC_TowerBossRewardItem_Data : public URScrollItemData
{
	GENERATED_BODY()
public:
	FName IconKey;
	int32 Count;
	void SetIcon(FName InIconKey, int32 InCount);
};

UCLASS()
class PROJECTR_API UUC_TowerBossRewardItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetIcon(FName InIconKey, int32 InCount);
	void InvalidateData();

public:
	UPROPERTY()
	UUC_MaterialIcon*	UC_MaterialIcon		=	nullptr;
};