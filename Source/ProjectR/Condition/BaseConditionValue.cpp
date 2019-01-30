// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"

#include "BaseConditionValue.h"

#include "Condition/ConditionProperty/BaseConditionProperty.h"
#include "Condition/ConditionProperty/CharacterConditionProperty.h"
#include "Condition/ConditionProperty/CharacterHpPercentConditionProperty.h"


void UBaseConditionValue::SourcePropertyValue(float InValue)
{
	SourceProperty = CreatePropertyValue(InValue);
}

void UBaseConditionValue::SourcePropertyCharacterHpPercentValue(AActor* InOwner)
{
	SourceProperty = CreatePropertyCharacterHpPercentValue(InOwner);
}

void UBaseConditionValue::SourcePropertyCharacterValue(AActor* InOwner, ECharacterPropertyTypeEnum InPropertyType)
{
	SourceProperty = CreatePropertyCharacterValue(InOwner, InPropertyType);
}

void UBaseConditionValue::DestPropertyValue(float InValue)
{
	DestProperty = CreatePropertyValue(InValue);
}

void UBaseConditionValue::DestPropertyCharacterHpPercentValue(AActor* InOwner)
{
	DestProperty = CreatePropertyCharacterHpPercentValue(InOwner);
}

void UBaseConditionValue::DestPropertyCharacterValue(AActor* InOwner, ECharacterPropertyTypeEnum InPropertyType)
{
	DestProperty = CreatePropertyCharacterValue(InOwner, InPropertyType);
}

UBaseConditionProperty* UBaseConditionValue::CreatePropertyValue(float InValue)
{
	UBaseConditionProperty* ReturnObject = NewObject< UBaseConditionProperty >((UObject*)GetTransientPackage(), UBaseConditionProperty::StaticClass());
	if (IsValid(ReturnObject))
	{
		ReturnObject->InitProperty(InValue);
	}

	return ReturnObject;
}

UBaseConditionProperty*	UBaseConditionValue::CreatePropertyCharacterValue(AActor* InOwner, ECharacterPropertyTypeEnum InPropertyType)
{
	UCharacterConditionProperty* ReturnObject = NewObject< UCharacterConditionProperty >((UObject*)GetTransientPackage(), UCharacterConditionProperty::StaticClass());
	if (IsValid(ReturnObject))
	{
		ReturnObject->InitConditionProperty(InOwner, InPropertyType);
	}

	return ReturnObject;
}

UBaseConditionProperty* UBaseConditionValue::CreatePropertyCharacterHpPercentValue(AActor* InOwner)
{
	UCharacterHpPercentConditionProperty* ReturnObject = NewObject< UCharacterHpPercentConditionProperty >((UObject*)GetTransientPackage(), UCharacterHpPercentConditionProperty::StaticClass());
	if (IsValid(ReturnObject))
	{
		ReturnObject->InitConditionProperty(InOwner);
	}

	return ReturnObject;
}