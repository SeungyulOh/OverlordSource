// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"


#include "ConditionBuffEnter.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


#include "Skill_V2/SharedSkill/ISkill_V2.h"


bool UConditionBuffEnter::Check()
{
	Super::Check();

	return bEnterBuff;
}

void UConditionBuffEnter::InitCondition(AActor* Owner, int32 InCheckCount)
{
	Super::InitCondition(Owner, InCheckCount);

	bEnterBuff = false;

	if (IsValid(Owner))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
		RGameInstance->EventManager->OnBuffEnter.RemoveDynamic(this, &UConditionBuffEnter::OnBuffEnter);
		RGameInstance->EventManager->OnBuffEnter.AddDynamic(this, &UConditionBuffEnter::OnBuffEnter);
	}
}

void UConditionBuffEnter::OnBuffEnter(AActor* InCaster, int32 InStateFlag )
{
	if (CheckStart)
	{
		if (OwnerCharacter.IsValid())
		{
			if (OwnerCharacter == InCaster)
			{
				if (CheckBuffMap.Contains(InStateFlag))
				{
					if (!bEnterBuff)
					{
						bEnterBuff = true;
					}
				}
			}
		}
	}
}

void UConditionBuffEnter::AddBuffType(int32 InStateFlag)
{
	if (!CheckBuffMap.Contains(InStateFlag))
	{
		CheckBuffMap.Emplace(InStateFlag);
	}
}
