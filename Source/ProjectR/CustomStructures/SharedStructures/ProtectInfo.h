// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ProtectInfo.generated.h"

class UBuffProtect;
/**
 * 
 */

USTRUCT(BlueprintType)
struct PROJECTR_API FProtectInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	EProtectType						ProtectType			= EProtectType::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	EAttackTypeEnum						AttackType			= EAttackTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	ECharacterPropertyTypeEnum			PropertyType = ECharacterPropertyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
		ESkillApplierTypeEnum				SkillApplierType = ESkillApplierTypeEnum::VE_Applier_Max;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	FName								ImmuneGroupRow;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	int32								ProtectPower		= 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	int32								ProtectPowerMax = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	float								ProtectTime			= 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ProtectInfo)
	float								ProtectElapsedTime	= 0.0f;
};