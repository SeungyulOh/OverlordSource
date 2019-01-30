// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "CharacterBattleData.generated.h"


/**
*
*/
USTRUCT()
struct FCharacterBattleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	ECharacterPropertyTypeEnum	AttackPropertyType = ECharacterPropertyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	EAttackTypeEnum				AttackType = EAttackTypeEnum::VE_Melee;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		EPriorityTypeEnum			AIPriorityType = EPriorityTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		MaxHP = 0;

	// TODO: Delete
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		PhysicalAttackPower = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		MagicAttackPower = 0;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		int32		MagicCriticalRate = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		float		SightRange = 0.0f;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		float		SightRange_PD = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		float		AttackRange = 0.0f;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		float		FlyAttackRange = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		float		MoveSpeed = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		float		AttackSpeed = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		UnitRadius = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		float		RangeThreshold = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		Armor = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		MagicResistance = 0;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		int32		ParryValue = 0;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		int32		ParryDamageReduceRate_Scale = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		DodgeValue = 0;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		int32		MPRegen = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		CriticalValue = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		CriticalDamageRate_Scale = 0;

 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
 		int32		CriticalResistValue = 0;
 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		int32		CriticalResistDamage = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		PierceValue = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		int32		AccuracyValue = 0;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		int32		LifeStealValue = 0;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
// 		int32		SpawnCount = 0;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
		float		BulletSpeed = 0.0f;

	float		AttackHitperSecond = 0;

};
