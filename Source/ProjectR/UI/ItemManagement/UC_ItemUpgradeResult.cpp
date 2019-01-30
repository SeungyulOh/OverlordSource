// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemUpgradeResult.h"

#include "UC_StatChanges.h"
#include "UI/Common/UC_ItemIcon.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "UP_ItemUpgrade.h"
#include "UtilFunctionIntegrated.h"


void UUC_ItemUpgradeResult::InvalidateData()
{
	Super::InvalidateData();

	if (!ensure(!ItemUD.IsEmpty()))
		return;

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (!ensure(ItemData != nullptr))
		return;
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

	ItemIcon->RefreshEquipmentIcon(ItemUD);
	ItemIcon->Shine();

// 	FItemTableInfo* ItemInfoBefore = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataBefore.itemId);
// 	if (!ensure(ItemInfoBefore != nullptr))
// 		return;
	SizeBox_1->SetVisibility(ESlateVisibility::Collapsed);
	StatDiffs->SetVisibility(ESlateVisibility::Collapsed);

// 	UUP_ItemUpgrade::RefreshTextGrade(Text_Before, ItemInfoBefore->ItemGrade);
// 	UUP_ItemUpgrade::RefreshTextGrade(Text_After, ItemInfo->ItemGrade);

	//stat changes
	//StatDiffs->Refresh(&ItemDataBefore, ItemData);
}

void UUC_StatDiffs::Refresh(const FITEM* ItemDataBefore, const FITEM* ItemDataAfter)
{
	if (IsValid(Text_PowerValue))
	{
		int32 CombatPowerBefore = FItemTableInfo::GetCombatPower(ItemDataBefore);
		int32 CombatPowerAfter = FItemTableInfo::GetCombatPower(ItemDataAfter);
		// CombatPowerAfter BETTER be larger than CombatPowerBefore
		Text_PowerValue->SetText(FText::FromString(FString::Printf(TEXT("+%d"), CombatPowerAfter - CombatPowerBefore)));
	}

	for (UUC_StatDiff* StatDiff : BasicStatDiffs)
	{
		StatDiff->SetVisibility(ESlateVisibility::Collapsed);
		StatDiff->SetDisable(false);
	}
	for (UUC_StatDiff* StatDiff : SubstatDiffs)
	{
		StatDiff->SetVisibility(ESlateVisibility::Collapsed);
		StatDiff->SetDisable(false);
	}

	FItemTableInfo* ItemInfoBefore = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataBefore->itemId);
	if (!ensure(ItemInfoBefore != nullptr))
		return;
	FItemTableInfo* ItemInfoAfter = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataAfter->itemId);
	if (!ensure(ItemInfoAfter != nullptr))
		return;

	TArray<int32> BasicOptionsBefore;
	ItemInfoBefore->GetBasicOptions(BasicOptionsBefore);
	TArray<int32> BasicOptionsAfter;
	ItemInfoAfter->GetBasicOptions(BasicOptionsAfter);

	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfoBefore, ItemDataBefore, BasicOptionsBefore);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfoBefore, ItemDataBefore, BasicOptionsBefore);

	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfoAfter, ItemDataAfter, BasicOptionsAfter);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfoAfter, ItemDataAfter, BasicOptionsAfter);

	int32 WidgetIndex = 0;
	for (int32 i = 0; i <= (int32)EItemStatOption::MagicResistance; i++)
	{
		// Do not show stat that didn't change
		if (BasicOptionsBefore[i] == BasicOptionsAfter[i])
			continue;

		int32 DiffValue = BasicOptionsAfter[i] - BasicOptionsBefore[i];
		ensure(DiffValue > 0);

		if (!BasicStatDiffs.IsValidIndex(WidgetIndex))
			break;

		BasicStatDiffs[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);

		BasicStatDiffs[WidgetIndex]->Refresh((EItemStatOption)i, DiffValue);

		WidgetIndex++;
	}

	WidgetIndex = 0;
	for (int32 i = (int32)EItemStatOption::AttackRange; i < (int32)EItemStatOption::Max; i++)
	{
		// Do not show stat that didn't change
		if (BasicOptionsBefore[i] == BasicOptionsAfter[i])
			continue;

		int32 DiffValue = BasicOptionsAfter[i] - BasicOptionsBefore[i];
		ensure(DiffValue > 0);

		if (!SubstatDiffs.IsValidIndex(WidgetIndex))
			break;

		SubstatDiffs[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);

		SubstatDiffs[WidgetIndex]->Refresh((EItemStatOption)i, DiffValue);

		WidgetIndex++;
	}
}

void UUC_StatDiff::Refresh(EItemStatOption ItemStatOption, int32 DiffValue)
{
	Text_Name->LocalizingKey = UUIFunctionLibrary::ItemStatOptionToStringKey(ItemStatOption);
	Text_Name->ApplyLocalizedText();

	FText DiffText;
	if (FItemTableInfo::IsItemStatPerMil(ItemStatOption))
	{
		// "+10.0%"
		DiffText = FText::FromString(FString::Printf(TEXT("+%.1f%%"), (float)DiffValue / 10.0));
	}
	else
	{
		// "+15"
		DiffText = FText::FromString(FString::Printf(TEXT("+%d"), DiffValue));
	}
	Text_Value->SetText(DiffText);
}
