// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/BaseConditionList.h"
#include "ConditionOr.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UConditionOr : public UBaseConditionList
{
	GENERATED_BODY()

public:
	bool	Check() override;

};
