// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "ColosseumTableInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FColosseumGrade : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FColosseumGrade)
	FText					Grade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FColosseumGrade)
	FName					SeasonReward;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FColosseumRank : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FColosseumGrade)
	FName					RankReward;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FColosseumModes : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					MapTableKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	int32					Stamina;
};
