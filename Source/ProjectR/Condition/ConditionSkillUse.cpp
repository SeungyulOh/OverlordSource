// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"

#include "ConditionSkillUse.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"


#include "Skill_V2/SharedSkill/ISkill_V2.h"


bool UConditionSkillUse::Check()
{
	Super::Check();

	if (SkillUseCount > SkillCheckCount)
		return true;

	return false;
}

void UConditionSkillUse::SetUseSkill(FName InSkillName, int32 InCount)
{
	SkillName = InSkillName;

	SkillUseCount = 0;
	SkillCheckCount = InCount;

	if (OwnerCharacter.IsValid())
	{
		if (!SkillName.IsNone())
		{
			USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
			if (IsValid(SkillEntityComponent))
			{
				SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &UConditionSkillUse::OnSkillExe);
				SkillEntityComponent->OnNotifySkillCasting.AddDynamic(this, &UConditionSkillUse::OnSkillExe);
			}
		}
	}
}

void UConditionSkillUse::OnSkillExe(AActor* InCaster, UISkill_V2* UseSkill)
{
	if (CheckStart)
	{
		if (IsValid(InCaster) && OwnerCharacter == InCaster)
		{
			if (IsValid(UseSkill))
			{
				if (UseSkill->GetSkillID() == SkillName)
				{
					++SkillUseCount;
				}
			}
		}
	}
}