// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "LoadingTableInfo.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FLoadingTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LoadingTableInfo)
	EGamePlayModeTypeEnum LoadingContentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LoadingTableInfo)
	FName	TextureID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LoadingTableInfo)
	TArray<FName> LoadingTexts;
};