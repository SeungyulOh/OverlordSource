// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "Table/LevelAppliedTables/WorkedCharacterTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Table/ItemTableInfo.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "WorkedItemTableInfo.h"


void FWorkedItemTableInfo::SetDataFromItemInfo(FItemTableInfo* itemInfo, int32 EnchantLevel, int32 InItemLevel /*=1*/)
{
	if (itemInfo == nullptr)
		return;

	Data = *itemInfo;

	BasePhysicalAttackPower		= Data.PhysicalAttackPower;
	BaseMagicAttackPower		= Data.MagicAttackPower;
	BaseArmor					= Data.Armor;
	BaseMagicResistance			= Data.MagicResistance;
	BaseMaxHP					= Data.MaxHP;

	ItemLevel = InItemLevel;

	ApplyEnchantValue(EnchantLevel);
}


void FWorkedItemTableInfo::ApplyEnchantValue(int32 EnchantLevel)
{
	switch (Data.EquipPosition)
	{
		case EItemEquipPositionEnum::VE_Weapon:
		case EItemEquipPositionEnum::VE_Armor:
		case EItemEquipPositionEnum::VE_Pants:
		case EItemEquipPositionEnum::VE_Ring:
		{
			SetItemLevel();

			BasePhysicalAttackPower = Data.PhysicalAttackPower;
			BaseMagicAttackPower = Data.MagicAttackPower;
			BaseArmor = Data.Armor;
			BaseMagicResistance = Data.MagicResistance;
			BaseMaxHP = Data.MaxHP;

		}
		break;
	}

	if (EnchantLevel > 0)
	{
		Data.PhysicalAttackPower = SetEnchantValue(BasePhysicalAttackPower, EnchantLevel);
		Data.MagicAttackPower = SetEnchantValue(BaseMagicAttackPower, EnchantLevel);
		Data.Armor = SetEnchantValue(BaseArmor, EnchantLevel);
		Data.MagicResistance = SetEnchantValue(BaseMagicResistance, EnchantLevel);
		Data.MaxHP = SetEnchantValue(BaseMaxHP, EnchantLevel);
	}

	EnchantPhysicalAttackPowerNotAddOption	= Data.PhysicalAttackPower;
	EnchantMagicAttackPowerNotAddOption		= Data.MagicAttackPower;
	EnchantArmorNotAddOption				= Data.Armor;
	EnchantMagicResistanceNotAddOption		= Data.MagicResistance;
	EnchantMaxHPNotAddOption				= Data.MaxHP;
}

void FWorkedItemTableInfo::SetItemLevel()
{
	if (ItemLevel <= 1)
		return;

	//temp...

	int32 Max = ItemLevel + 1;
	FITEM_ICON_INFO Info;
	Info.ItemEquipType = Data.EquipType;
	Info.ItemEquipPosition = Data.EquipPosition;
	Info.itemGrade = Data.ItemGrade;
	for (int32 i = 1; i < Max; ++i)
	{
		auto TargetLevelUpTable = UUIFunctionLibrary::GetItemLevelUpTableInfo(Info, i);
		if (TargetLevelUpTable == nullptr)
			break;

		switch (Data.EquipPosition)
		{
			case EItemEquipPositionEnum::VE_Weapon:
			{
				if (Data.PhysicalAttackPower != 0)
				{
					Data.PhysicalAttackPower += TargetLevelUpTable->PlusPhysicalAttackPower;
				}
				if (Data.MagicAttackPower != 0)
				{
					Data.MagicAttackPower += TargetLevelUpTable->PlusMagicAttackPower;
				}
			}
			break;
			case EItemEquipPositionEnum::VE_Armor:
			{
				if (Data.Armor != 0)
				{
					Data.Armor += TargetLevelUpTable->PlusArmor;
				}
				if (Data.MagicResistance != 0)
				{
					Data.MagicResistance += TargetLevelUpTable->PlusMagicResistance;
				}
			}
			break;
			case EItemEquipPositionEnum::VE_Pants:
			{
				if (Data.Armor != 0)
				{
					Data.Armor += TargetLevelUpTable->PlusArmor;
				}
				if (Data.MagicResistance != 0)
				{
					Data.MagicResistance += TargetLevelUpTable->PlusMagicResistance;
				}
			}
			break;
			case EItemEquipPositionEnum::VE_Ring:
			{
				if (Data.MaxHP != 0)
				{
					//Data.MaxHP += TargetLevelUpTable->PlusMaxHP;
				}
			}
			break;
		}
		
	}

}

int32 FWorkedItemTableInfo::SetEnchantValue(int32 OriginalValue, int32 EnchantLevel)
{
	float BaseValue			= (float)OriginalValue;
	float AddEnchantValue	= (BaseValue * EnchantLevel) * 0.01f;// / 100.0f;
	float FinalValue		= BaseValue + AddEnchantValue;
	return FMath::TruncToInt(FinalValue);
} 


//void FWorkedItemTableInfo::SetItemGenerateFixedOption(EItemGenerateOptionType InOptionType, float InValue, int32 InEnchantValue, int32 InEnchantLevel)
//{
//	//auto FixedOption = UTableManager::GetInstancePtr()->GetItemFixedOptionRow(Data->ItemGenerateOptionKey);
//	//if (FixedOption)
//	{
//	//	auto OptionType = UUtilFunctionLibrary::GetItemOptionType(Element.Option);
//	}
//
//	float ResultValue = InValue + (InEnchantValue * InEnchantLevel);
//	SetItemGenerateOption(InOptionType, ResultValue);
//}
//
//
//void FWorkedItemTableInfo::SetItemGenerateOption(EItemGenerateOptionType InOptionType, float InValue)
//{
//	if(InValue == 0)
//		return;
//
//	switch (InOptionType)
//	{
//		case EItemGenerateOptionType::VE_AttackSpeed:					Data.AttackSpeed += InValue;					break;
//		case EItemGenerateOptionType::VE_MoveSpeed:						MoveSpeed = InValue;							break;
//		case EItemGenerateOptionType::VE_PierceValue:					Data.PierceValue += InValue;					break;
//		case EItemGenerateOptionType::VE_MaxHP:							Data.MaxHP += InValue;							break;
//		case EItemGenerateOptionType::VE_ParryValue:					Data.ParryValue += InValue;						break;
//		case EItemGenerateOptionType::VE_PhysicalAttackPower:			Data.PhysicalAttackPower += InValue;			break;
//		case EItemGenerateOptionType::VE_MagicAttackPower:				Data.MagicAttackPower += InValue;				break;
//		case EItemGenerateOptionType::VE_Armor:							Data.Armor += InValue;							break;
//		case EItemGenerateOptionType::VE_MagicResistance:				Data.MagicResistance += InValue;				break;
//		case EItemGenerateOptionType::VE_DodgeValue:					Data.DodgeValue += InValue;						break;
//		case EItemGenerateOptionType::VE_CriticalValue:					Data.CriticalValue += InValue;					break;
//		case EItemGenerateOptionType::VE_AccuracyValue:					Data.AccuracyValue += InValue;					break;
//		case EItemGenerateOptionType::VE_CriticalDamageRate_Scale:		Data.CriticalDamageRate_Scale += InValue;		break;
//		case EItemGenerateOptionType::VE_ParryDamageReduceRate_Scale:	Data.ParryDamageReduceRate_Scale += InValue;	break;
//		case EItemGenerateOptionType::VE_PierceDefenceBonus:			Data.PierceDefenceBonus += InValue;				break;
//	}
//}


//void FWorkedItemTableInfo::ApplyItemOption(const FWorkedCharacterTableInfo& EquippedCharacterWorkedTableInfo, FName ItemID, TArray<int32>& Options)
//{
//	FItemTableInfo* ItemTable = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemID);	
//	if (ItemTable == nullptr)
//		return;
//
//	if (ItemTable->ItemGenerateOptionKey.IsNone())
//		return;
//
//	FItemGenerateOptionTableInfo* OptionGenerateTableInfo = UTableManager::GetInstancePtr()->GetItemGenerateOptionRow(ItemTable->ItemGenerateOptionKey);
//	if (OptionGenerateTableInfo == nullptr)
//		return;
//
//	SetItemGenerateOption(OptionGenerateTableInfo->ItemGenerateOption1, OptionGenerateTableInfo->ItemGenerateOption1Value);
//	SetItemGenerateOption(OptionGenerateTableInfo->ItemGenerateOption2, OptionGenerateTableInfo->ItemGenerateOption2Value);
//	SetItemGenerateOption(OptionGenerateTableInfo->ItemGenerateOption3, OptionGenerateTableInfo->ItemGenerateOption3Value);
//
//	if (ItemTable)
//	{
//		switch (ItemTable->EquipPosition)
//		{
//		case EItemEquipPositionEnum::VE_Weapon:
//		case EItemEquipPositionEnum::VE_Armor:
//		case EItemEquipPositionEnum::VE_Pants:
//		case EItemEquipPositionEnum::VE_Ring:
//		{
//			if (!OptionGenerateTableInfo->BonusOnEquip.IsNone())
//			{
//				bool bExe = false;
//				FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(OptionGenerateTableInfo->BonusOnEquip);
//				if (CharacterTableInfo)
//				{
//					if (CharacterTableInfo->DisplayName == EquippedCharacterWorkedTableInfo.Data.DisplayName)
//						bExe = true;
//				}
//
//				if ((OptionGenerateTableInfo->BonusOnEquip == TEXT("All")) || bExe)
//				{
//					if (OptionGenerateTableInfo->ItemGenerateBonusOption != EItemGenerateOptionType::VE_None)
//					{
//						if (Options.IsValidIndex(0))
//						{
//							SetItemGenerateOption(OptionGenerateTableInfo->ItemGenerateBonusOption, (float)Options[0]);
//						}
//					}
//				}
//			}
//		}
//		break;
//		}
//	}
//	for (auto OptionRowInt : Options)
//	{
//		FString OptionRowStr = FString::FromInt(OptionRowInt);
//		auto OptionTableInfo = UTableManager::GetInstancePtr()->GetItemOptionRow(FName(*OptionRowStr));
//		if (OptionTableInfo == nullptr)
//			continue;
//		if (OptionTableInfo->incType == EItemOptionIncreaseType::VE_Prob)
//		{
//			switch (OptionTableInfo->OptionType)
//			{
//			case EItemOptionType::VE_AttackSpeed:
//				Data.AttackSpeed += (EquippedCharacterWorkedTableInfo.Data.AttackSpeed * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break; 
//			case EItemOptionType::VE_PierceValue:
//				Data.PierceValue += (EquippedCharacterWorkedTableInfo.Data.PierceValue * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_CriticalRate_Scale:
//				Data.CriticalRate_Scale += (EquippedCharacterWorkedTableInfo.Data.CriticalRate_Scale * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_CriticalDamage_Scale:
//				Data.CriticalDamageRate_Scale += (EquippedCharacterWorkedTableInfo.Data.CriticalDamageRate_Scale * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_MoveSpeed:
//				MoveSpeed = (EquippedCharacterWorkedTableInfo.Data.MoveSpeed * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//// 			case EItemOptionType::VE_DamageBonus_Scale:
//// 				Data.PhysicalAttackPower	+= (EquippedCharacterWorkedTableInfo.Data.PhysicalAttackPower * ((float)OptionTableInfo->value / (float)ScaleNumber));
//// 				Data.MagicAttackPower		+= (EquippedCharacterWorkedTableInfo.Data.MagicAttackPower * ((float)OptionTableInfo->value / (float)ScaleNumber));
//// 				break;
//			case EItemOptionType::VE_PDamageBonus_Scale:
//				Data.PhysicalAttackPower += (EquippedCharacterWorkedTableInfo.Data.PhysicalAttackPower * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_MaxHP:
//				Data.MaxHP += (EquippedCharacterWorkedTableInfo.Data.MaxHP * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_BlockRate_Scale:
//				break;
//			case EItemOptionType::VE_BlockValue:
//				break;
//			case EItemOptionType::VE_PDefBonusValue:
//				Data.Armor += (EquippedCharacterWorkedTableInfo.Data.Armor * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_MDefBonusValue:
//				Data.MagicResistance += (EquippedCharacterWorkedTableInfo.Data.MagicResistance * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_ParryRate_Scale:
//				Data.ParryRate_Scale += (EquippedCharacterWorkedTableInfo.Data.ParryRate_Scale * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			case EItemOptionType::VE_ParryValue:
//				Data.ParryValue += (EquippedCharacterWorkedTableInfo.Data.ParryValue * ((float)OptionTableInfo->value / (float)ScaleNumber));
//				break;
//			}
//		}
//		else
//		{
//			switch (OptionTableInfo->OptionType)
//			{
//			case EItemOptionType::VE_AttackSpeed:
//				Data.AttackSpeed += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_PierceValue:
//				Data.PierceValue += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_CriticalRate_Scale:
//				Data.CriticalRate_Scale += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_CriticalDamage_Scale:
//				Data.CriticalDamageRate_Scale += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_MoveSpeed:
//				MoveSpeed = OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_DamageBonus_Scale:
//				Data.PhysicalAttackPower += OptionTableInfo->value;
//				Data.MagicAttackPower += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_PDamageBonus_Scale:
//				Data.PhysicalAttackPower += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_MaxHP:
//				Data.MaxHP += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_HPDrainValue:
//				Data.HPDrainValue += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_BlockRate_Scale:
//				break;
//			case EItemOptionType::VE_BlockValue:
//				break;
//			case EItemOptionType::VE_PDefBonusValue:
//				Data.Armor += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_MDefBonusValue:
//				Data.MagicResistance += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_ParryRate_Scale:
//				Data.ParryRate_Scale += OptionTableInfo->value;
//				break;
//			case EItemOptionType::VE_ParryValue:
//				Data.ParryValue += OptionTableInfo->value;
//				break;
//			}
//		}
//	}
//}

//void FWorkedItemTableInfo::ApplyGemItemOption(const FWorkedCharacterTableInfo& EquippedCharacterWorkedTableInfo, const FITEM& Item)
//{
//	for (FString JewelID : Item.jewels)
//	{
//		if( JewelID.IsEmpty())
//			continue;
//
//		FGemItemOptionTableInfo* GemOptionTable = UTableManager::GetInstancePtr()->GetGemItemOptionRow( FName(*JewelID));
//		if(GemOptionTable == nullptr)
//			continue;
//
//		switch (GemOptionTable->Jewel_Option_Type)
//		{
//		case EGemItemPropertyTypeEnum::VE_PhysicalAttackPower:
//			Data.PhysicalAttackPower += GemOptionTable->Jewel_Option_Value;
//			break;
//		case EGemItemPropertyTypeEnum::VE_MagicAttackPower:
//			Data.MagicAttackPower += GemOptionTable->Jewel_Option_Value;
//			break;
//		case EGemItemPropertyTypeEnum::VE_Armor:
//			Data.Armor	+= GemOptionTable->Jewel_Option_Value;
//			break;
//		case EGemItemPropertyTypeEnum::VE_MagicResistance:
//			Data.MagicResistance += GemOptionTable->Jewel_Option_Value;
//			break;
//		case EGemItemPropertyTypeEnum::VE_CriticalDamageRate_Scale:
//			Data.CriticalDamageRate_Scale += GemOptionTable->Jewel_Option_Value;
//			break;
//		case EGemItemPropertyTypeEnum::VE_MaxHP:
//			Data.MaxHP += GemOptionTable->Jewel_Option_Value;
//			break;
//		case EGemItemPropertyTypeEnum::VE_DamageBonus:
//			Data.PhysicalAttackPower += (EquippedCharacterWorkedTableInfo.Data.PhysicalAttackPower + GemOptionTable->Jewel_Option_Value);
//			Data.MagicAttackPower += (EquippedCharacterWorkedTableInfo.Data.MagicAttackPower + GemOptionTable->Jewel_Option_Value);
//			break;
//
//		default:
//			break;
//		}
//
//	}
//}
