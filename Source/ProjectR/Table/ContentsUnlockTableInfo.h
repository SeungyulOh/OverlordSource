// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ContentsUnlockTableInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct PROJECTR_API FContentsUnlockTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	EGamePlayModeTypeEnum				ContentType = EGamePlayModeTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	int32						SubKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	FName						Trigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	int32						Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	FName						StrKey1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	FName						StrKey2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	FName						StrKey3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnlockTableInfo)
	FName						StrKey4;

	
	
};