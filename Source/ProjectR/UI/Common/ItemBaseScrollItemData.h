// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "ItemBaseScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UItemBaseScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	void SetData(const FITEM_ICON_INFO& InIconInfo);

	FORCEINLINE	FITEM_ICON_INFO& GetItemIconData() { return ItemIconInfo; }


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ItemInvenScrollItemData)
	FITEM_ICON_INFO			ItemIconInfo;
};
