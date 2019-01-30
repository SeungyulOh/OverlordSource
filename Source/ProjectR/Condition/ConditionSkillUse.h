// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Condition/BaseCondition.h"
#include "ConditionSkillUse.generated.h"

class UISkill_V2;

/**
 * 
 */
UCLASS()
class PROJECTR_API UConditionSkillUse : public UBaseCondition
{
	GENERATED_BODY()

public:
	bool	Check() override;

	UFUNCTION()
	void  OnSkillExe(AActor* InCaster, UISkill_V2* UseSkill);

	void  SetUseSkill(FName InSkillName, int32 InCount);

protected:
	FName				SkillName;
	int32				SkillCheckCount = 0;
	int32				SkillUseCount = 0;
};
