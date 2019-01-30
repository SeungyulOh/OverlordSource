// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataEnum.h"
#include "ForceTutorialTableInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FForceTutorialTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FForceTutorialTableInfo)
	TArray<FName>	Details;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FForceTutorialTableInfo)
	FName			LastEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FForceTutorialTableInfo)
	FName			PrevEvent;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FForceTutorialDetailTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FForceTutorialDetailTableInfo)
	FName		DlgString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FForceTutorialDetailTableInfo)
	FString		Xml;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FForceTutorialDetailTableInfo)
	int32		SaveIdx = 0;
};