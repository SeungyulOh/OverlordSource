// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemEnchantResult.h"

#include "UC_StatChanges.h"
#include "UI/Common/UC_ItemIcon.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "UC_ItemUpgradeResult.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Control/RRichTextBlock.h"


void UUC_ItemEnchantResult::NativeConstruct()
{
	Super::NativeConstruct();

	Color_EnchantLvAfter = Text_EnchantLvAfter->ColorAndOpacity;
}

void UUC_ItemEnchantResult::InvalidateData()
{
	Super::InvalidateData();

	if (ItemUD.IsEmpty())
		return;

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (ItemInfo == nullptr) {
		UE_LOG(LogUI, Log, TEXT("UUP_ItemEnchantResult_NewDesign::InvalidateData returned ItemData with unknown idtemId"));
		return;
	}

	Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

	bool bFailed = ItemDataBefore.enchant == ItemData->enchant;
	bool bSpecialEnchantSuccess = (ItemData->enchant - ItemDataBefore.enchant) >= 2;

	ItemIcon->RefreshEquipmentIcon(ItemUD);
	if (bFailed)
	{
		Switcher_Result->SetActiveWidgetIndex(2);
		ItemIcon->NoShine();
	}
	else
	{
		if (bSpecialEnchantSuccess)
		{
			Switcher_Result->SetActiveWidgetIndex(1);
			ItemIcon->Flare();

			Text_DescriptionSpecialEnchant->SetRichText(DescriptionSpecialEnchant.GetText());
		}
		else
		{
			Switcher_Result->SetActiveWidgetIndex(0);
			ItemIcon->Shine();
		}
	}

	Text_EnchantLvBefore->SetText(FText::FromString(FString::Printf(TEXT("+%d"), ItemDataBefore.enchant)));
	Text_EnchantLvAfter->SetText(FText::FromString(FString::Printf(TEXT("+%d"), ItemData->enchant)));

	Text_EnchantLvAfter->SetColorAndOpacity(ItemData->enchantLocked ? Color_EnchantLevelUnenchantable : Color_EnchantLvAfter);

	// stat changes
	StatDiffs->Refresh(&ItemDataBefore, ItemData);
}

