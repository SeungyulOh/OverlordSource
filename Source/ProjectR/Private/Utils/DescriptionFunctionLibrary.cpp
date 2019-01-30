// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"


#include "Table/ItemTableInfo.h"
#include "UI/UIDefs.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Global/TableManager.h"
#include "Global/LocalStringTableManager.h"

//#include "DescriptionFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"

void UDescriptionFunctionLibrary::MakeUnitNameArgument(FFormatNamedArguments& InFormatNamedArguments)
{
	auto UnitStringRowNames = ULocalStringTableManager::GetInstancePtr()->GetStringKeyNames(EStringTableType::VE_Unit);
	for (auto& UnitStringRowElem : UnitStringRowNames)
	{
		auto UnitValue = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Unit, UnitStringRowElem);
		InFormatNamedArguments.Emplace(UnitStringRowElem.ToString(), UnitValue);
	}
}

FString UDescriptionFunctionLibrary::GetHeroPropertyString(const FString& PropertyName, UCharacterBattleInfo* InCharacterBattleInfo)
{
#if LOG_UI
	UE_LOG(LogUI, Log, TEXT("ENTER GetHeroPropertyString(%s,)"), *PropertyName);
#endif

	if (PropertyName.Equals("PhysicalAttackPower"))
	{
		int32 PhysicalAttackPower = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower);
		return FString::FromInt(PhysicalAttackPower);
	}
	else if (PropertyName.Equals("MagicAttackPower"))
	{
		int32 MagicAttackPower = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicAttackPower);
		return FString::FromInt(MagicAttackPower);
	}
// 	else if (PropertyName.Equals("MagicCriticalRate"))
// 	{
// 		float PreciseValue = InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicCriticalRate);
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
	else if (PropertyName.Equals("AttackSpeed"))
	{
		float PreciseValue = InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_AttackSpeed);
		if (PreciseValue < 0.0f)
		{
		//Todo cowspirit: For DebugPanel
		//	PreciseValue = 0.0f;
		}
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
	else if (PropertyName.Equals("CriticalValue"))
	{
		int32 CriticalValue = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_CriticalValue);
		return FString::FromInt(CriticalValue);
	}
	else if (PropertyName.Equals("CriticalDamageRate_Scale"))
	{
		float PreciseValue = InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale) / 100.0f;
		
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
// 	else if (PropertyName.Equals("CriticalResistValue"))
// 	{
// 		float PreciseValue = InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_CriticalResistValue);
// 
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
// 	else if (PropertyName.Equals("CriticalResistDamage"))
// 	{
// 		float PreciseValue = InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_CriticalResistDamage);
// 
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
	else if (PropertyName.Equals("AccuracyValue"))
	{
		int32 AccuracyValue = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_AccuracyValue);
		return FString::FromInt(AccuracyValue);
	}
	else if (PropertyName.Equals("PierceValue"))
	{
		int32 PierceValue = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_PierceValue);
		return FString::FromInt(PierceValue);
	}
	else if (PropertyName.Equals("MaxHP"))
	{
		int32 MaxHP = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
		return FString::FromInt(MaxHP);
	}
	else if (PropertyName.Equals("Armor"))
	{
		int32 Armor = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_Armor);
		return FString::FromInt(Armor);
	}
	else if (PropertyName.Equals("MagicResistance"))
	{
		int32 MagicResistance = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicResistance);
		return FString::FromInt(MagicResistance);
	}
// 	else if (PropertyName.Equals("ParryValue"))
// 	{
// 		int32 ParryValue = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_ParryValue);
// 		return FString::FromInt(ParryValue);
// 	}
// 	else if (PropertyName.Equals("ParryDamageReduceRate_Scale"))
// 	{
// 		float PreciseValue = InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_ParryDamageReduceRate_Scale) / 100.0f;
// 		
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
	else if (PropertyName.Equals("DodgeValue"))
	{
		int32 DodgeValue = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_DodgeValue);
		return FString::FromInt(DodgeValue);
	}
	else if (PropertyName.Equals("MoveSpeed"))
	{
		float PreciseValue = InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MoveSpeed);
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
	else if (PropertyName.Equals("AttackRange"))
	{
		int32 AttackRange = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_AttackRange);
		return FString::FromInt(AttackRange);
	}

	UE_LOG(LogUI, Log, TEXT("EXIT GetHeroPropertyString with nothing"));

	return FString();// DescriptionStr;
}

FString UDescriptionFunctionLibrary::GetHeroPropertyPercentString(const FString& PropertyName, UCharacterBattleInfo* InCharacterBattleInfo)
{
	FString DescriptionStr = "";
	if (PropertyName.Equals("CriticalValue"))
	{
		int32 CriticalValue = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_CriticalValue);
		int32 HeroLevel		= InCharacterBattleInfo->GetLevel();
		float PreciseValue = 0.0f;
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
	else if (PropertyName.Equals("AccuracyValue"))
	{
		int32 HeroLevel		= InCharacterBattleInfo->GetLevel();
		int32 AccuracyValue = InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_AccuracyValue);
		float PreciseValue = 0.0f;
		PreciseValue		= PreciseValue * 100.0f;
		
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
	else if (PropertyName.Equals("PierceValue"))
	{
		int32 HeroLevel		= InCharacterBattleInfo->GetLevel();
		int32 PierceValue	= InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_PierceValue);
		float PreciseValue = 0.0f;
		PreciseValue = PreciseValue * 100.0f;
		
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
	else if (PropertyName.Equals("Armor"))
	{
		int32 HeroLevel		= InCharacterBattleInfo->GetLevel();
		int32 ArmorValue	= InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_Armor);
		float PreciseValue = static_cast<float>(ArmorValue) / static_cast<float>((ArmorValue + (HeroLevel * DefenceRateMultiplyConstantWithLevel)));
		PreciseValue = PreciseValue * 100.0f;
		
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
	else if (PropertyName.Equals("MagicResistance"))
	{
		int32 HeroLevel				= InCharacterBattleInfo->GetLevel();
		int32 MagicResistanceValue	= InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicResistance);
		float PreciseValue = static_cast<float>(MagicResistanceValue) / static_cast<float>((MagicResistanceValue + (HeroLevel * DefenceRateMultiplyConstantWithLevel)));
		PreciseValue = PreciseValue * 100.0f;
		
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
// 	else if (PropertyName.Equals("ParryValue"))
// 	{
// 		int32 HeroLevel		= InCharacterBattleInfo->GetLevel();
// 		int32 ParryValue	= InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_ParryValue);
// 		float PreciseValue = 0.0f;
// 		PreciseValue = PreciseValue * 100.0f;
// 		
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
	else if (PropertyName.Equals("DodgeValue"))
	{
		int32 HeroLevel		= InCharacterBattleInfo->GetLevel();
		int32 DodgeValue	= InCharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_DodgeValue);
		float PreciseValue = 0.0f;
		PreciseValue = PreciseValue * 100.0f;
		
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}

	return DescriptionStr;
}

FText UDescriptionFunctionLibrary::HeroPropertyValueToText(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo)
{
	ECharacterPropertyTypeEnum CharacterPropertyType = CharacterProperty;
	if (CharacterPropertyType == ECharacterPropertyTypeEnum::VE_AttackPower)
	{
		if (InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower))
			CharacterPropertyType = ECharacterPropertyTypeEnum::VE_PhysicalAttackPower;
		else if (InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower))
			CharacterPropertyType = ECharacterPropertyTypeEnum::VE_MagicAttackPower;
	}

	float Value = InCharacterBattleInfo->GetCharacterFinalProperty<float>(CharacterPropertyType);
	FString value = TEXT("");
	if (CharacterProperty == ECharacterPropertyTypeEnum::VE_AttackSpeed)
	{
		value = FString::SanitizeFloat(Value);
	}
	else if (CharacterProperty == ECharacterPropertyTypeEnum::VE_AttackRange)
	{
		value = FString::SanitizeFloat(Value * 0.01f) + TEXT("m");
	}
	else
	{
		Value = FMath::RoundToPositiveInfinity(Value);
		value = FString::FromInt((int32)Value);
	}
		

	return FText::FromString(value);

}

FText UDescriptionFunctionLibrary::HeroItemPropertyToText(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo)
{
	FCharacterBattleProperty& CharacterItemProperties = InCharacterBattleInfo->TotalItemCharacterProperty;

	ECharacterPropertyTypeEnum CharacterPropertyType = CharacterProperty;
	if (CharacterPropertyType == ECharacterPropertyTypeEnum::VE_AttackPower)
	{
		if (InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower))
			CharacterPropertyType = ECharacterPropertyTypeEnum::VE_PhysicalAttackPower;
		else if (InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower))
			CharacterPropertyType = ECharacterPropertyTypeEnum::VE_MagicAttackPower;
	}

	if (IsHeroPropertyFloat(CharacterPropertyType))
	{
		float Value = CharacterItemProperties.GetCharacterProperty<float>(CharacterPropertyType);
		if (IsHeroPropertyPerMil(CharacterPropertyType))
		{
			return FText::FromString(FString::Printf(TEXT("+%.1f"), Value / 10.0));
		}
		else
		{
			return FText::FromString(FString::Printf(TEXT("+%.1f"), Value));
		}
		
	}
	else
	{
		int32 Value = CharacterItemProperties.GetCharacterProperty<int32>(CharacterPropertyType);
		if (IsHeroPropertyPerMil(CharacterPropertyType))
		{
			return FText::FromString(FString::Printf(TEXT("+%.1f"), ((float)Value) / 10.0));
		}
		else
		{
			return FText::FromString(FString::Printf(TEXT("+%d"), Value));
		}
	}
}

FText UDescriptionFunctionLibrary::HeroAdditionalPropertyToText(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo)
{
	FCharacterBattleProperty& CharacterItemProperties = InCharacterBattleInfo->AdditionalCharacterProperty;

	ECharacterPropertyTypeEnum CharacterPropertyType = CharacterProperty;
	if (CharacterPropertyType == ECharacterPropertyTypeEnum::VE_AttackPower)
	{
		if (InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower))
			CharacterPropertyType = ECharacterPropertyTypeEnum::VE_PhysicalAttackPower;
		else if (InCharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower))
			CharacterPropertyType = ECharacterPropertyTypeEnum::VE_MagicAttackPower;
	}

	if (IsHeroPropertyFloat(CharacterPropertyType))
	{
		float Value = CharacterItemProperties.GetCharacterProperty<float>(CharacterPropertyType);
		if (IsHeroPropertyPerMil(CharacterPropertyType))
		{
			return FText::FromString(FString::Printf(TEXT("+%.1f"), Value / 10.0));
		}
		else
		{
			return FText::FromString(FString::Printf(TEXT("+%.1f"), Value));
		}

	}
	else
	{
		int32 Value = CharacterItemProperties.GetCharacterProperty<int32>(CharacterPropertyType);
		if (IsHeroPropertyPerMil(CharacterPropertyType))
		{
			return FText::FromString(FString::Printf(TEXT("+%.1f"), ((float)Value) / 10.0));
		}
		else
		{
			return FText::FromString(FString::Printf(TEXT("+%d"), Value));
		}
	}

}

FName UDescriptionFunctionLibrary::HeroPropertyToUIStringKey(ECharacterPropertyTypeEnum CharacterProperty)
{
	switch (CharacterProperty)
	{
	case ECharacterPropertyTypeEnum::VE_AttackPower:
		return TEXT("UI_HeroManagement_HeroInfo_Stat_Popwer");
	case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
		return TEXT("USK_uc_heroProperty_Phyattack");
	case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
		return TEXT("USK_uc_heroProperty_Magicattack");
	case ECharacterPropertyTypeEnum::VE_Armor:
		return TEXT("USK_uc_heroProperty_PhyDefence");
	case ECharacterPropertyTypeEnum::VE_MagicResistance:
		return TEXT("USK_uc_heroProperty_MagicDefence");
	case ECharacterPropertyTypeEnum::VE_MaxHP:
		return TEXT("USK_uc_heroProperty_Health");
	case ECharacterPropertyTypeEnum::VE_DodgeValue:
		return TEXT("USK_uc_heroProperty_Dodge");
	case ECharacterPropertyTypeEnum::VE_CriticalValue:
		return TEXT("USK_uc_heroProperty_Critical");
	case ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale:
		return TEXT("USK_uc_heroProperty_CriticalDamage");
	case ECharacterPropertyTypeEnum::VE_AttackRange:
		return TEXT("USK_uc_heroProperty_AttackRange");
	case ECharacterPropertyTypeEnum::VE_PierceValue:
		return TEXT("USK_uc_heroProperty_Pierce");
	case ECharacterPropertyTypeEnum::VE_AttackSpeed:
		return TEXT("USK_uc_heroProperty_AttackSpeed");
	case ECharacterPropertyTypeEnum::VE_AccuracyValue:
		return TEXT("USK_uc_heroProperty_Hit");
	case ECharacterPropertyTypeEnum::VE_MoveSpeed:
		return TEXT("USK_uc_heroProperty_MoveSpeed");
	case ECharacterPropertyTypeEnum::VE_CriticalResistValue:
		return TEXT("USK_uc_heroProperty_CriticalResist");

	default:
		UE_LOG(LogUI, Log, TEXT("UDescriptionFunctionLibrary::HeroPropertyToStringKey: unexpected CharacterPropertyType"));
		return TEXT("");
	}
}

bool UDescriptionFunctionLibrary::IsHeroPropertyFloat(ECharacterPropertyTypeEnum CharacterProperty)
{
	switch (CharacterProperty)
	{
	case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
	case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
	case ECharacterPropertyTypeEnum::VE_Armor:
	case ECharacterPropertyTypeEnum::VE_MagicResistance:
	case ECharacterPropertyTypeEnum::VE_MaxHP:
	case ECharacterPropertyTypeEnum::VE_HP:
	case ECharacterPropertyTypeEnum::VE_DodgeValue:
	case ECharacterPropertyTypeEnum::VE_CriticalValue:
	case ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale:
	case ECharacterPropertyTypeEnum::VE_PierceValue:
	case ECharacterPropertyTypeEnum::VE_AccuracyValue:
		return false;

	case ECharacterPropertyTypeEnum::VE_AttackRange:
	case ECharacterPropertyTypeEnum::VE_MoveSpeed:
	case ECharacterPropertyTypeEnum::VE_AttackSpeed:
		return true;

	default:
		UE_LOG(LogUI, Log, TEXT("UDescriptionFunctionLibrary::IsHeroPropertyFloat: unexpected CharacterPropertyType"));
		return true;
	}
}


bool UDescriptionFunctionLibrary::IsHeroPropertyPerMil(ECharacterPropertyTypeEnum CharacterProperty)
{
	switch (CharacterProperty)
	{
	case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
	case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
	case ECharacterPropertyTypeEnum::VE_Armor:
	case ECharacterPropertyTypeEnum::VE_MagicResistance:
	case ECharacterPropertyTypeEnum::VE_MaxHP:
	case ECharacterPropertyTypeEnum::VE_HP:
	case ECharacterPropertyTypeEnum::VE_DodgeValue:
	case ECharacterPropertyTypeEnum::VE_MoveSpeed:
	case ECharacterPropertyTypeEnum::VE_AttackRange:
		return false;

	case ECharacterPropertyTypeEnum::VE_CriticalValue:
	case ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale:
	case ECharacterPropertyTypeEnum::VE_PierceValue:
	case ECharacterPropertyTypeEnum::VE_AccuracyValue:
	case ECharacterPropertyTypeEnum::VE_AttackSpeed:
		return true;

	default:
		UE_LOG(LogUI, Log, TEXT("UDescriptionFunctionLibrary::IsHeroPropertyPerMil: unexpected CharacterPropertyType"));
		return false;
	}
}

bool UDescriptionFunctionLibrary::IsHeroPropertyContributedByItem(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo)
{
	if (IsHeroPropertyFloat(CharacterProperty))
	{
		float Value = InCharacterBattleInfo->TotalItemCharacterProperty.GetCharacterProperty<float>(CharacterProperty);
		return !FMath::IsNearlyZero(Value);
	}
	else
	{
		int32 Value = InCharacterBattleInfo->TotalItemCharacterProperty.GetCharacterProperty<int32>(CharacterProperty);
		return Value != 0; // Better be not less than 0
	}
}

bool UDescriptionFunctionLibrary::IsHeroPorpetryAdditional(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo)
{
	if (IsHeroPropertyFloat(CharacterProperty))
	{
		float Value = InCharacterBattleInfo->AdditionalCharacterProperty.GetCharacterProperty<float>(CharacterProperty);
		return !FMath::IsNearlyZero(Value);
	}
	else
	{
		int32 Value = InCharacterBattleInfo->AdditionalCharacterProperty.GetCharacterProperty<int32>(CharacterProperty);
		return Value != 0; // Better be not less than 0
	}
}

/*
======================================================================================================================================================================================================================================
Skill
======================================================================================================================================================================================================================================
*/
void UDescriptionFunctionLibrary::MakeFormatNamedArguments_WorkedSkillTableInfo(const FString& SkillDesc, const FWorkedSkillTableInfo* InWorkedSkillTableInfo, FFormatNamedArguments& InTargetInfoFormatNamedArguments, const TSet<FString>& CalculatePropertys, bool bActiveSkill /*= false*/)
{ 
	for (TFieldIterator<UProperty> PropIt(InWorkedSkillTableInfo->SkillTableInfo_V2->StaticStruct()); PropIt; ++PropIt)
	{
		UProperty* Property		= *PropIt;

		FString PropertyName		= Property->GetNameCPP();

		if (PropertyName.Contains(TEXT("_DEPRECATED")))
		{
			PropertyName.Replace(TEXT("_DEPRECATED"), TEXT(""));
		}

		//auto PropertyType		= Property->GetCPPType();
		FString DescriptionStr	= "";
		//int32 AttackPower		= 0;

		if (PropertyName.Equals(TEXT("CastingTime"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CastingTime, 2);
		}
		else if (PropertyName.Equals(TEXT("CoolTime"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CoolTime, 2);
		}
		else if (PropertyName.Equals(TEXT("Interval"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->Interval, 2);
		}
		else if (PropertyName.Equals(TEXT("CastingRange"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->CastingRange);
			
		}
		else if (PropertyName.Equals(TEXT("CasterSlotLevel"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterSlotLevel);
		}
		else if (PropertyName.Equals(TEXT("CasterDuration"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterDuration, 2);
		}
		else if (PropertyName.Equals(TEXT("CasterPercent1"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterPercent1, 2);
		}
		else if (PropertyName.Equals(TEXT("CasterValue1"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterValue1);
		}
		else if (PropertyName.Equals(TEXT("CasterPercent2"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterPercent2, 2);
		}
		else if (PropertyName.Equals(TEXT("CasterValue2"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterValue2);
		}
		else if (PropertyName.Equals(TEXT("CasterPercent3"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterPercent3, 2);
		}
		else if (PropertyName.Equals(TEXT("CasterValue3"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterValue3);
		}
		else if (PropertyName.Equals(TEXT("CasterPercent4"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterPercent4, 2);
		}
		else if (PropertyName.Equals(TEXT("CasterValue4"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterValue4);
		}
		else if (PropertyName.Equals(TEXT("TargetSlotLevel"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetSlotLevel);
		}
		else if (PropertyName.Equals(TEXT("TargetDuration"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetDuration, 2);
		}
		else if (PropertyName.Equals(TEXT("TargetPercent1"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetPercent1, 2);
		}
		else if (PropertyName.Equals(TEXT("TargetValue1"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetValue1);
		}
		else if (PropertyName.Equals(TEXT("TargetPercent2"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetPercent2, 2);
		}
		else if (PropertyName.Equals(TEXT("TargetValue2"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetValue2);
		}
		else if (PropertyName.Equals(TEXT("TargetPercent3"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetPercent3, 2);
		}
		else if (PropertyName.Equals(TEXT("TargetValue3"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetValue3);
		}
		else if (PropertyName.Equals(TEXT("TargetPercent4"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetPercent4, 2);
		}
		else if (PropertyName.Equals(TEXT("TargetValue4"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->TargetValue4);
		}
		else if (PropertyName.Equals(TEXT("ApplyProcedure"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->ApplyProcedure);
		}
		else if (PropertyName.Equals(TEXT("SummonTime"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->GroundDuration, 2);
		}
// 		else if (PropertyName.Equals("PerformanceActionTime"))
// 		{
// 			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->PerformanceActionTime, 2);
// 		}
// 		else if (PropertyName.Equals("AcquireLevel"))
// 		{
// 			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->AcquireLevel);
// 		}
// 		else if (PropertyName.Equals("AcquireMoney"))
// 		{
// 			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->AcquireMoney);
// 		}
// 		else if (PropertyName.Equals("AcquireRune"))
// 		{
// 			DescriptionStr = GetIntToAbsIntToString(InWorkedSkillTableInfo->SkillTableInfo_V2->AcquireRune);
// 		}
		else if (PropertyName.Equals(TEXT("ApplyCheckTick"), ESearchCase::IgnoreCase))
		{
			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->Interval, 2);
		}		
// 		else if (PropertyName.Equals("Duration"))
// 		{
// 			DescriptionStr = GetFloatToStringWithPrecision(InWorkedSkillTableInfo->SkillTableInfo_V2->CasterDuration, 2);
// 		}
// 		
		
		InTargetInfoFormatNamedArguments.Emplace(PropertyName, FText::FromString(DescriptionStr));
	}
}


float UDescriptionFunctionLibrary::GetSkillModifyValue(EScaleValueTypeEnum InScaleValueTypeEnum, float InBuffModifyValue, EBuffPropensityEnum InSkillBuffPropnensity, float CharacterPropertyValue, bool bIsCalculate)
{
	float PreciseValue	= 0;
	PreciseValue		= InBuffModifyValue;
	
	if(bIsCalculate)
	{
		switch (InScaleValueTypeEnum)
		{
		case EScaleValueTypeEnum::VE_100Rate:
			PreciseValue = CharacterPropertyValue * ( InBuffModifyValue / 100.0f );
			break;
		case EScaleValueTypeEnum::VE_Normal:
			PreciseValue = InBuffModifyValue;
			break;
		default:
			PreciseValue = InBuffModifyValue;
			break;
		}
	}
	
	return PreciseValue;
}

FString UDescriptionFunctionLibrary::GetFloatToStringWithPrecision(float FloatValue, int32 Precision, bool IncludeLeadingZero)
{
	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumIntegralDigits		= (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits		= 1000000000;
	NumberFormat.MinimumFractionalDigits	= Precision;
	NumberFormat.MaximumFractionalDigits	= Precision;

	float AbsValue = FMath::Abs(FloatValue);
	
	FString PrecisionString = FText::AsNumber(AbsValue, &NumberFormat).ToString();
	
	if (PrecisionString.Contains(".00"))
	{
		PrecisionString.RemoveFromEnd(".00");
	}
	else
	{
		PrecisionString.RemoveFromEnd("0");
	}

	return PrecisionString;
}


FString UDescriptionFunctionLibrary::GetIntToAbsIntToString(int32 IntValue)
{
	int32 AbsValue = FMath::Abs<int32>(IntValue);	
	
	return FText::AsNumber(AbsValue).ToString();
}

FText UDescriptionFunctionLibrary::GetItemEquipPositionToText(EItemEquipPositionEnum EquipPosition)
{
	// temp thing
	//return UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EItemEquipPositionEnum"), (int32)EquipPosition);

	FName UIStringKey = NAME_None;

	switch (EquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
		UIStringKey = TEXT("UI_ItemManage_EquipType01");
		break;
	case EItemEquipPositionEnum::VE_Armor:
		UIStringKey = TEXT("UI_ItemManage_EquipType02");
		break;
	case EItemEquipPositionEnum::VE_Pants:
		UIStringKey = TEXT("UI_ItemManage_EquipType03");
		break;
	case EItemEquipPositionEnum::VE_Ring:
		UIStringKey = TEXT("UI_ItemManage_EquipType04");
		break;
	case EItemEquipPositionEnum::VE_Necklace:
		UIStringKey = TEXT("UI_ItemManage_EquipType05");
		break;
	default:
		check(false);
		break;
	}

	return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UIStringKey);
}

/*
======================================================================================================================================================================================================================================
Item
======================================================================================================================================================================================================================================
*/

FString UDescriptionFunctionLibrary::GetItemPropertyString(const FString& PropertyName, UCharacterBattleInfo* InCharacterBattleInfo)
{
	auto TotalItemCharacterProperty = InCharacterBattleInfo->TotalItemCharacterProperty;
	FString DescriptionStr = "";

	if (PropertyName.Equals("PhysicalAttackPower"))
	{
		return FString::FromInt(TotalItemCharacterProperty.PhysicalAttackPower);
	}
	else if (PropertyName.Equals("MagicAttackPower"))
	{
		return FString::FromInt(TotalItemCharacterProperty.MagicAttackPower);
	}
// 	else if (PropertyName.Equals("MagicCriticalRate"))
// 	{
// 		float PreciseValue = TotalItemCharacterProperty.MagicCriticalRate;
// 
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
	else if (PropertyName.Equals("AttackSpeed"))
	{
		float PreciseValue = (TotalItemCharacterProperty.AttackSpeed) * 100.0f;
		if (PreciseValue < 0.0f)
		{
			PreciseValue = 0.0f;
		}
		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
	else if (PropertyName.Equals("CriticalValue"))
	{
		return FString::FromInt(TotalItemCharacterProperty.CriticalValue);
	}
	else if (PropertyName.Equals("CriticalDamageRate_Scale"))
	{
		float PreciseValue = TotalItemCharacterProperty.CriticalDamageRate_Scale / 100.0f;

		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}
// 	else if (PropertyName.Equals("CriticalResistValue"))
// 	{
// 		float PreciseValue = TotalItemCharacterProperty.CriticalResistValue;
// 
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
// 	else if (PropertyName.Equals("CriticalResistDamage"))
// 	{
// 		float PreciseValue = TotalItemCharacterProperty.CriticalResistDamage;
// 
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
	else if (PropertyName.Equals("AccuracyValue"))
	{
		return FString::FromInt(TotalItemCharacterProperty.AccuracyValue);
	}
	else if (PropertyName.Equals("PierceValue"))
	{
		return FString::FromInt(TotalItemCharacterProperty.PierceValue);
	}
	else if (PropertyName.Equals("MaxHP"))
	{
		return FString::FromInt(TotalItemCharacterProperty.MaxHP);
	}
	else if (PropertyName.Equals("Armor"))
	{
		return FString::FromInt(TotalItemCharacterProperty.Armor);
	}
	else if (PropertyName.Equals("MagicResistance"))
	{
		return FString::FromInt(TotalItemCharacterProperty.MagicResistance);
	}
// 	else if (PropertyName.Equals("ParryValue"))
// 	{
// 		return FString::FromInt(TotalItemCharacterProperty.ParryValue);
// 	}
// 	else if (PropertyName.Equals("ParryDamageReduceRate_Scale"))
// 	{
// 		float PreciseValue = TotalItemCharacterProperty.ParryDamageReduceRate_Scale / 100.0f;
// 
// 		return GetFloatToStringWithPrecision(PreciseValue, 2);
// 	}
	else if (PropertyName.Equals("DodgeValue"))
	{
		return FString::FromInt(TotalItemCharacterProperty.DodgeValue);
	}
	else if (PropertyName.Equals("MoveSpeed"))
	{
		float PreciseValue = TotalItemCharacterProperty.MoveSpeed / 100.0f;

		return GetFloatToStringWithPrecision(PreciseValue, 2);
	}

	return DescriptionStr;
}

void UDescriptionFunctionLibrary::MakePrecision(FString& InString, int32 FrontZeroCount)
{
	if (InString.Len() >= FrontZeroCount)
		return;

	for (int32 i = 0; i < FrontZeroCount - InString.Len(); ++i)
		InString.InsertAt(0, "0");
}

void UDescriptionFunctionLibrary::GetTimeStr(float Time, FString& Min, FString& Sec)
{
	int32 iMin = (int32)(Time / 60.0f);
	int32 iSec = (int32)(Time - (float)iMin * 60.0f);

	Min = FString::FromInt(iMin / 10);
	Min += FString::FromInt(iMin % 10);
	Sec = FString::FromInt(iSec / 10);
	Sec += FString::FromInt(iSec % 10);
}

void UDescriptionFunctionLibrary::GetTimeStrOneParam(float Time, FString& TimeStr)
{
	int32 iHour = (int32)(Time / 3600.0f);
	int32 iMin = (int32)((Time - (float)iHour * 3600.0f) / 60.0f);
	int32 iSec = (int32)(Time - (float)iHour * 3600.0f - (float)iMin * 60.0f);

	TimeStr = "";
#ifdef RULE_TESTDELETE
	TimeStr += FString::FromInt(iHour * 0.1f);
#else
	TimeStr += FString::FromInt(iHour / 10);
#endif
	TimeStr += FString::FromInt(iHour % 10);
	TimeStr += ":";
#ifdef RULE_TESTDELETE
	TimeStr += FString::FromInt(iMin * 0.1f);
#else
	TimeStr += FString::FromInt(iMin / 10);
#endif
	TimeStr += FString::FromInt(iMin % 10);
	TimeStr += ":";
#ifdef RULE_TESTDELETE
	TimeStr += FString::FromInt(iSec * 0.1f);
#else
	TimeStr += FString::FromInt(iSec / 10);
#endif
	TimeStr += FString::FromInt(iSec % 10);
}

/*
String UtilFunction
*/

FText UDescriptionFunctionLibrary::GetLocalizedString(EStringTableType TableType, FName StringID)
{
	if (StringID.IsNone())
		return FText();

	FText StringValue;

	if (ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(TableType, StringID, StringValue) == false)
	{
		return FText();
	}

	return StringValue;
}

FName UDescriptionFunctionLibrary::GetItemName(FName ItemID)
{
	FItemTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemID);
	if (TableInfo)
	{
		return TableInfo->ItemName;
	}

	return FName();
}

// FName UDescriptionFunctionLibrary::GetWalletPropertyName(FName WalletType)
// {
// 	UEnum* WALLET_TYPE = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWALLET_TYPE"), true);
// 	//FCurrencyPropertyTableInfo* CurrencyPropertyTableInfo = nullptr;
// 	//FName	CurrencyPropertyKey;
// 	FString	EnumString = WALLET_TYPE->GetNameStringByIndex((int32)WalletType);
// 	FName CurrencyPropertyKey = FName(*EnumString);
// 
// 	return CurrencyPropertyKey;
// }

void UDescriptionFunctionLibrary::GetTimeStringByInteger(FString& InTimeValue, int32 InTimeIntegerValue)
{
	InTimeValue = FString::FromInt(InTimeIntegerValue);

	if (InTimeIntegerValue < 10)
	{
		InTimeValue = "0" + InTimeValue;
	}
}
