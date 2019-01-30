// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "SharedConstants/GlobalConstants.h"
#include "ShopProductTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FShopProductTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	FName		productKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	ESHOP_PRODUCT_TYPE		productType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	FName		productName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	int32		amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	int32		buyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	FName		currencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	int32		price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	int32		displayGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	TAssetPtr< UTexture2D >			ItemIconRef = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	FName		productDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	FName		Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	EItemGradeEnum	Grade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	FName			DisplayItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	int32			buyType;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FShopFloorStore : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FShopFloorStore)
	int32					FloorInfoId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FShopFloorStore)
	int32					ProductNum;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FShopFloorStore)
// 	ESHOP_FLOORSTORE_TYPE	StoreType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FShopFloorStore)
	FName					Product_Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FShopFloorStore)
	FName			Product_Currency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FShopFloorStore)
	int32					Product_CurrencyAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FShopFloorStore)
	int32					Product_Probability;
};