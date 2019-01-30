// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "SharedConstants/GlobalConstants.h"
#include "ShopDisplayGroupTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FShopDisplayGroupTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopDisplayGroupTableInfo)
	FName			currencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopDisplayGroupTableInfo)
	TArray< int32 > resetPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopDisplayGroupTableInfo)
	FString			accessType;
};