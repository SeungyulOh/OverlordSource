// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroItemOptions_ScrollItem.h"

#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketDataStructures.h"

#include "Table/ItemTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "HeroItemOptions_ScrollItemData.h"

void UUC_HeroItemOptions_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	// collect BP widgets into arrays

	ItemOptionPanels.Empty();
	ItemOptionPanels.Push(Panel_ItemOption1);
	ItemOptionPanels.Push(Panel_ItemOption2);
	ItemOptionPanels.Push(Panel_ItemOption3);
	ItemOptionPanels.Push(Panel_ItemOption4);
	ItemOptionPanels.Push(Panel_ItemOption5);
	ItemOptionPanels.Push(Panel_ItemOption6);
	ItemOptionPanels.Push(Panel_ItemOption7);
	ItemOptionPanels.Push(Panel_ItemOption8);

	ItemOptionLabels.Empty();
	ItemOptionLabels.Push(Text_ItemOptionLabel1);
	ItemOptionLabels.Push(Text_ItemOptionLabel2);
	ItemOptionLabels.Push(Text_ItemOptionLabel3);
	ItemOptionLabels.Push(Text_ItemOptionLabel4);
	ItemOptionLabels.Push(Text_ItemOptionLabel5);
	ItemOptionLabels.Push(Text_ItemOptionLabel6);
	ItemOptionLabels.Push(Text_ItemOptionLabel7);
	ItemOptionLabels.Push(Text_ItemOptionLabel8);

	//ItemOptionPluses.Empty();
	//ItemOptionPluses.Push(Text_ItemOptionPlus1);
	//ItemOptionPluses.Push(Text_ItemOptionPlus2);
	//ItemOptionPluses.Push(Text_ItemOptionPlus3);
	//ItemOptionPluses.Push(Text_ItemOptionPlus4);

	ItemOptionValues.Empty();
	ItemOptionValues.Push(Text_ItemOptionValue1);
	ItemOptionValues.Push(Text_ItemOptionValue2);
	ItemOptionValues.Push(Text_ItemOptionValue3);
	ItemOptionValues.Push(Text_ItemOptionValue4);
	ItemOptionValues.Push(Text_ItemOptionValue5);
	ItemOptionValues.Push(Text_ItemOptionValue6);
	ItemOptionValues.Push(Text_ItemOptionValue7);
	ItemOptionValues.Push(Text_ItemOptionValue8);
}

void UUC_HeroItemOptions_ScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HeroItemOptions_ScrollItem::InvalidateData()
{
	//Super::InvalidateData();

	auto ItemOptionsData = Cast<UHeroItemOptions_ScrollItemData>(ItemData);
	if (!IsValid(ItemOptionsData))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroItemOptions_ScrollItem::InvalidateData: failed to get HeroItemOptions_ScrollItemData"));
		return;
	}

	FString ItemUD = ItemOptionsData->ItemUD;

	if (ItemUD.IsEmpty())
		return;

	// Set Random options
	
	FITEM* _ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (_ItemData == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroItemOptions_ScrollItem::InvalidateData: failed to get ItemData"));
		return;
	}
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(_ItemData->itemId);
	if (_ItemData == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroItemOptions_ScrollItem::InvalidateData: failed to get ItemInfo"));
		return;
	}

	TArray<int32> AdditionalOptions;
	ItemInfo->GetAdditionalOptions(_ItemData, AdditionalOptions);
	FRuneTable::ApplyRunes(_ItemData, AdditionalOptions);

	for (int32 i = 0; i < ItemOptionPanels.Num(); i++)
	{
		ItemOptionPanels[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 WidgetIndex = 0;
	for (int32 i = 0; i < (int32)EItemStatOption::Max; i++)
	{
		if (AdditionalOptions[i] == 0)
			continue;

		RefreshRandomOptionUI((EItemStatOption)i, AdditionalOptions[i], WidgetIndex);

		WidgetIndex++;
	}
}

void UUC_HeroItemOptions_ScrollItem::RefreshRandomOptionUI(EItemStatOption ItemStatOption, int32 Value, int32 WidgetIndex)
{
	ItemOptionPanels[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);

	// Refresh label
	FName ItemStatOptionStringKey = UUIFunctionLibrary::ItemStatOptionToStringKey(ItemStatOption);
	FText ItemStatOptionText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemStatOptionStringKey);
	ItemOptionLabels[WidgetIndex]->SetText(ItemStatOptionText);

	// Refresh value
	FText ValueText = UUtilFunctionLibrary::ItemAdditionalOptionStatValueToText(ItemStatOption, Value);
	ItemOptionValues[WidgetIndex]->SetText(ValueText);
}
