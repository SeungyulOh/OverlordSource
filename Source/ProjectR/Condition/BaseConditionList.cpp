// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"

#include "BaseConditionList.h"

void UBaseConditionList::AddCondition(UBaseCondition* InCondition)
{
	if (IsValid(InCondition))
	{
		ConditionList.Emplace(InCondition);
	}
}

void UBaseConditionList::Update(float deltaTime)
{
	for (UBaseCondition* condition : ConditionList)
	{
		if (IsValid(condition))
			condition->Update(deltaTime);
	}
}