// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"


#include "GlobalIntegrated.h"
#include "Global/ShopManager.h"

#include "GameInfo/RInventory.h"

#include "UtilFunctionIntegrated.h"

#include "UI/RWidgetManager.h"
#include "UI/UI_Common/UP_CommonPopup.h"

#include "UC_FloorShop_ScrollItem.h"
#include "UI/Common/UC_ItemIcon.h"


void UUC_FloorShop_ScrollItem::NativeConstruct()
{	
	Super::NativeConstruct();

	if (IsValid(Button_Buy))
	{
		Button_Buy->OnClicked.RemoveDynamic(this, &UUC_FloorShop_ScrollItem::OnClickItemBuy);
		Button_Buy->OnClicked.AddDynamic(this, &UUC_FloorShop_ScrollItem::OnClickItemBuy);
	}
}


void UUC_FloorShop_ScrollItem::NativeDestruct()
{
	Super::NativeDestruct();	
}


void UUC_FloorShop_ScrollItem::SetItemInfo(FSHOP_FLOORSHOP_SLOT InItemInfo)
{
	ItemInfo = InItemInfo;
	FITEM_ICON_INFO		ItemIconInfo;

	switch (ItemInfo.type)
	{
		case EREWARD_TYPE::ITEM:
		{
			UC_GenericItemIcon->Switcher_Icon->SetActiveWidgetIndex(0);
			FItemTableInfo*		ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemInfo.rewardId);
			checkf(ItemTableInfo != nullptr, TEXT("[%s]ItemTableInfo is Empty"), __FUNCTION__);

			if (IsValid(Text_Name))
				Text_Name->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName));
			
			UC_GenericItemIcon->EquipmentIcon->ItemIconCommon->RefreshIcon(ItemTableInfo->ItemIcon);
			UC_GenericItemIcon->EquipmentIcon->ItemIconCommon->RefreshGradeFrame(ItemTableInfo->ItemGrade);
			break;
		}
		case EREWARD_TYPE::WALLET:
		{
			UC_GenericItemIcon->Switcher_Icon->SetActiveWidgetIndex(2);
			FWalletTableInfo*	WalletTableInfo = UTableManager::GetInstancePtr()->GetWalletRow(ItemInfo.rewardId);
			checkf(WalletTableInfo != nullptr, TEXT("[%s]WalletTableInfo is Empty"), __FUNCTION__);

			if (IsValid(Text_Name))
				Text_Name->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, WalletTableInfo->CommonStringKey));
			
			UC_GenericItemIcon->MaterialIcon->ItemIconCommon->RefreshIcon(WalletTableInfo->ItemIconKey);
			UC_GenericItemIcon->MaterialIcon->Text_Amount->SetText(FText::AsNumber(ItemInfo.amt));
			break;
		}
	}
	FWalletTableInfo* currencyInfo = UTableManager::GetInstancePtr()->GetWalletRow(FName(*ItemInfo.currencyType));
	
	switch (ItemInfo.state)
	{
	case ESHOP_FLOORSHOP_SLOT_STATE::SOLDOUT:
	{
		WidgetSwitcher_Visible->SetActiveWidgetIndex(2);
		LOCK->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case ESHOP_FLOORSHOP_SLOT_STATE::DISABLED_FLOOR:
	{
		WidgetSwitcher_Visible->SetActiveWidgetIndex(1);
		LOCK->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_Title->SetActiveWidgetIndex(0);

		Text_Currency_Disable->SetText(FText::AsNumber(ItemInfo.price));
		UUIFunctionLibrary::SetImageBrushFromTable(currencyInfo->ItemIconKey, Image_Currency_Disable);
	}
	break;
	case ESHOP_FLOORSHOP_SLOT_STATE::DISABLED_OPEN:
	{
		WidgetSwitcher_Visible->SetActiveWidgetIndex(1);
		LOCK->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_Title->SetActiveWidgetIndex(1);

		Text_Currency_Disable->SetText(FText::AsNumber(ItemInfo.openGem));
		UUIFunctionLibrary::SetImageBrushFromTable(currencyInfo->ItemIconKey, Image_Currency_Disable);
	}
	break;
	case ESHOP_FLOORSHOP_SLOT_STATE::ENABLED_BUY:
	{
		WidgetSwitcher_Visible->SetActiveWidgetIndex(0);
		LOCK->SetVisibility(ESlateVisibility::Collapsed);
		WidgetSwitcher_Title->SetActiveWidgetIndex(0);

		Text_Currency_Enable->SetText(FText::AsNumber(ItemInfo.price));
		UUIFunctionLibrary::SetImageBrushFromTable(currencyInfo->ItemIconKey, Image_Currency_Enable);
	}
	break;
	case ESHOP_FLOORSHOP_SLOT_STATE::ENABLED_OPEN:
	{
		WidgetSwitcher_Visible->SetActiveWidgetIndex(0);
		LOCK->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_Title->SetActiveWidgetIndex(1);

		Text_Currency_Enable->SetText(FText::AsNumber(ItemInfo.openGem));
		UUIFunctionLibrary::SetImageBrushFromTable(TEXT("PROPERTY_GEM"), Image_Currency_Enable);
	}
	break;
	default:
		break;
	}
}

// void UUC_FloorShop_ScrollItem::Lock(int32 InLevel)
// {
// 	WidgetSwitcher_Visible->SetActiveWidgetIndex(1);
// 	if (IsValid(LOCK))
// 		LOCK->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// // 
// // 	if (IsValid(Text_Count))
// // 		Text_Count->SetText(FText::Format(FText::FromString("Lv.{0}"), FText::AsNumber(InLevel)));
// }

void UUC_FloorShop_ScrollItem::OnClickItemBuy()
{
	RGAMEINSTANCE(this)->ShopManager->BuyItemDelegate.Broadcast(ItemInfo);
}

