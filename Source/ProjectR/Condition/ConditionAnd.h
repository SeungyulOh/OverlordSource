// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/BaseConditionList.h"
#include "ConditionAnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UConditionAnd : public UBaseConditionList
{
	GENERATED_BODY()

public:
	bool	Check() override;

};
