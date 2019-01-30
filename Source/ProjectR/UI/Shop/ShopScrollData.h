// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataEnum.h"
#include "ShopScrollData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UShopScrollData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FITEM_ICON_INFO			ItemIconInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FName					ProductID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	ESHOP_PRODUCT_TYPE		ProductType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText					ProductName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText					ProductDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	int32					MaxPurchaseCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	int32					CurrentPurchaseCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FName					CurrencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	int32					Price;

	// Temp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	int32					DisplayGroup = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	int32					buyType = 0;
};