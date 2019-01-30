// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"


#include "BaseConditionProperty.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"


void UBaseConditionProperty::InitProperty(float InValue)
{
	FValue = InValue;
}