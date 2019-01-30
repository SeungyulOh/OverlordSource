#pragma once

// #include "Object.h"
#include "BattleLogicConstants.generated.h"


USTRUCT()
struct PROJECTR_API FBattleLogicConstants
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	float PhysicalAttackPower = 0.0f;
	UPROPERTY()
	float MagicAttackPower = 0.0f;
	UPROPERTY()
	float Armor = 0.0f;
	UPROPERTY()
	float MagicResistance = 0.0f;
	UPROPERTY()
	float MaxHP = 0.0f;
	
	UPROPERTY()
	float CommonFunctionConstant = 1.03f;
	UPROPERTY()
	float CriticalConstant = 0.7f;
	UPROPERTY()
	float CriticalDamageConstant = 0.7f;
	UPROPERTY()
	float PierceConstant = 0.9f;
	UPROPERTY()
	float ParryConstant = 0.8f;
	UPROPERTY()
	float DefenceRateConstant = 0.9f;
	UPROPERTY()
	float HitConstant = 0.2f;
	UPROPERTY()
	float BackHitRateBonus = 0.0f;
	UPROPERTY()
	float BackCriticalRateBonus = 0.0f;
	UPROPERTY()
	float MagicCriticalDamageC = 0.0f;

	UPROPERTY()
	float CriticalDamageValue = 1000.0f;
};