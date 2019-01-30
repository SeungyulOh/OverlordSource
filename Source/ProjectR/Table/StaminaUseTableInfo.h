// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "StaminaUseTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FStaminaUseTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaminaUseTableInfo)
	int32	StaminaPVP = 0;
};

