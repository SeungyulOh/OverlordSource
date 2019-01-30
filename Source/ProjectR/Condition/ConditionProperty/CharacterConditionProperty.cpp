// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"


#include "CharacterConditionProperty.h"

#include "CustomStructures/BattleObjectInfo.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"


// Condition ID must be set before this function
void UCharacterConditionProperty::InitConditionProperty(AActor* Owner, ECharacterPropertyTypeEnum InPropertyType)
{
	OwnerCharacter = Owner;
	PropertyType = InPropertyType;
}

float UCharacterConditionProperty::GetPropertyValue()
{
	if (OwnerCharacter.IsValid())
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
		if (IsValid(BattleEntityComponent) && IsValid(BattleEntityComponent->GetBattleInfo()))
		{
			return BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(PropertyType);
		}
	}

	return 0.0f;
}