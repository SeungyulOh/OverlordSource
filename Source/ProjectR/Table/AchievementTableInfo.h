// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "AchievementTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FAchievementTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	EACHIEVE_TYPE type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	FName NameString_Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = AchievementTableInfo)
	FName desc_key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	int32 passValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	FName rewardKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	FName type_pt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	EMissionCategoryType	MissionCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementTableInfo)
	EUIMenuIDEnum			Shortcut;
};