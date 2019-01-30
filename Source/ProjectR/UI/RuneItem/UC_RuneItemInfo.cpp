// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectR.h"


#include "Global/TableManager.h"
#include "Table/ItemTableInfo.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"

#include "UC_RuneItemInfo.h"

void UUC_RuneItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	const int32 RuneItemNumber = 5;
	const int32 MagicNumber = RuneItemNumber - 1;
	BasicStatLabelTextBlocks.Empty(MagicNumber);
	BasicStatValueTextBlocks.Empty(MagicNumber);
	SubstatLabelTextBlocks.Empty(MagicNumber);
	SubstatValueTextBlocks.Empty(MagicNumber);

	for (int32 i = 1; i < RuneItemNumber; i++)
	{
		UWidget*		StatText			=	BaseOptionInfoText->GetChildAt(i);
		UTextBlock*		TextWidget			=	Cast<UTextBlock>(StatText);
		BasicStatLabelTextBlocks.Push(TextWidget);

		UWidget*		StatTextValue		=	BaseOptionInfo->GetChildAt(i);
		UTextBlock*		TextValueWidget		=	Cast<UTextBlock>(StatTextValue);
		BasicStatValueTextBlocks.Push(TextValueWidget);

		UWidget*		SubStatText			=	AddOptionInfoText->GetChildAt(i-1);
		UTextBlock*		SubTextWidget		=	Cast<UTextBlock>(SubStatText);
		SubstatLabelTextBlocks.Push(SubTextWidget);

		UWidget*		SubStatTextValue	=	AddOptionInfo->GetChildAt(i-1);
		UTextBlock*		SubTextValueWidget	=	Cast<UTextBlock>(SubStatTextValue);
		SubstatValueTextBlocks.Push(SubTextValueWidget);
	}

}

void UUC_RuneItemInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_RuneItemInfo::SetInValidData(FString InCurrentItemUD)
{
	CurrentItemUD = InCurrentItemUD;

	SetItemData();
}

void UUC_RuneItemInfo::SetItemData()
{
	if (CurrentItemUD.IsEmpty() == true)
		return;



	FITEM*				ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CurrentItemUD);

	if (ItemData == nullptr)
		return;

	FItemTableInfo*		ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);

	if (ItemInfo == nullptr)
		return;

	int32 ItemPowerValue = RGAMEINSTANCE(this)->RInventory->GetItemValue(ItemData->itemId, ItemData->enchant, ItemData->level);
	UWidget*		StatTextValue = BaseOptionInfo->GetChildAt(0);
	UTextBlock*		TextValueWidget = Cast<UTextBlock>(StatTextValue);
	TextValueWidget->SetText(FText::AsNumber(ItemPowerValue));

	RuneIcon->RefreshEquipmentIcon(CurrentItemUD);
	
	// Refresh Basic Options
	
	TArray<int32> BasicOptions;
	ItemInfo->GetBasicOptions(BasicOptions);
	
	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemData, BasicOptions);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemData, BasicOptions);
	FRuneTable::ApplyRunes(ItemData, BasicOptions);
	
	InvalidateBasicStats(BasicOptions);
	InvalidateSubstats(BasicOptions);

}

void UUC_RuneItemInfo::InvalidateBasicStats(const TArray<int32>& BasicOptions)
{
	for (UTextBlock* TextBlock : BasicStatLabelTextBlocks)
	{
		TextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	for (UTextBlock* TextBlock : BasicStatValueTextBlocks)
	{
		TextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 WidgetIndex = 0;
	int32 Max = (int32)EItemStatOption::MagicResistance;
	for (int32 i = 0; i <= Max; i++)
	{
		if (BasicOptions[i] == 0)
			continue;

		RefreshStatUI((EItemStatOption)i, BasicOptions[i], BasicStatLabelTextBlocks, BasicStatValueTextBlocks, WidgetIndex);

		WidgetIndex++;
	}
}

void UUC_RuneItemInfo::InvalidateSubstats(const TArray<int32>& BasicOptions)
{
	for (UTextBlock* TextBlock : SubstatLabelTextBlocks)
	{
		TextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	for (UTextBlock* TextBlock : SubstatValueTextBlocks)
	{
		TextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 WidgetIndex = 0;
	for (int32 i = (int32)EItemStatOption::AttackRange; i < (int32)EItemStatOption::Max; i++)
	{
		if (BasicOptions[i] == 0)
			continue;

		RefreshStatUI((EItemStatOption)i, BasicOptions[i], SubstatLabelTextBlocks, SubstatValueTextBlocks, WidgetIndex);

		WidgetIndex++;
	}
}

void UUC_RuneItemInfo::RefreshStatUI(EItemStatOption ItemStatOption, int32 Value, TArray<UTextBlock*> LabelTextBlocks, TArray<UTextBlock*> ValueTextBlocks, int32 WidgetIndex)
{
	if (!LabelTextBlocks.IsValidIndex(WidgetIndex))
	{
		return;
	}

	// label
	FName StringKey = UUIFunctionLibrary::ItemStatOptionToStringKey(ItemStatOption);
	FText StatNameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, StringKey);

	LabelTextBlocks[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);
	LabelTextBlocks[WidgetIndex]->SetText(StatNameText);

	// value
	ValueTextBlocks[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);

	ValueTextBlocks[WidgetIndex]->SetText(UUtilFunctionLibrary::ItemBasicOptionStatValueToText(ItemStatOption, Value));
}