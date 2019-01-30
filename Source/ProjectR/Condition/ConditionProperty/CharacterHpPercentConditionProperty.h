// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/ConditionProperty/BaseConditionProperty.h"
#include "CharacterHpPercentConditionProperty.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UCharacterHpPercentConditionProperty : public UBaseConditionProperty
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = UCharacterHpPercentConditionProperty)
	void			InitConditionProperty(AActor* Owner);

	float			GetPropertyValue() override;

protected:
	UPROPERTY()
	TWeakObjectPtr< AActor >			OwnerCharacter;
};
