// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillUnlockLevelTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSkillUnlockLevelTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillUnlockLevelTable)
	int32	HeroLevel = 0;
};
