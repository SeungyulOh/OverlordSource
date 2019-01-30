// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_StatChanges.h"

#include "Table/ItemLevelUpTableInfo.h"
#include "UC_StatChange.h"


void UUC_StatChanges::NativeConstruct()
{
	Super::NativeConstruct();
	
	// collect BP widgets
	StatChanges.Empty();
	StatChanges.Push(StatChange1);
	StatChanges.Push(StatChange2);
	StatChanges.Push(StatChange3);
	StatChanges.Push(StatChange4);
	StatChanges.Push(StatChange5);
}

void UUC_StatChanges::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_StatChanges::Refresh(const FItemTableInfo* ItemInfo, const FITEM* ItemDataBefore, const FITEM* ItemDataAfter)
{
	TArray<int32> BasicOptionsBefore;
	ItemInfo->GetBasicOptions(BasicOptionsBefore);
	TArray<int32> BasicOptionsAfter = BasicOptionsBefore;

	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemDataBefore, BasicOptionsBefore);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemDataBefore, BasicOptionsBefore);

	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemDataAfter, BasicOptionsAfter);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemDataAfter, BasicOptionsAfter);

	for (UUC_StatChange* StatChange : StatChanges)
	{
		StatChange->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 WidgetIndex = 0;
	for (int32 i = 0; i < (int32)EItemStatOption::Max; i++)
	{
		// 0 here means the stat is not of Basic Options
		if (BasicOptionsBefore[i] == 0)
			continue;

		// Do not show stat that didn't change
		if (BasicOptionsBefore[i] == BasicOptionsAfter[i])
			continue;

		if (!StatChanges.IsValidIndex(WidgetIndex))
			break;

		StatChanges[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);

		StatChanges[WidgetIndex]->Refresh((EItemStatOption)i, BasicOptionsBefore[i], BasicOptionsAfter[i]);

		WidgetIndex++;
	}

}
