// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemLevelUpResult.h"

#include "UC_StatChanges.h"
#include "UI/Common/UC_ItemIcon.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "UC_ItemUpgradeResult.h"
#include "UtilFunctionIntegrated.h"


void UUC_ItemLevelUpResult::InvalidateData()
{
	Super::InvalidateData();

	check(!ItemUD.IsEmpty());

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (!ensure(ItemData != nullptr))
		return;
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	Text_Lucky->SetVisibility(IsLucky ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	ItemIcon->RefreshEquipmentIcon(ItemUD);

	Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

	Text_LvBefore->SetText(FText::FromString(FString::Printf(TEXT("+%d"), ItemDataBefore.level)));
	Text_LvAfter->SetText(FText::FromString(FString::Printf(TEXT("+%d"), ItemData->level)));

	// stat changes
	StatDiffs->Refresh(&ItemDataBefore, ItemData);
}

