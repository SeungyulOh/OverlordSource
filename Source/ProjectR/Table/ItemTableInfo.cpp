// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ItemTableInfo.h"

#include "ItemLevelUpTableInfo.h"



#include "UtilFunctionIntegrated.h"


#include "Gameinfo/RInventory.h"

//#include "Item/WeaponBase.h"

//#define GET_MESH_ASSET( X )\
//if (IsRWeapon)\
//{\
//	WeaponUClass = X##_RMeshRef.Get();\
//	if (WeaponUClass == nullptr)\
//	{\
//		X##_RMeshRef.ToSoftObjectPath().TryLoad();\
//		WeaponUClass = X##_RMeshRef.Get();\
//	}\
//}\
//else\
//{\
//	WeaponUClass = X##_LMeshRef.Get();\
//	if (WeaponUClass == nullptr)\
//	{\
//		X##_LMeshRef.ToSoftObjectPath().TryLoad();\
//		WeaponUClass = X##_LMeshRef.Get();\
//	}\
//}
//
//
//UClass* FItemTableInfo::GetWeaponBlueprintClass(bool IsRWeapon, FName InCustomTargetCharacterID )
//{
//	UClass* WeaponUClass = nullptr;
//	
//	if( !Mesh_CustomTarget01.IsNone() )
//	{
//		if (Mesh_CustomTarget01.IsEqual(InCustomTargetCharacterID))
//		{
//			GET_MESH_ASSET(CustomTarget01);			
//			return WeaponUClass;
//		}
//	}else if (!Mesh_CustomTarget02.IsNone())
//	{
//		if (Mesh_CustomTarget02.IsEqual(InCustomTargetCharacterID))
//		{
//			GET_MESH_ASSET(CustomTarget02);
//			return WeaponUClass;
//		}
//	}
//
//	GET_MESH_ASSET(Main);
//
//	return WeaponUClass;
//}

bool FItemTableInfo::CanEquip(EClassTypeEnum ClassType, EItemEquipTypeEnum EquipType)
{
	// Apparently EClassTypeEnum::VE_Buffer means ones that *buff* other characters. Not used anyway.

	switch (EquipType)
	{
	case EItemEquipTypeEnum::VE_Sword:
	case EItemEquipTypeEnum::VE_Dagger:
	case EItemEquipTypeEnum::VE_Hammer:
	case EItemEquipTypeEnum::VE_Rod:
		return ClassType == EClassTypeEnum::VE_Tanker || ClassType == EClassTypeEnum::VE_MeleeDealer;
		break;

	case EItemEquipTypeEnum::VE_Bow:
	case EItemEquipTypeEnum::VE_Crossbow:
	case EItemEquipTypeEnum::VE_Gun:
		return ClassType == EClassTypeEnum::VE_RangeDealer;
		break;

	case EItemEquipTypeEnum::VE_Staff:
	case EItemEquipTypeEnum::VE_Magic:
	case EItemEquipTypeEnum::VE_Wand:
		return ClassType == EClassTypeEnum::VE_MagicDealer || ClassType == EClassTypeEnum::VE_Supporter;
		break;

	default:
		return true;
	}
}

//bool FItemTableInfo::CanEquip(EAttackTypeEnum AttackType, EItemEquipTypeEnum EquipType)
//{
//	switch (EquipType)
//	{
//	case EItemEquipTypeEnum::VE_Sword:
//	case EItemEquipTypeEnum::VE_Dagger:
//	case EItemEquipTypeEnum::VE_Hammer:
//	case EItemEquipTypeEnum::VE_Rod:
//		return AttackType == EAttackTypeEnum::VE_Melee;
//		break;
//
//	case EItemEquipTypeEnum::VE_Bow:
//	case EItemEquipTypeEnum::VE_Crossbow:
//	case EItemEquipTypeEnum::VE_Gun:
//		return AttackType == EAttackTypeEnum::VE_Range;
//		break;
//
//	case EItemEquipTypeEnum::VE_Staff:
//	case EItemEquipTypeEnum::VE_Magic:
//	case EItemEquipTypeEnum::VE_Wand:
//		return AttackType == EAttackTypeEnum::VE_Magic;
//		break;
//
//	default:
//		return true;
//	}
//}

void FItemTableInfo::GetBasicOptions(TArray<int32>& OutArray) const
{
	OutArray.Empty();
	OutArray.AddZeroed((int32)EItemStatOption::Max);

	OutArray[(int32)EItemStatOption::PhysicalAttackPower] = PhysicalAttackPower;
	OutArray[(int32)EItemStatOption::MagicAttackPower] = MagicAttackPower;
	OutArray[(int32)EItemStatOption::Armor] = Armor;
	OutArray[(int32)EItemStatOption::MagicResistance] = MagicResistance;
	OutArray[(int32)EItemStatOption::MaxHP] = MaxHP;
	OutArray[(int32)EItemStatOption::AttackRange] = AttackRange;
	OutArray[(int32)EItemStatOption::MoveSpeed] = MoveSpeed;
	OutArray[(int32)EItemStatOption::DodgeValue] = DodgeValue;
	OutArray[(int32)EItemStatOption::CriticalValue] = CriticalValue;
	OutArray[(int32)EItemStatOption::CriticalDamageRate_Scale] = CriticalDamageRate_Scale;
	OutArray[(int32)EItemStatOption::PierceValue] = PierceValue;
	OutArray[(int32)EItemStatOption::AccuracyValue] = AccuracyValue;
}

int32 FItemTableInfo::GetCombatPower(const FITEM* ItemData)
{
	// (design) °­¼®Á¦
	// preliminary formula: just sum all stat values

	TArray<int32> ItemStatOptions;
	ItemStatOptions.AddZeroed((int32)EItemStatOption::Max);
	URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(ItemStatOptions, *ItemData);

	float Sum = 0.0;
	for (int32 i = 0; i < ItemStatOptions.Num(); i++)
	{
		if (ItemStatOptions[i] == 0)
			continue;

		EItemStatOption StatOption = (EItemStatOption)i;
		if (IsItemStatPerMil(StatOption))
		{
			Sum += (float)ItemStatOptions[i] / 1000.;
		}
		else
		{
			Sum += (float)ItemStatOptions[i];
		}
	}

	return (int32)Sum;
}

bool FItemTableInfo::IsItemStatPerMil(EItemStatOption ItemStatOption)
{
	switch (ItemStatOption)
	{
	case EItemStatOption::PhysicalAttackPower:
	case EItemStatOption::MagicAttackPower:
	case EItemStatOption::Armor:
	case EItemStatOption::MagicResistance:
	case EItemStatOption::MaxHP:
	case EItemStatOption::AccuracyValue:
	case EItemStatOption::CriticalResistValue:
	case EItemStatOption::DodgeValue:
	case EItemStatOption::CriticalValue:
		return false;
	case EItemStatOption::PierceValue:
		return true;

	default:
		UE_LOG(LogGamePlay, Log, TEXT("FItemTableInfo::IsItemStatPerMil: unexpected ItemStatOption"));
		return false;
	}
}

FItemEnchantTableInfo* FItemEnchantTableInfo::LookUp(EItemEquipPositionEnum EquipPosition, EItemGradeEnum Grade, int32 EnchantLevel)
{
	TArray<FName> EnchantRowNames = UTableManager::GetInstancePtr()->DTItemEnchantTable->GetRowNames();
	for (FName EnchantRowName : EnchantRowNames)
	{
		FItemEnchantTableInfo* ItemEnchantInfo = UTableManager::GetInstancePtr()->GetItemEnchantRow(EnchantRowName);
		if (ItemEnchantInfo->EquipPosition == EquipPosition && ItemEnchantInfo->ItemGrade == Grade && ItemEnchantInfo->EnchantLevel == EnchantLevel)
		{
			return ItemEnchantInfo;
		}
	}

	UE_LOG(LogGamePlay, Log, TEXT("FItemEnchantTableInfo::LookUp: failed to find row"));

	return nullptr;
}

void FItemEnchantTableInfo::ApplyEnchant(const FItemTableInfo* ItemInfo, const FITEM* ItemData, TArray<int32>& BasicOptions)
{
	if (ItemData->enchant > 0)
	{
		FItemEnchantTableInfo* EnchantInfo = FItemEnchantTableInfo::LookUp(ItemInfo->EquipPosition, ItemInfo->ItemGrade, ItemData->enchant);
		if (EnchantInfo == nullptr)
			return;

		// DT_ItemEnchant.EnchantEffect is per mil
		float EnchantEffectRate = EnchantInfo->EnchantEffect / 1000.0;

		// Truncate the values
		BasicOptions[(int32)EItemStatOption::PhysicalAttackPower] = FMath::RoundToPositiveInfinity(BasicOptions[(int32)EItemStatOption::PhysicalAttackPower] + (float)BasicOptions[(int32)EItemStatOption::PhysicalAttackPower] * EnchantEffectRate);
		BasicOptions[(int32)EItemStatOption::MagicAttackPower] = FMath::RoundToPositiveInfinity(BasicOptions[(int32)EItemStatOption::MagicAttackPower] + (float)BasicOptions[(int32)EItemStatOption::MagicAttackPower] * EnchantEffectRate);
		BasicOptions[(int32)EItemStatOption::Armor] = FMath::RoundToPositiveInfinity(BasicOptions[(int32)EItemStatOption::Armor] + (float)BasicOptions[(int32)EItemStatOption::Armor] * EnchantEffectRate);
		BasicOptions[(int32)EItemStatOption::MagicResistance] = FMath::RoundToPositiveInfinity(BasicOptions[(int32)EItemStatOption::MagicResistance] + (float)BasicOptions[(int32)EItemStatOption::MagicResistance] * EnchantEffectRate);
		BasicOptions[(int32)EItemStatOption::MaxHP] = FMath::RoundToPositiveInfinity(BasicOptions[(int32)EItemStatOption::MaxHP] + (float)BasicOptions[(int32)EItemStatOption::MaxHP] * EnchantEffectRate);
	}
}

void FItemTableInfo::GetAdditionalOptions(const FITEM* ItemData, TArray<int32>& OutAdditionalOptions) const
{
	OutAdditionalOptions.Empty();
	OutAdditionalOptions.AddZeroed((int32)EItemStatOption::Max);

	EItemStatOption ItemStatOption = EItemStatOption::PhysicalAttackPower;
	int32 Value = 0;

	if (GetOneAdditionalOption(ItemData, 0, ItemStatOption, Value))
		OutAdditionalOptions[(int32)ItemStatOption] += Value;

	if (GetOneAdditionalOption(ItemData, 1, ItemStatOption, Value))
		OutAdditionalOptions[(int32)ItemStatOption] += Value;

	if (GetOneAdditionalOption(ItemData, 2, ItemStatOption, Value))
		OutAdditionalOptions[(int32)ItemStatOption] += Value;

	if (GetOneAdditionalOption(ItemData, 3, ItemStatOption, Value))
		OutAdditionalOptions[(int32)ItemStatOption] += Value;
}


bool FItemTableInfo::GetOneAdditionalOption(const FITEM* ItemData, int32 Index, EItemStatOption& OutItemStatOption, int32& OutValue) const
{
	FName RandomOptionKey = NAME_None;
	switch (Index)
	{
	case 0:
		RandomOptionKey = AddOption0;
		break;
	case 1:
		RandomOptionKey = AddOption1;
		break;
	case 2:
		RandomOptionKey = AddOption2;
		break;
	case 3:
		RandomOptionKey = AddOption3;
		break;
	default:
		check(false);
		break;
	}

	if (RandomOptionKey.IsNone())
		return false; // No random option at this index

	FItemRandomOptionTableInfo* RandomOptionInfo = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RandomOptionKey);
	if (RandomOptionInfo == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("FItemTableInfo::ApplyRandomOption: random option key %s not found in the table"), *RandomOptionKey.ToString());
		return false;
	}

	if (ItemData->options.IsValidIndex(Index) == false)
		return false;

	return RandomOptionInfo->GetOneRandomOption(ItemData->options[Index].idx, ItemData->options[Index].factor, /* out */ OutItemStatOption, /* out */ OutValue);
}

bool FItemTableInfo::IsAccessory(EItemEquipPositionEnum EquipPosition)
{
	switch (EquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
	case EItemEquipPositionEnum::VE_Armor:
	case EItemEquipPositionEnum::VE_Pants:
		return false;
		break;
	case EItemEquipPositionEnum::VE_Necklace:
	case EItemEquipPositionEnum::VE_Ring:
		return true;
		break;
	default:
		UE_LOG(LogGamePlay, Log, TEXT("FItemTableInfo::IsAccessory: unexpected EquipPosition"));
		return false;
		break;
	}
}

int32 FItemTableInfo::NumItemEquipPositions()
{
	return (int32)EItemEquipPositionEnum::VE_Max - (int32)EItemEquipPositionEnum::VE_Weapon;
}

void FItemRandomOptionTableInfo::ApplyItemRandomOptions(const FItemTableInfo* ItemInfo, const FITEM* ItemData, TArray<int32>& InOutItemStatOptions)
{
	TArray<int32> AdditionalOptions;
	ItemInfo->GetAdditionalOptions(ItemData, AdditionalOptions);

	for (int32 i = 0; i < (int32)EItemStatOption::Max; i++)
	{
		InOutItemStatOptions[i] += AdditionalOptions[i];
	}
}

bool FItemRandomOptionTableInfo::GetOneRandomOption(int32 OptionIndex, float Factor, EItemStatOption& OutItemStatOption, int32& OutValue) const
{
	if (!Options.IsValidIndex(OptionIndex))
	{
		UE_LOG(LogGamePlay, Log, TEXT("FItemRandomOptionTableInfo::GetOneRandomOption: Option idx given from server exceeds specified in the item random option table"));
		return false;
	}

	OutItemStatOption = Options[OptionIndex];

	int32 ValueBase = OptionValueBases[OptionIndex];
	float ValueAdd = OptionValueAdds[OptionIndex];

	float ComputedValue = (float)ValueBase + ValueAdd * Factor;
	OutValue = (int32)ComputedValue; // (design) truncation in display and effect

	return true;
}

void FItemRandomOptionTableInfo::GetOneAdditionalOption(FName RandomOptionKey, int32 OptionIndex, float Factor, TArray<int32>& OutAdditionalOptions)
{
	if (RandomOptionKey.IsNone())
		return;

	// Determine actual option
	FItemRandomOptionTableInfo* RandomOptionInfo = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RandomOptionKey);
	if (RandomOptionInfo == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("FItemTableInfo::ApplyRandomOption: random option key %s not found in the table"), *RandomOptionKey.ToString());
		return;
	}

	EItemStatOption ItemStatOption;
	int32 Value;
	if (!RandomOptionInfo->GetOneRandomOption(OptionIndex, Factor, /* out */ ItemStatOption, /* out */ Value))
	{
		return;
	}

	OutAdditionalOptions[(int32)ItemStatOption] += Value;
}

EItemStatOption FRuneTable::GetAdditionalOptionStat(EItemEquipPositionEnum InEquipPosition)
{
	switch (InEquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
	{
		FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneOption_WeaponID);
		if (!ensure(info != nullptr))
			return EItemStatOption::Max;
		return info->Options[0];
		break;
	}
	case EItemEquipPositionEnum::VE_Armor:
	case EItemEquipPositionEnum::VE_Pants:
	case EItemEquipPositionEnum::VE_Necklace:
	case EItemEquipPositionEnum::VE_Ring:
	{
		FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneOption_ArmorID);
		if (!ensure(info != nullptr))
			return EItemStatOption::Max;
		return info->Options[0];
		break;
	}
	default:
		check(false);
		break;
	}

	return EItemStatOption::Max;
}

int32 FRuneTable::GetAdditionalOptionValue(EItemEquipPositionEnum InEquipPosition)
{
	int32				Value = 0;

	switch (InEquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
	{
		FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneOption_WeaponID);
		if (info != nullptr)
			Value = info->OptionValueBases[0];
		break;
	}
	case EItemEquipPositionEnum::VE_Armor:
	case EItemEquipPositionEnum::VE_Pants:
	case EItemEquipPositionEnum::VE_Necklace:
	case EItemEquipPositionEnum::VE_Ring:
	{
		FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneOption_ArmorID);
		if (info != nullptr)
			Value = info->OptionValueBases[0];
		break;
	}
	default:
	{
		return INT32_MAX;
	}
	}

	return Value;
}

FText FRuneTable::GetAdditionalOptionFText(EItemEquipPositionEnum InEquipPosition)
{
	EItemStatOption		ItemStatOption = EItemStatOption::Max;

	switch (InEquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
	{
		FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneOption_WeaponID);
		if (info != nullptr)
			ItemStatOption = info->Options[0];
		break;
	}
	case EItemEquipPositionEnum::VE_Armor:
	case EItemEquipPositionEnum::VE_Pants:
	case EItemEquipPositionEnum::VE_Necklace:
	case EItemEquipPositionEnum::VE_Ring:
	{
		FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneOption_ArmorID);
		if (info != nullptr)
			ItemStatOption = info->Options[0];
		break;
	}
	}

	FName StringKey = UUIFunctionLibrary::ItemStatOptionToStringKey(ItemStatOption);
	FText StatNameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, StringKey);
	return StatNameText;
}

void FRuneTable::ApplyRunes(const FITEM* ItemData, TArray<int32>& BasicOptions)
{
	FItemTableInfo*		ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	EItemStatOption		ItemStatOption = EItemStatOption::Max;
	int32				Value = 0;

	if (ItemTableInfo == nullptr)
		return;

	for (int i = 0; i < 4; i++)
	{
		if (ItemData->runes.Num() == 0 || ItemData->runes.IsValidIndex(i) == false)
			return;
		if (ItemData->runes.Num() <= i || ItemData->runes[i].IsEqual("None") == true)
			continue;

		int32			idx = RGAMEINSTANCE(GEngine)->RInventory->RuneWallet.Find(ItemData->runes[i]);
		FRuneTable*		RuneTable = UTableManager::GetInstancePtr()->GetRuneOptionRowInt(idx + 1);

		if (RuneTable == nullptr)
			continue;

		switch (ItemTableInfo->EquipPosition)
		{
		case EItemEquipPositionEnum::VE_Weapon:
		{
			FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneTable->RuneOption_WeaponID);
			if (info != nullptr)
			{
				ItemStatOption = info->Options[0];
				Value = info->OptionValueBases[0];
			}
			break;
		}
		case EItemEquipPositionEnum::VE_Armor:
		case EItemEquipPositionEnum::VE_Pants:
		case EItemEquipPositionEnum::VE_Necklace:
		case EItemEquipPositionEnum::VE_Ring:
		{
			FItemRandomOptionTableInfo* info = UTableManager::GetInstancePtr()->GetItemRandomOptionRow(RuneTable->RuneOption_ArmorID);
			if (info != nullptr)
			{
				ItemStatOption = info->Options[0];
				Value = info->OptionValueBases[0];
			}
			break;
		}
		}
		if (ItemStatOption == EItemStatOption::Max)
			continue;

		BasicOptions[(int32)ItemStatOption] += Value;
	}
}

int32 FRuneTable::WalletKeyToRuneGrade(FName WalletKey)
{
	int32 idx = RGAMEINSTANCE(GEngine)->RInventory->RuneWallet.Find(WalletKey);
	ensure(idx != INDEX_NONE);
	return idx + 1;
}

FRuneTable* FRuneTable::LookUp(int32 RuneGrade)
{
	FRuneTable* RuneTable = UTableManager::GetInstancePtr()->GetRuneOptionRowInt(RuneGrade);
	ensure(RuneTable != nullptr);
	return RuneTable;
}

int32 FRuneTable::EquipPositionToNumSockets(EItemEquipPositionEnum EquipPosition)
{
	switch (EquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
		return 4;
	case EItemEquipPositionEnum::VE_Armor:
		return 3;
	case EItemEquipPositionEnum::VE_Pants:
		return 2;
	case EItemEquipPositionEnum::VE_Necklace:
	case EItemEquipPositionEnum::VE_Ring:
		return 0;
	default:
		ensure(false);
		return 0;
	}
}

int32 FRuneTable::EquipPositionToNumSocketsByTable(const FItemTableInfo* InTableInfo)
{
	return InTableInfo ? InTableInfo->ItemRuneSlotCount : 0;
}

TArray<FName> FRuneWordTable::GetRowRunes()
{
	TArray<FName> RowRunes;
	RowRunes.Emplace(Slot01);
	RowRunes.Emplace(Slot02);
	RowRunes.Emplace(Slot03);
	RowRunes.Emplace(Slot04);
	return RowRunes;
}

int32 FRuneWordTable::GetRowNumSockets()
{

	TArray<FName> RowRunes = GetRowRunes();

	int32 RowNumSockets = 0;
	for (; RowNumSockets < 4; RowNumSockets++)
	{
		if (RowRunes[RowNumSockets].IsNone())
			break;
	}

	return RowNumSockets;
}

FName FRuneWordTable::CheckForRuneWord(TArray<FName> Runes, int32 NumSockets)
{
	TArray< FName > RowNames = UTableManager::GetInstancePtr()->DTRuneWord->GetRowNames();
	for (FName rowName : RowNames)
	{
		FRuneWordTable* RowInfo = UTableManager::GetInstancePtr()->GetRuneWordRow(rowName);
		check(RowInfo != nullptr);


		TArray<FName> RowRunes = RowInfo->GetRowRunes();
		int32 RowNumSockets = 0;
		for (; RowNumSockets < 4; RowNumSockets++)
		{
			if (RowRunes[RowNumSockets].IsNone())
				break;
		}

		if (RowNumSockets != NumSockets)
			continue; // row not for this type of equipment

		if (Runes.Num() < NumSockets)
			continue; // can't be match

		bool IsMatch = true;
		for (int32 i = 0; i < RowNumSockets; i++)
		{
			if (RowRunes[i] != Runes[i])
			{
				IsMatch = false;
				break;
			}
		}

		if (IsMatch)
		{
			return rowName;
		}
	}

	return NAME_None;
}

FItemUpgradeCostInfo* FItemUpgradeCostInfo::LookUp(EItemGradeEnum InItemGrade, EItemEquipTypeEnum InEquipType, int32 InItemLevel)
{
	TArray<FName> RowNames = UTableManager::GetInstancePtr()->DTItemUpgradeCostTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FItemUpgradeCostInfo* Info = UTableManager::GetInstancePtr()->GetItemUpgradeCostRow(RowName);
		if (Info->ItemGrade == InItemGrade && Info->EquipType == InEquipType && Info->ItemLevel == InItemLevel)
		{
			return Info;
		}
	}

	UE_LOG(LogGamePlay, Log, TEXT("FItemUpgradeCostInfo::LookUp: failed to find row"));

	return nullptr;
}

bool FItemUpgradeCostInfo::CanUpgrade(EItemGradeEnum InCurrentGrade)
{
	switch (InCurrentGrade)
	{
	case EItemGradeEnum::Normal:
	case EItemGradeEnum::Rare:
	case EItemGradeEnum::Epic:
	case EItemGradeEnum::Unique:
		return true;
	case EItemGradeEnum::Tower:
		return false;
	default:
		check(false);
		break;
	}
	return false;
}

bool FItemUpgradeCostInfo::CanResetOption(EItemGradeEnum InCurrentGrade)
{
	return InCurrentGrade == EItemGradeEnum::Unique;
}
