// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollItem.h"
#include "UC_FloorShopScrollListItem.generated.h"

class UUC_FloorShop_ScrollItem;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_FloorShopScrollListItem : public URScrollItem
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void InvalidateData()	override;
	
	//property
	UPROPERTY()
		UTextBlock* Text_FloorNum;
	UPROPERTY()
		UTextBlock* Text_LockDesc;
	UPROPERTY()
		TArray<UUC_FloorShop_ScrollItem* > itemList;
};

UCLASS()
class PROJECTR_API UUC_FloorShopScrollListItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	TArray<FSHOP_FLOORSHOP_SLOT> dataList;
	uint8 floorNum;
};
