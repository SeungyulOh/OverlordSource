// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Condition/ConditionProperty/BaseConditionProperty.h"
#include "CharacterConditionProperty.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UCharacterConditionProperty : public UBaseConditionProperty
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = UCharacterConditionProperty)
	virtual void	InitConditionProperty(AActor* Owner, ECharacterPropertyTypeEnum InPropertyType);

	float			GetPropertyValue() override;

protected:
	UPROPERTY()
	TWeakObjectPtr< AActor >			OwnerCharacter;

	UPROPERTY()
	ECharacterPropertyTypeEnum			PropertyType;
};
