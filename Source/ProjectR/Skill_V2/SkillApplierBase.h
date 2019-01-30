// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "SkillApplierBase.generated.h"




/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSkillApplierBase
{
	GENERATED_USTRUCT_BODY()

public:

	void operator += ( const FSkillApplierBase& Target)
	{
		TargetPercent += Target.TargetPercent;
		TargetValue += Target.TargetValue;
	}

	void operator -= (const FSkillApplierBase& Target)
	{
		TargetPercent -= Target.TargetPercent;
		TargetValue -= Target.TargetValue;
	}

	bool operator == (const FSkillApplierBase& Target)
	{
		if (TargetPercent == Target.TargetPercent &&
			TargetValue == Target.TargetValue &&
			SkillApplierType == Target.SkillApplierType)
			return true;

		return false;
	}
	
	FORCEINLINE float GetPercent() { return TargetPercent; }
	FORCEINLINE int32 GetIntPercent() { return (int32)TargetPercent; }
	FORCEINLINE int32 GetValue() { return TargetValue; }
	FORCEINLINE float GetFloatValue() { return (float)TargetValue; }

	ESkillApplierTypeEnum				SkillApplierType = ESkillApplierTypeEnum::VE_None;

	float								TargetPercent	= 0.0f;
	int32								TargetValue		= 0;
	
	int32								CasterMagicPower = 0.0f;
};