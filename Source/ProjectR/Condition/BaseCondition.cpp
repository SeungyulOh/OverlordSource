// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"


#include "BaseCondition.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"


// Condition ID must be set before this function
void UBaseCondition::InitCondition(AActor* Owner, int32 InCheckCount)
{
	OwnerCharacter = Owner;

	CheckCount = InCheckCount;
	CurentCheckCount = 0;

	CheckStart = false;
}

bool UBaseCondition::IsCheck()
{
	if (CheckCount <= 0)
	{
		return Check();
	}
	else
	{
		bool bCheck = Check();

		if (bCheck)
		{
			++CurentCheckCount;

			if (CheckCount == CurentCheckCount)
			{
				return true;
			}
		}
	}

	return false;
}

bool UBaseCondition::Check()
{
	CheckStart = true;
	return false;
}

void UBaseCondition::Update(float deltaTime)
{
}
