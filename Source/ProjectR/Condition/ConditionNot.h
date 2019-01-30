// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/BaseCondition.h"
#include "ConditionNot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UConditionNot : public UBaseCondition
{
	GENERATED_BODY()

public:
	bool Check() override;
	void SetCondition(UBaseCondition* InCondition);

	void Update(float deltaTime) override;

protected:
	UPROPERTY()
	UBaseCondition*				SourceCondition = nullptr;
};
