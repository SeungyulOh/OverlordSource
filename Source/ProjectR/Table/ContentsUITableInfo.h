// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "PaperSprite.h"
#include "ContentsUITableInfo.generated.h"

/**
 * for UC_ConvenienceButton
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FContentsUITableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FContentsUITableInfo)
	FName UIStringKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FContentsUITableInfo)
	FName		ContentsIconKey;
};
