// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"

#include "ConditionEqualValue.h"
#include "Condition/ConditionProperty/BaseConditionProperty.h"


bool UConditionEqualValue::Check()
{
	Super::Check();

	if (IsValid(SourceProperty) && IsValid(DestProperty))
	{
		return SourceProperty->GetPropertyValue() == DestProperty->GetPropertyValue();
	}

	return false;
}