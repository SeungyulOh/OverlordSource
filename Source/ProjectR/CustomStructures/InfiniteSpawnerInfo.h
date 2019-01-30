// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "InfiniteSpawnerInfo.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FInfiniteSpawnerInfo
{
	GENERATED_USTRUCT_BODY()
	
	 
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpawnerInfo)
	FName		CharacterID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpawnerInfo)
	float		SpawnDelay;
};
