// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "ItemInventory_ScrollData.generated.h"


/*
* ScrollItemData behind UC_ItemInventory_ScrollItem
*/
UCLASS()
class PROJECTR_API UItemInventory_ScrollData : public URScrollItemData
{
	GENERATED_BODY()

public:
	FString ItemUD;

	// NAME_None for no holder hero
	FName HolderHeroID = NAME_None;

	// means unequippable in ItemInventory normal mode
	// ungrindable in bBatchGrindMode
	bool bDisabled = false;

	// For bBatchGrindMode
	// Not using URScrollItemData.bSelected.
	bool IsSelected = false;
};
