// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroInfoUpgrade_Stat.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"

#include "GameInfo/RInventory.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UtilFunctionIntegrated.h"



void UUC_HeroInfoUpgrade_Stat::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_HeroInfoUpgrade_Stat::NativeDestruct()
{
	Super::NativeDestruct();
}


void UUC_HeroInfoUpgrade_Stat::Refresh(FCharacterTableInfo* InPrevTable, FCharacterTableInfo* InNextTable)
{
	FText LabelString = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UDescriptionFunctionLibrary::HeroPropertyToUIStringKey(CharacterPropertyType));

	int32 PrevValue	=	0;
	int32 NextValue	=	0;

	SetValue(PrevValue, NextValue, InPrevTable, InNextTable);

	if(IsValid(Text_Name))
		Text_Name->SetText(LabelString);

	if(IsValid(Text_PrevValue))
		Text_PrevValue->SetText(FText::AsNumber(PrevValue));

	if (IsValid(Text_NextValue))
		Text_NextValue->SetText(FText::AsNumber(NextValue));

	if (IsValid(Text_AddValue))
		Text_AddValue->SetText(FText::Format(FText::FromString(TEXT("+{0}")), NextValue-PrevValue));
}

void UUC_HeroInfoUpgrade_Stat::SetValue(int32& InPrevValue, int32& IntNextValue, FCharacterTableInfo* InPrevTable, FCharacterTableInfo* InNextTable)
{
	switch (CharacterPropertyType)
	{
		case ECharacterPropertyTypeEnum::VE_MaxLevel:
		{
			InPrevValue = InPrevTable->MaxLevel;
			IntNextValue = InNextTable->MaxLevel;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_AttackPower:
		{
			if (InPrevTable->PhysicalAttackPower != 0 && InNextTable->PhysicalAttackPower != 0)
			{
				InPrevValue = InPrevTable->PhysicalAttackPower;
				IntNextValue = InNextTable->PhysicalAttackPower;
			}
			else if (InPrevTable->MagicAttackPower != 0 && InNextTable->MagicAttackPower != 0)
			{
				InPrevValue = InPrevTable->MagicAttackPower;
				IntNextValue = InNextTable->MagicAttackPower;
			}
			break;
		}
		case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
		{
			InPrevValue = InPrevTable->PhysicalAttackPower;
			IntNextValue = InNextTable->PhysicalAttackPower;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
		{
			InPrevValue		=	InPrevTable->MagicAttackPower;
			IntNextValue	=	InNextTable->MagicAttackPower;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_AccuracyValue:
		{
			InPrevValue		=	InPrevTable->AccuracyValue;
			IntNextValue	=	InNextTable->AccuracyValue;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_PierceValue:
		{
			InPrevValue		=	InPrevTable->PierceValue;
			IntNextValue	=	InNextTable->PierceValue;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_AttackSpeed:
		{
			InPrevValue		=	InPrevTable->AttackSpeed;
			IntNextValue	=	InNextTable->AttackSpeed;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_CriticalValue:
		{
			InPrevValue		=	InPrevTable->CriticalValue;
			IntNextValue	=	InNextTable->CriticalValue;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale:
		{
			InPrevValue		=	InPrevTable->CriticalDamageRate_Scale;
			IntNextValue	=	InNextTable->CriticalDamageRate_Scale;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_AttackRange:
		{
			InPrevValue		=	InPrevTable->AttackRange;
			IntNextValue	=	InNextTable->AttackRange;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_Armor:
		{
			InPrevValue		=	InPrevTable->Armor;
			IntNextValue	=	InNextTable->Armor;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_MagicResistance:
		{
			InPrevValue		=	InPrevTable->MagicResistance;
			IntNextValue	=	InNextTable->MagicResistance;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_DodgeValue:
		{
			InPrevValue		=	InPrevTable->DodgeValue;
			IntNextValue	=	InNextTable->DodgeValue;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_MaxHP:
		{
			InPrevValue		=	InPrevTable->MaxHP;
			IntNextValue	=	InNextTable->MaxHP;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_MoveSpeed:
		{
			InPrevValue		=	InPrevTable->MoveSpeed;
			IntNextValue	=	InNextTable->MoveSpeed;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_SightRange:
		{
			InPrevValue		=	InPrevTable->SightRange;
			IntNextValue	=	InNextTable->SightRange;
			break;
		}
		case ECharacterPropertyTypeEnum::VE_UnitRadius:
		{
			InPrevValue		=	InPrevTable->UnitRadius;
			IntNextValue	=	InNextTable->UnitRadius;
			break;
		}
	}
}
