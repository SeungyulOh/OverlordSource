// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ConditionOr.h"

bool UConditionOr::Check()
{
	Super::Check();

	for (UBaseCondition* condition : ConditionList)
	{
		if (IsValid(condition))
		{
			if (condition->Check())
			{
				return true;
			}
		}
	}

	return false;
}