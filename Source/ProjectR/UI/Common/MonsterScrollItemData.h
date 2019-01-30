// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "MonsterScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UMonsterScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName NPCID;
	
	UPROPERTY()
	int32 MonsterLevel;

	UPROPERTY()
	bool IsBossMonster = false;
	
};
