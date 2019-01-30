// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ApplyBuffInfo.generated.h"

/**
* 
*/

USTRUCT(BlueprintType)
struct PROJECTR_API FApplyBuffInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ApplyBuffInfo)
	FName						ApplyBuff;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ApplyBuffInfo)
	EFriendshipTypeEnum		ApplyTargetRedirect = EFriendshipTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ApplyBuffInfo)
	EApplyBuffConditionEnum		ApplyBuffCondition = EApplyBuffConditionEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ApplyBuffInfo)
	ESkillStateEnum				ApplyBuffSkillState = ESkillStateEnum::VE_None;
};
