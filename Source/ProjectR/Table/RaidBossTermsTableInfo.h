// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "RaidBossTermsTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FRaidBossTermsTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTermsTableInfo)
	ERaidBossTermsTypeEnum				Terms01_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTermsTableInfo)
	FName								Terms01_Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTermsTableInfo)
	ERaidBossTermsTypeEnum				Terms02_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTermsTableInfo)
	FName								Terms02_Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTermsTableInfo)
	ERaidBossTermsTypeEnum				Terms03_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTermsTableInfo)
	FName								Terms03_Value;
		
	


};