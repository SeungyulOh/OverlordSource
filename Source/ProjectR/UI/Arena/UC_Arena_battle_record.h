// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UI/UIDefs.h"
#include "UC_Arena_battle_record.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Arena_Battle_Record_Data : public URScrollItemData
{
	GENERATED_BODY()
public:
	bool Defence;
	bool Result;
	int32 Point;
	int32 Level;
	FText Name;
	FText Guild;
	int32 Power;
	TArray<FName> HeroIDs;
	void SetHistory(bool InDefence, bool InResult, int32 InPoint, int32 InLevel, FText InName, FText InGuild, int32 InPower, TArray<FName> InHeroIDs);
};

UCLASS()
class PROJECTR_API UUC_Arena_Battle_Record_Item : public URScrollItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_9 = nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_AResult = nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_APointImage = nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_DResult = nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_DPointImage = nullptr;



	UPROPERTY()
	UTextBlock*					TextBlock_APoint = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_ALevel = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_AName = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_AGuild = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_APower = nullptr;
	UPROPERTY()
	TArray<class URenewal_BaseHeroIcon*> AttackHeros;

	UPROPERTY()
	UTextBlock*					TextBlock_DPoint = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_DLevel = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_DName = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_DGuild = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_DPower = nullptr;
	UPROPERTY()
	TArray<class URenewal_BaseHeroIcon*> DefenceHeros;
	virtual void	InvalidateData()	override;
	void SetHistory(bool InDefence, bool Result, int32 Point, int32 Level, FText Name, FText Guild, int32 Power, TArray<FName> HeroIds);
};