// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RaidBossAttackTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FRaidBossAttackTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack01_Terms;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack01_Notice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack02_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack02_Notice;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack03_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack03_Notice;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack04_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack04_Notice;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack05;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack05_Terms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidBossAttackTableInfo)
	FName Attack05_Notice;


};