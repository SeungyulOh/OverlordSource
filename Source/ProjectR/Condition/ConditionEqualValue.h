// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/BaseConditionValue.h"
#include "ConditionEqualValue.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UConditionEqualValue : public UBaseConditionValue
{
	GENERATED_BODY()

public:
	bool	Check() override;

};
