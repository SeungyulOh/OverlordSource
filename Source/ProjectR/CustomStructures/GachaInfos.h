// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Engine/DataTable.h"
#include "GachaInfos.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct PROJECTR_API FGachaInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	FName Description;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	FName SummonType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	FName Cost_Currency;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	int32 Cost_Amount;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	int32 AddMileage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	FName RewardId_Basic;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	int32 RewardId_BasicNum;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	FName RewardId_Fix;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	FName String_Key01;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGachaInfo)
	FName String_Key02;

public:
	
	static FGachaInfo* Lookup(FName InSummontype);
};

