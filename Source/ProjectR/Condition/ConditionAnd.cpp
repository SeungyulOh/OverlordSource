// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ConditionAnd.h"

#include "Condition/BaseCondition.h"

bool UConditionAnd::Check()
{
	Super::Check();

	for (UBaseCondition* condition : ConditionList)
	{
		if (IsValid(condition))
		{
			if (!condition->Check())
			{
				return false;
			}
		}
	}

	return true;
}