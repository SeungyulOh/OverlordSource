// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/ConditionAnd.h"
#include "RootCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URootCondition : public UConditionAnd
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URootCondition)
	FName				ConditionId;


};
