// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"


#include "CharacterHpPercentConditionProperty.h"

#include "CustomStructures/BattleObjectInfo.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"


// Condition ID must be set before this function
void UCharacterHpPercentConditionProperty::InitConditionProperty(AActor* Owner)
{
	OwnerCharacter = Owner;
}

float UCharacterHpPercentConditionProperty::GetPropertyValue()
{
	if (OwnerCharacter.IsValid())
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
		if (IsValid(BattleEntityComponent))
		{
			return BattleEntityComponent->GetCurrentHPPercent() * 100.0f;
		}
	}

	return 0.0f;
}