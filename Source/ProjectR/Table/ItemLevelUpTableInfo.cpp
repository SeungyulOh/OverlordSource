// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ItemLevelUpTableInfo.h"



FItemLevelUpTableInfo* FItemLevelUpTableInfo::LookUp(EItemEquipTypeEnum EquipType, EItemGradeEnum Grade, int32 Level)
{
	TArray<FName> RowNames = UTableManager::GetInstancePtr()->DTItemLevelUpTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FItemLevelUpTableInfo* Info = UTableManager::GetInstancePtr()->GetItemLevelUpTableRow(RowName);
		if (Info->EquipType == EquipType && Info->Grade == Grade && Info->Level == Level)
		{
			return Info;
		}
	}

	UE_LOG(LogGamePlay, Log, TEXT("FItemLevelUpTableInfo::LookUp: failed to find row"));

	return nullptr;
}

void FItemLevelUpTableInfo::ApplyItemLevel(const FItemTableInfo* ItemInfo, const FITEM* ItemData, TArray<int32>& BasicOptions)
{
	if (ItemInfo->ItemGrade == EItemGradeEnum::Tower)
	{
		FItemLevelUpTableInfo *ItemLevelUpInfo = FItemLevelUpTableInfo::LookUp(ItemInfo->EquipType, ItemInfo->ItemGrade, ItemData->level);
		if (ItemLevelUpInfo == nullptr)
			return;

		BasicOptions[(int32)EItemStatOption::PhysicalAttackPower] += ItemLevelUpInfo->PlusPhysicalAttackPower;
		BasicOptions[(int32)EItemStatOption::MagicAttackPower] += ItemLevelUpInfo->PlusMagicAttackPower;
		BasicOptions[(int32)EItemStatOption::Armor] += ItemLevelUpInfo->PlusArmor;
		BasicOptions[(int32)EItemStatOption::MagicResistance] += ItemLevelUpInfo->PlusMagicResistance;
	}
}
