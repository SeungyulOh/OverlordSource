// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "SharedConstants/GlobalConstants.h"
#include "InAppPurchaseTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FInAppPurchaseTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	//FName		productId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	FName		MIX_KEY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	FName		System;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	FName		RewardKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	FName		SystemMailId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	float		Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	FName		Currency;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InAppPurchaseTableInfo)
	//FName		title;

};
