// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ConditionNot.h"

#include "Condition/BaseCondition.h"

bool UConditionNot::Check()
{
	Super::Check();

	if (IsValid(SourceCondition))
	{
		return !SourceCondition->Check();
	}

	return false;
}

void UConditionNot::SetCondition(UBaseCondition* InCondition)
{
	SourceCondition = InCondition;
}

void UConditionNot::Update(float deltaTime)
{
	if (IsValid(SourceCondition))
		SourceCondition->Update(deltaTime);
}