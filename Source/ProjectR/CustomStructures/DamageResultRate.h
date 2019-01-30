// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "DamageResultRate.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FDamageResultRate
{
	GENERATED_USTRUCT_BODY()

public:
	//Percent test data

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DamageResult)
	float	MissRate = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DamageResult)
	float	ImmuneRate = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DamageResult)
	float	CounterRate = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DamageResult)
	float	DodgeRate = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DamageResult)
	float	ParryRate = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DamageResult)
	float	CriticalRate = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DamageResult)
	float	HitRate = 0.0f;
};
