// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Condition/BaseCondition.h"
#include "ConditionBuffEnter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UConditionBuffEnter : public UBaseCondition
{
	GENERATED_BODY()

public:
	bool	Check() override;
	void	InitCondition(AActor* Owner, int32 InCheckCount = 0) override;

	UFUNCTION()
	void  OnBuffEnter(AActor* InCaster, int32 InStateFlag );

	void  AddBuffType(int32 InStateFlag);

protected:
	TSet<int32>	CheckBuffMap;
	bool	bEnterBuff = false;
};
