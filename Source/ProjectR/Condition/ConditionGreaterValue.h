// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/BaseConditionValue.h"
#include "ConditionGreaterValue.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UConditionGreaterValue : public UBaseConditionValue
{
	GENERATED_BODY()

public:
	bool	Check() override;

};
