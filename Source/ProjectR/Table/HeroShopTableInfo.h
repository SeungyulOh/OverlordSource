// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataEnum.h"
#include "HeroShopTableInfo.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FHeroShopProductTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	FName		rewardId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	FName		productName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	EREWARD_TYPE		currencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	int32		basePrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	int32		priceExtraCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	int32		isDiscount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	int32		primeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	int32		randomRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	FName		panelId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	int32		limitEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	TAssetPtr< UTexture2D >			ItemSpriteRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	EItemGradeEnum		Grade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	FName		HeroKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	FName		productDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	FName		Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopProductTableInfo)
	FName		DisplayItemID;

};


USTRUCT(BlueprintType)
struct PROJECTR_API FHeroShopSynergyTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopSynergyTableInfo)
	FName		SynergyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopSynergyTableInfo)
	int32		randomRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopSynergyTableInfo)
	FName		panelId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopSynergyTableInfo)
	FName		groupMember1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopSynergyTableInfo)
	FName		groupMember2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroShopSynergyTableInfo)
	FName		groupMember3;

};
