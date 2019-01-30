// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemEnchantResult_NewDesign.h"

#include "UC_StatChanges.h"
#include "UI/Common/UC_ItemIcon.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"


void UUP_ItemEnchantResult_NewDesign::NativeConstruct()
{
	Super::NativeConstruct();

	Button_OK->OnClicked.AddDynamic(this, &UUP_ItemEnchantResult_NewDesign::OnButtonOKClicked);
}

void UUP_ItemEnchantResult_NewDesign::NativeDestruct()
{
	Button_OK->OnClicked.RemoveDynamic(this, &UUP_ItemEnchantResult_NewDesign::OnButtonOKClicked);

	Super::NativeDestruct();
}

void UUP_ItemEnchantResult_NewDesign::InvalidateData()
{
	Super::InvalidateData();

	if (ItemUD.IsEmpty())
		return;

	ItemIcon->RefreshEquipmentIcon(ItemUD);

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (ItemInfo == nullptr) {
		UE_LOG(LogUI, Log, TEXT("UUP_ItemEnchantResult_NewDesign::InvalidateData returned ItemData with unknown idtemId"));
		return;
	}

	Text_EnchantLvBefore->SetText(FText::AsNumber(ItemDataBefore.enchant));
	Text_EnchantLvAfter->SetText(FText::AsNumber(ItemData->enchant));

	// tempy code: Remove flares if enchant failed
	bool bFailed = ItemDataBefore.enchant == ItemData->enchant;
	for (UWidget* Widget : FlareWidgets)
	{
		Widget->SetVisibility(bFailed ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	// stat changes
	//StatChanges->Refresh(ItemInfo, &ItemDataBefore, ItemData);
}

void UUP_ItemEnchantResult_NewDesign::OnButtonOKClicked()
{
	OnComplete.Broadcast();
}
