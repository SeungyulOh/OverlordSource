// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "SkillLevelUpCostTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSkillLevelUpCostTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillLevelUpCost)
	EClassTypeEnum		Class = EClassTypeEnum::VE_Tanker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillLevelUpCost)
	int32			Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillLevelUpCost)
	TArray<int32>	Gold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillLevelUpCost)
	FName			RequireWalletId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillLevelUpCost)
	TArray<int32>	RequireWalletQty;
};
