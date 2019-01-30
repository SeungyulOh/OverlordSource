// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Condition/BaseCondition.h"
#include "BaseConditionValue.generated.h"

class UBaseConditionProperty;

/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseConditionValue : public UBaseCondition
{
	GENERATED_BODY()

public:
	void		SourcePropertyValue(float InValue);
	void		SourcePropertyCharacterHpPercentValue(AActor* InOwner);
	void		SourcePropertyCharacterValue(AActor* InOwner, ECharacterPropertyTypeEnum InPropertyType);

	void		DestPropertyValue(float InValue);
	void		DestPropertyCharacterHpPercentValue(AActor* InOwner);
	void		DestPropertyCharacterValue(AActor* InOwner, ECharacterPropertyTypeEnum InPropertyType);

protected:
	UBaseConditionProperty*				CreatePropertyValue(float InValue);
	UBaseConditionProperty*				CreatePropertyCharacterValue(AActor* InOwner, ECharacterPropertyTypeEnum InPropertyType);
	UBaseConditionProperty*				CreatePropertyCharacterHpPercentValue(AActor* InOwner);

protected:
	UPROPERTY()
	UBaseConditionProperty*				SourceProperty = nullptr;

	UPROPERTY()
	UBaseConditionProperty*				DestProperty = nullptr;
};
