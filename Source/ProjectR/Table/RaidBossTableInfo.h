// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "RaidBossTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FRaidBossTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				Phase01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase01_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase01_AttackName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				Phase02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase02_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase02_AttackName;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				Phase03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase03_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase03_AttackName;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				Phase04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase04_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase04_AttackName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				Phase05;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase05_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								Phase05_AttackName;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				SpecialPhase01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								SpecialPhase01_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								SpecialPhase01_AttackName;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				SpecialPhase02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								SpecialPhase02_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								SpecialPhase02_AttackName;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	ERaidBossActionTypeEnum				SpecialPhase03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								SpecialPhase03_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FRaidBossTableInfo)
	FName								SpecialPhase03_AttackName;

};