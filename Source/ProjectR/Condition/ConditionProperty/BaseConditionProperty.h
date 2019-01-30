// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "BaseConditionProperty.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseConditionProperty : public UObject
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = UBaseConditionProperty)
	void	InitProperty(float InValue);
	
	virtual float	GetPropertyValue() { return FValue; }

protected:
	float			FValue = 0.0f;
};
