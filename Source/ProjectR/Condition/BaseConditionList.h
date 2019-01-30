// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/BaseCondition.h"
#include "BaseConditionList.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseConditionList : public UBaseCondition
{
	GENERATED_BODY()

public:
	void AddCondition(UBaseCondition* InCondition);

	void Update(float deltaTime) override;

protected:
	UPROPERTY()
	TArray<UBaseCondition*>				ConditionList;
};
