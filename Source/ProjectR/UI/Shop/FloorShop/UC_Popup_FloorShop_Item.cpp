// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"

#include "Global/ShopManager.h"

#include "GameInfo/RInventory.h"

#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"

#include "Table/CurrencyPropertyTableInfo.h"
#include "Table/ItemTableInfo.h"
#include "Table/ShopProductTableInfo.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UC_Popup_FloorShop_Item.h"
#include "UI/Common/UC_ItemIcon.h"


void UUC_Popup_FloorShop_Item::NativeConstruct()
{	
	Super::NativeConstruct();
	Init();
}


void UUC_Popup_FloorShop_Item::NativeDestruct()
{
	Super::NativeDestruct();	
}

void UUC_Popup_FloorShop_Item::Init()
{
	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Popup_FloorShop_Item::OnClickCancel);
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_FloorShop_Item::OnClickCancel);
	}
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_FloorShop_Item::OnClickCancel);
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_FloorShop_Item::OnClickCancel);
	}
	if (IsValid(Button_Ok))
	{
		Button_Ok->OnClicked.RemoveDynamic(this, &UUC_Popup_FloorShop_Item::OnClickOK);
		Button_Ok->OnClicked.AddDynamic(this, &UUC_Popup_FloorShop_Item::OnClickOK);
	}
}

void UUC_Popup_FloorShop_Item::SetItemInfo(FSHOP_FLOORSHOP_SLOT InItemInfo)
{
	ItemInfo = InItemInfo;
	FITEM_ICON_INFO	ItemIconInfo;

	switch (ItemInfo.type)
	{
	case EREWARD_TYPE::ITEM:
	{
		UC_GenericItemIcon->Switcher_Icon->SetActiveWidgetIndex(0);
		FItemTableInfo*		ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemInfo.rewardId);
		checkf(ItemTableInfo != nullptr, TEXT("[%s]ItemTableInfo is Empty"), __FUNCTION__);

		if (IsValid(Text_ItemName))
			Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName));

		UC_GenericItemIcon->EquipmentIcon->ItemIconCommon->RefreshIcon(ItemTableInfo->ItemIcon);
		UC_GenericItemIcon->EquipmentIcon->ItemIconCommon->RefreshGradeFrame(ItemTableInfo->ItemGrade);
		break;
	}
	case EREWARD_TYPE::WALLET:
	{
		UC_GenericItemIcon->Switcher_Icon->SetActiveWidgetIndex(2);
		FWalletTableInfo*	WalletTableInfo = UTableManager::GetInstancePtr()->GetWalletRow(ItemInfo.rewardId);
		checkf(WalletTableInfo != nullptr, TEXT("[%s]WalletTableInfo is Empty"), __FUNCTION__);

		if (IsValid(Text_ItemName))
			Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, WalletTableInfo->CommonStringKey));

		UC_GenericItemIcon->MaterialIcon->ItemIconCommon->RefreshIcon(WalletTableInfo->ItemIconKey);
		UC_GenericItemIcon->MaterialIcon->Text_Amount->SetText(FText::AsNumber(ItemInfo.amt));
		break;
	}
	}

	if (ItemInfo.state == ESHOP_FLOORSHOP_SLOT_STATE::ENABLED_BUY)
	{
		FWalletTableInfo* currencyInfo = UTableManager::GetInstancePtr()->GetWalletRow(FName(*ItemInfo.currencyType));
		UUIFunctionLibrary::SetImageBrushFromTable(currencyInfo->ItemIconKey, Image_Currency);
		Text_Currency->SetText(FText::AsNumber(ItemInfo.price));
		UC_GenericItemIcon->Lock->SetVisibility(ESlateVisibility::Collapsed);
		WidgetSwitcher_ContentsText->SetActiveWidgetIndex(0);
	}
	else if (ItemInfo.state == ESHOP_FLOORSHOP_SLOT_STATE::ENABLED_OPEN)
	{
		UUIFunctionLibrary::SetImageBrushFromTable(TEXT("PROPERTY_GEM"), Image_Currency);
		Text_Currency->SetText(FText::AsNumber(ItemInfo.openGem));
		UC_GenericItemIcon->Lock->SetVisibility(ESlateVisibility::Visible);
		WidgetSwitcher_ContentsText->SetActiveWidgetIndex(1);
	}
}

void UUC_Popup_FloorShop_Item::OnClickCancel()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Popup_FloorShop_Item::OnClickOK()
{
	this->SetVisibility(ESlateVisibility::Collapsed);

	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	URInventory*		RInventory		=	GameInstance->RInventory;
	UShopManager*		ShopManager		=	GameInstance->ShopManager;
	 
	checkf(RInventory != nullptr, TEXT("UUC_FloorShop_ScrollItem::OnClickItemBuy[RInventory is Empty]"));
	checkf(ShopManager != nullptr, TEXT("UUC_FloorShop_ScrollItem::OnClickItemBuy[ShopManager is Empty]"));
	 	
	int32 Currentcy;
	UUP_CommonPopupSplash* CommonPopup;
	if (ItemInfo.state == ESHOP_FLOORSHOP_SLOT_STATE::ENABLED_BUY)
	{
		Currentcy = RInventory->GetCurrency(FName(*ItemInfo.currencyType));
		if (Currentcy >= ItemInfo.price)
	 		ShopManager->RequestFloorShopItemBuy(ItemInfo.slotId);
		else
			CommonPopup = UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_OptionValueShortage")));
	}
	else if (ItemInfo.state == ESHOP_FLOORSHOP_SLOT_STATE::ENABLED_OPEN)
	{
		Currentcy = RInventory->GetCurrency(WALLET_GEM);
		if (Currentcy >= ItemInfo.price)
			ShopManager->RequestFloorShopSlotOpen(ItemInfo.slotId);
		else
			CommonPopup = UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_OptionValueShortage")));
	}
}

