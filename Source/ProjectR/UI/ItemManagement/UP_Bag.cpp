// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Bag.h"

#include "UI/Control/RScrollView.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "Table/ItemTableInfo.h"

#include "Table/CharacterTableInfo.h"
#include "UI/Control/RScrollItem.h"
#include "UC_GenericPopup.h"
#include "UI/RWidgetManager.h"
#include "UI/Component/CheckBox_Radio.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroItemInfo.h"

#include "UI/UI_Common/UP_RewardPopup.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/UIDefs.h"
#include "UtilFunctionIntegrated.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketTypeEnum.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UI/Common/ItemInventory_ScrollData.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/HorizontalBox_RadioGroup.h"
#include "UI/ItemManagement/UP_GenericSplash.h"
#include "UI/ItemManagement/UC_RewardsSplash.h"
#include "UC_AmountSelectSlider.h"


void UUP_Bag::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_BatchSelect))
	{
		Button_BatchSelect->OnClicked.AddDynamic(this, &UUP_Bag::OnButtonAllSelect);
		BatchSelectBtnControl.AddDynamic(this, &UUP_Bag::BatchButtonControl);
	}
	if (IsValid(Button_Select))
		Button_Select->OnClicked.AddDynamic(this, &UUP_Bag::OnButtonSelect);

	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.AddDynamic(this, &UUP_Bag::OnButtonBatchCancelClicked);
	if (IsValid(Button_Sell))
		Button_Sell->OnClicked.AddDynamic(this, &UUP_Bag::OnButtonBatchSellClicked);
	if (IsValid(Button_Grind))
		Button_Grind->OnClicked.AddDynamic(this, &UUP_Bag::OnButtonBatchGrindClicked);
	if (IsValid(Button_Lock))
		Button_Lock->OnClicked.AddDynamic(this, &UUP_Bag::OnButtonBatchLockClicked);

	if (IsValid(Button_BagExtend))
		Button_BagExtend->OnClicked.AddDynamic(this, &UUP_Bag::OnButtonBagExtendClicked);

	if(IsValid(UC_Popup_Item_Filter_Total))
		UC_Popup_Item_Filter_Total->CloseFilter.AddDynamic(this, &UUP_Bag::BatchButtonControl);

	Color_BagCapacityCurrentCountNormal = Text_BagCapacityCurrentCount->ColorAndOpacity;

	RadioGroup_Tabs->SetActiveRadioIndex(0, true);
 }

void UUP_Bag::NativeDestruct()
{
	if (IsValid(Button_Select))
	{
		Button_Select->OnClicked.RemoveAll(this);
		BatchSelectBtnControl.RemoveAll(this);
	}
	if (IsValid(Button_BatchLock))
		Button_BatchLock->OnClicked.RemoveAll(this);
	if (IsValid(Button_BatchSelect))
		Button_BatchSelect->OnClicked.RemoveAll(this);

	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_Sell))
		Button_Sell->OnClicked.RemoveAll(this);
	if (IsValid(Button_Grind))
		Button_Grind->OnClicked.RemoveAll(this);
	if (IsValid(Button_Lock))
		Button_Lock->OnClicked.RemoveAll(this);

	if (IsValid(Button_BagExtend))
		Button_BagExtend->OnClicked.RemoveAll(this);

	if (IsValid(UC_Popup_Item_Filter_Total))
		UC_Popup_Item_Filter_Total->CloseFilter.RemoveDynamic(this, &UUP_Bag::BatchButtonControl);

	Super::NativeDestruct();
}

void UUP_Bag::InvalidateData()
{
	Super::InvalidateData();
	ResetVars();
	InvalidateItemInventoryScrollView();

	switch (CurrentTab)
	{
		case EItemInventoryEnum::Weapons:
		case EItemInventoryEnum::Defense:
		case EItemInventoryEnum::Accessories:
		{
			UC_Popup_Item_Filter_Total->CheckBoxReset((int32)CurrentTab);
			Button_BatchLock->SetVisibility(ESlateVisibility::Visible);
			Button_BatchSelect->SetVisibility(ESlateVisibility::Visible);
			//Button_Sell->SetVisibility(ESlateVisibility::Visible);
			Button_Grind->SetVisibility(ESlateVisibility::Visible);
			Button_Lock->SetVisibility(ESlateVisibility::Visible);
			UC_Popup_ItemSorting->Button_AttackPower->SetVisibility(ESlateVisibility::Visible);
			UC_Popup_ItemSorting->Button_GettingOrder->SetVisibility(ESlateVisibility::Visible);
			Switcher_Sell->SetActiveWidgetIndex(0);
			break;
		}
		case EItemInventoryEnum::HeroPieces:
		case EItemInventoryEnum::Runes:
		case EItemInventoryEnum::Misc:
		case EItemInventoryEnum::Costume:
		{
			Button_BatchLock->SetVisibility(ESlateVisibility::Collapsed);
			Button_BatchSelect->SetVisibility(ESlateVisibility::Collapsed);
			//Button_Sell->SetVisibility(ESlateVisibility::Collapsed);
			Button_Grind->SetVisibility(ESlateVisibility::Collapsed);
			Button_Lock->SetVisibility(ESlateVisibility::Collapsed);
			Switcher_Sell->SetActiveWidgetIndex(1);
			UC_Popup_ItemSorting->Button_AttackPower->SetVisibility(ESlateVisibility::Collapsed);
			UC_Popup_ItemSorting->Button_GettingOrder->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
		default:
		{
			check(false);
			break;
		}
	}
	FilterVisibility(false);
	InvalidateBagCapacity();
}

void UUP_Bag::InvalidateButtonExecute()
{
	Button_Sell->SetIsEnabled(GetNumberOfSelectedItems() != 0);
}

void UUP_Bag::OnSelectionChanged()
{
	Super::OnSelectionChanged();

	//InvalidateButtonExecute();
}

void UUP_Bag::OnButtonSelect()
{
	CurrentBatchOperation = EItemInventoryBatchOperation::Sell;
	EnterBatchMode();
	InventoryScrollView->MultiSelect = IsInMultiSelectMode;
	InvalidateData();
}

void UUP_Bag::OnButtonAllSelect()
{
	OnButtonSelect();
	OnButtonBagAllSelectClicked();
}

void UUP_Bag::OnButtonSellClicked()
{
	EnterSellMode();

	InvalidateData();
}

void UUP_Bag::BatchButtonControl(bool enabled)
{
	Button_BatchSelect->SetIsEnabled(enabled);
	Button_BagExtend->SetIsEnabled(enabled);
	Button_Sell->SetIsEnabled(enabled);
	Button_Grind->SetIsEnabled(enabled);
}

void UUP_Bag::OnButtonBatchCancelClicked()
{
	if (IsInMultiSelectMode)
		ExitBatchMode();
	else if (InWalletSellMode)
		ExitSellMode();

	InventoryScrollView->MultiSelect = IsInMultiSelectMode;

	InvalidateData();
}

void UUP_Bag::OnButtonBatchSellClicked()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager	=	GameInstance->RWidgetManager;

	if (SelectedItemUDs.Num() == 0 && SelectedWalletKeys.Num() == 0)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message01"))));
		return;
	}
	if (CurrentTab == EItemInventoryEnum::Weapons || CurrentTab == EItemInventoryEnum::Defense || CurrentTab == EItemInventoryEnum::Accessories)
	{
		for (const FString& ItemUD : SelectedItemUDs)
		{
			FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);

			if(ItemData == nullptr)
				continue;

			if (ItemData->locked || ItemData->equipedHeroUD.IsEmpty() == false)
			{
				UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Sell_Impossible_SplashPopUp"))));
				return;
			}
		}
	}
	/*else if (CurrentTab == EItemInventoryEnum::Runes || CurrentTab == EItemInventoryEnum::Misc)
	{
		for (FName walletKey : SelectedWalletKeys)
		{
		}
	}*/
	
	CurrentBatchOperation = EItemInventoryBatchOperation::Sell;
	OnButtonBatchOperation();
}

void UUP_Bag::OnButtonBatchGrindClicked()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = GameInstance->RWidgetManager;

	if (SelectedItemUDs.Num() == 0)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message01"))));
		return;
	}
	for (const FString& ItemUD : SelectedItemUDs)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);

		if (ItemData == nullptr)
			continue;

		if (ItemData->locked || ItemData->equipedHeroUD.IsEmpty() == false)
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Grind_Impossible_SplashPopUp"))));
			return;
		}
	}

	CurrentBatchOperation = EItemInventoryBatchOperation::Grind;
	OnButtonBatchOperation();
}

void UUP_Bag::OnButtonBatchLockClicked()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = GameInstance->RWidgetManager;

	if (SelectedItemUDs.Num() == 0)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message01"))));
		return;
	}
	CurrentBatchOperation = EItemInventoryBatchOperation::Lock;
	OnButtonBatchOperation();
}

void UUP_Bag::OnButtonBatchOperation()
{
	switch (CurrentBatchOperation)
	{
		case EItemInventoryBatchOperation::Lock:
		{
			if (SelectedItemUDs.Num() == 0)
				return;
			TArray<FString> ItemUDsToLock;
			TArray<FString> ItemUDsToUnlock;
			for (const FString& SelectedItemUD : SelectedItemUDs)
			{
				FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(SelectedItemUD);
				check(ItemData != nullptr);
				if (!ItemData->locked)
					ItemUDsToLock.Emplace(SelectedItemUD);
				else
					ItemUDsToUnlock.Emplace(SelectedItemUD);
			}

			RGAMEINSTANCE(this)->HttpClient->OnInventoryUpdate.AddUObject(this, &UUP_Bag::OnInventoryItemHeroLockRp);
			Popup_HeroInfo_Lock->OnItemInventoryUpdated.BindUObject(this, &UUP_Bag::OnItemInventoryUpdated, (URScrollItem*)nullptr, Cast<UObject>(Popup_HeroInfo_Lock));
			Popup_HeroInfo_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Popup_HeroInfo_Lock->SetDataList(ItemUDsToLock, ItemUDsToUnlock, true);
			//UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ(ItemUDsToLock, ItemUDsToUnlock, {}, {});

			// exit batch mode here
			ExitBatchMode();
		}
		break;
		case EItemInventoryBatchOperation::Grind:
		{
			OpenGrindConfirmPopup();
		}
		break;
		case EItemInventoryBatchOperation::Sell:
		{
			OpenSellConfirmPopup();
		}
		break;
		default:
			check(false);
			break;
	}
}
// 
// void UUC_ItemGrindConfirm::OnButtonOKClicked()
// {
// 	check(SelectedItemUDs.Num() != 0);
// 
// 	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemGrind.AddUObject(this, &UUC_ItemGrindConfirm::OnInventoryGrindRp);
// 
// 	UPacketFunctionLibrary::INVENTORY_ITEM_GRIND_RQ(SelectedItemUDs);
// }

// void UUC_ItemGrindConfirm::OnInventoryGrindRp(bool bSuccess, TArray<FREWARD> Rewards)
// {
// 	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemGrind.RemoveAll(this);
// 
// 	if (bSuccess)
// 	{
// 		UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_RewardsSplash);
// 		check(Splash);
// 		UUC_RewardsSplash* RewardsSplash = Cast<UUC_RewardsSplash>(Splash->GetContentWidget());
// 		check(RewardsSplash);
// 
// 		RewardsSplash->RefreshRewards(Rewards, EItemInventoryRewardOrigin::Grind);
// 
// 		OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
// 	}
// 	else
// 	{
// 		ensure(false);
// 	}
// }

// void UUC_ItemSellConfirm::OnInventorySellRp(bool bSuccess, TArray<FREWARD> Rewards)
// {
// 	RGAMEINSTANCE(this)->HttpClient->OnInventorySell.RemoveAll(this);
// 
// 	if (bSuccess)
// 	{
// 		UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_RewardsSplash);
// 		check(Splash);
// 		UUC_RewardsSplash* RewardsSplash = Cast<UUC_RewardsSplash>(Splash->GetContentWidget());
// 		check(RewardsSplash);
// 
// 		RewardsSplash->RefreshRewards(Rewards, EItemInventoryRewardOrigin::Sell);
// 		
// 		OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
// 	}
// 	else
// 	{
// 		ensure(false);
// 	}
// }

void UUP_Bag::OnInventoryItemHeroLockRp(bool bSuccess)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryUpdate.RemoveAll(this);

	if (bSuccess)
	{
		bInventoryUpdated = true;
		InvalidateItemInventoryScrollView();
		ExitBatchMode();
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UIpage_HeroInventory_LockandReleaseResult"))), FText());
	}
	else
	{

	}
}

void UUP_Bag::OnButtonBagExtendClicked()
{
	if (RGAMEINSTANCE(this)->RInventory->CanBagBeExtendedFurther())
	{
// 		UUC_GenericPopup* Popup = UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_BagExtend);
// 		auto PopupContent = Cast<UUC_BagExtend>(Popup->GetContentWidget());
// 
		if(UC_Popup_Bag_Extend->GetVisibility() == ESlateVisibility::Collapsed)
			UC_Popup_Bag_Extend->SetVisibility(ESlateVisibility::Visible);
		else
			UC_Popup_Bag_Extend->SetVisibility(ESlateVisibility::Collapsed);

		UC_Popup_Bag_Extend->OnItemInventoryUpdated.BindUObject(this, &UUP_Bag::OnItemInventoryUpdated, (URScrollItem*)nullptr, Cast<UObject>(UC_Popup_Bag_Extend));
		UC_Popup_Bag_Extend->InvalidateData();
	}
	else
	{
		UUP_CommonPopup* common = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_InventoryExpansion_PopUp_Message05")),
			ECommonPopupType::VE_OK);
	}
}

void UUP_Bag::OnButtonBagAllSelectClicked()
{
	for (auto ScrollItem : InventoryScrollView->ScrollItemList)
	{
		if (ScrollItem == nullptr)
			continue;

		//only for equipable item function
		auto ScrollData = Cast<UInventoryScrollDataBase>(ScrollItem->ItemData);
		if (ScrollData == nullptr)
			continue;

		if (!ScrollData->bDisabled)
		{
			// Toggle the item select
			if (IsInMultiSelectMode == false)
				ScrollData->IsSelected = IsInMultiSelectMode;
			else
				ScrollData->IsSelected = IsInMultiSelectMode;

			ScrollItem->InvalidateData();
			OnSelectionChanged();
		}
	}
	TArray<URScrollItemData*>& scrollDataList = InventoryScrollView->ItemDataList;
	TArray<int32>& SelectedList = InventoryScrollView->GetSelectedScrollItem();
	for (int32 i = 0; i < scrollDataList.Num(); i++)
	{
		UInventoryScrollDataBase* itemData = Cast<UInventoryScrollDataBase>(scrollDataList[i]);
		if (!itemData->bDisabled)
		{
			SelectedList.Emplace(i);
			SelectedItemUDs.AddUnique(itemData->ItemUD);
		}
	}
	InventoryScrollView->InvalidateDataWithoutResetSelect();
}

void UUP_Bag::OnTabCheckStateChanged(bool bIsChecked, int32 Index)
{
	// (design) Batch mode is aborted when tab changes
	if (TabIndexToInventoryEnum(Index) != CurrentTab)
	{
		if (IsInMultiSelectMode)
		{
			ExitBatchMode();
		}
		else if (InWalletSellMode)
		{
			ExitSellMode();
		}
	}

	Super::OnTabCheckStateChanged(bIsChecked, Index);
}

void UUP_Bag::OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, UObject*  WidgetReturnedFrom)
{
	// Exit batch mode if operation done, stay otherwise
	switch (Result)
	{
	case EItemInventoryUpdate::NoChange:
		break;
	case EItemInventoryUpdate::OneItemUpdated:
	case EItemInventoryUpdate::Updated:
	case EItemInventoryUpdate::FilterChanged:
		if (IsInMultiSelectMode)
		{
			ExitBatchMode();
		}
		else if (InWalletSellMode)
		{
			ExitSellMode();
		}

		// fix this
		InvalidateBagCapacity();
		break;
	default:
		check(false);
		break;
	}
	if (bWidgetFinished)
	{
		Cast<URBaseWidget>(WidgetReturnedFrom)->SetVisibility(ESlateVisibility::Collapsed);
	}
	Super::OnItemInventoryUpdated(Result, bWidgetFinished, UpdatedScrollItem, EWidgetBluePrintEnum::WBP_None);
}

void UUP_Bag::OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom)
{
	switch (Result)
	{
	case EItemInventoryUpdate::NoChange:
		break;
	case EItemInventoryUpdate::OneItemUpdated:
	case EItemInventoryUpdate::Updated:
	case EItemInventoryUpdate::FilterChanged:
		if (IsInMultiSelectMode)
		{
			ExitBatchMode();
		}
		else if (InWalletSellMode)
		{
			ExitSellMode();
		}

		// fix this
		InvalidateBagCapacity();
		break;
	default:
		check(false);
		break;
	}
	Super::OnItemInventoryUpdated(Result, bWidgetFinished, UpdatedScrollItem, WidgetReturnedFrom);
}

void UUP_Bag::UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData)
{
	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ScrollItemData->ItemUD);
	if (IsInMultiSelectMode)
	{
		switch (CurrentBatchOperation)
		{
		case EItemInventoryBatchOperation::Lock:
			ScrollItemData->bDisabled = false;
			break;
		case EItemInventoryBatchOperation::Grind:
		case EItemInventoryBatchOperation::Sell:
		{
			// (design) items that are currently equipped or locked is disabled for selection
			check(ItemData != nullptr);

			ScrollItemData->bDisabled = !ItemData->equipedHeroUD.IsEmpty();
		}
		break;
		default:
			UE_LOG(LogUI, Log, TEXT("UUP_Bag::UpdateScrollItemData: CurrentBatchOperation"));
			break;
		}
	}
	else
	{
		ScrollItemData->bDisabled = false;
	}
}

void UUP_Bag::UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData)
{
	FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(ScrollItemData->WalletKey);
	check(WalletInfo != nullptr);

	if (IsInMultiSelectMode)
	{
		check(CurrentBatchOperation == EItemInventoryBatchOperation::Sell);
		ScrollItemData->bDisabled = WalletInfo->SellAvailable == 0 ? true : false;
	}
	else
	{
		if(InWalletSellMode)
			ScrollItemData->bDisabled = WalletInfo->SellAvailable == 0 ? true : false;
	}
}

void UUP_Bag::OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	check(!IsInMultiSelectMode);

	auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);

	// Show UC_Popup_ItemInfo

	UUC_GenericPopup* Popup = UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_ItemInfo);
	auto PopupContent = Cast<UUC_HeroItemInfo>(Popup->GetContentWidget());

	PopupContent->ItemUD = ScrollData->ItemUD;
	//PopupContent->IsBagMode = false;
	PopupContent->OnItemInventoryUpdatedDelegate.BindUObject(this, &UUP_Bag::OnItemInventoryUpdated, (URScrollItem*)SelectedItem, EWidgetBluePrintEnum::WBP_Popup_ItemInfo);

	Popup->InvalidateData();
}

void UUP_Bag::OnInventoryScrollViewSelectedItem_WalletItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	if (InWalletSellMode)
	{
		// Show BP UC_Popup_ItemSellQuantity
		auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
		check(IsValid(ScrollData));

		auto WalletScrollData = Cast<UInventoryScrollDataBase>(ScrollData);
		check(WalletScrollData != nullptr);

		UUC_GenericPopup* Popup = UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_WalletItemSell);
		auto PopupContent = Cast<UUC_WalletItemSell>(Popup->GetContentWidget());
		check(IsValid(PopupContent));

		PopupContent->WalletKey = WalletScrollData->WalletKey;
		//PopupContent->SelectedAmount = WalletScrollData->SelectedAmount;

		int32 OwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WalletScrollData->WalletKey);
		PopupContent->MaxAmount = OwnedAmount;

		PopupContent->OnItemInventoryUpdatedDelegate.BindUObject(this, &UUC_ItemInventoryBase::OnItemInventoryUpdated, (URScrollItem*)nullptr, EWidgetBluePrintEnum::WBP_Popup_WalletItemSell);

		Popup->InvalidateData();
	}
	else
	{
		// Super : show item info
		Super::OnInventoryScrollViewSelectedItem_WalletItem(SelectedItem);
	}
}

void UUP_Bag::EnterBatchMode()
{
// 	CurrentBatchOperation = BatchOperation;
// 
// 	if (BatchOperation == EItemInventoryBatchOperation::Lock)
// 	{
// 		IsInLockMode = true;
// 	}

	Switcher_ControlPanel->SetActiveWidgetIndex(1);
	//InvalidateButtonExecute();

	EnterMultiSelectMode();
}

void UUP_Bag::ExitBatchMode()
{
	Switcher_ControlPanel->SetActiveWidgetIndex(0);
	
	IsInLockMode = false;
	ExitMultiSelectMode();
	InventoryScrollView->MultiSelect = IsInMultiSelectMode;

	// Should invalidate before server replay comes?
}

void UUP_Bag::EnterSellMode()
{
	InWalletSellMode = true;

	Switcher_ControlPanel->SetActiveWidgetIndex(1);
	//InvalidateButtonExecute();
}

void UUP_Bag::ExitSellMode()
{
	InWalletSellMode = false;

	Switcher_ControlPanel->SetActiveWidgetIndex(0);
}

void UUP_Bag::InvalidateBagCapacity()
{
	int32 OwnedItemsCount = RGAMEINSTANCE(this)->RInventory->GetOwnedItemsCount();
	int32 BagCapacity = RGAMEINSTANCE(this)->RInventory->GetBagCapacity();

	Text_BagCapacityCurrentCount->SetText(FText::AsNumber(OwnedItemsCount));
	Text_BagCapacityCurrentCount->SetColorAndOpacity(OwnedItemsCount > BagCapacity ? Color_BagCapacityCurrentCountWhenExceeded : Color_BagCapacityCurrentCountNormal);

	Text_BagCapacityMaxCount->SetText(FText::AsNumber(BagCapacity));
}

void UUP_Bag::OpenGrindConfirmPopup()
{
	UC_Popup_Item_Grind->SetData(SelectedItemUDs);
	UC_Popup_Item_Grind->OnItemInventoryUpdated.BindUObject(this, &UUP_Bag::OnItemInventoryUpdated, (URScrollItem*)nullptr, Cast<UObject>(UC_Popup_Item_Grind));
	UC_Popup_Item_Grind->SetVisibility(ESlateVisibility::Visible);
}

void UUP_Bag::OpenSellConfirmPopup()
{
	UC_Popup_Item_Sell->SetData(SelectedItemUDs, SelectedWalletKeys, SelectedWalletAmounts);
	UC_Popup_Item_Sell->OnItemInventoryUpdated.BindUObject(this, &UUP_Bag::OnItemInventoryUpdated, (URScrollItem*)nullptr, Cast<UObject>(UC_Popup_Item_Sell));
	UC_Popup_Item_Sell->SetVisibility(ESlateVisibility::Visible);
}

void UUC_Toolbar_Button::NativeConstruct()
{
	The_Button->OnClicked.AddDynamic(this, &UUC_Toolbar_Button::OnButtonClicked);
}

void UUC_Toolbar_Button::NativeDestruct()
{
	The_Button->OnClicked.RemoveAll(this);
}

void UUC_Toolbar_Button::OnButtonClicked()
{
	OnButtonClickedDelegate.Broadcast();
}

void UUC_ItemBatchActionConfirm::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Cancel->OnClicked.AddDynamic(this, &UUC_ItemBatchActionConfirm::OnButtonCancelClicked);
	Button_OK->OnClicked.AddDynamic(this, &UUC_ItemBatchActionConfirm::OnButtonOKClicked);

	if (IsValid(ParentPopup))
	{
		ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_ItemBatchActionConfirm::OnButtonCancelClicked);
	}
}

void UUC_ItemBatchActionConfirm::NativeDestruct()
{
	if (IsValid(ParentPopup))
	{
		if (IsValid(ParentPopup->Button_Close))
			ParentPopup->Button_Close->OnClicked.RemoveAll(this);
	}

	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_OK))
		Button_OK->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_ItemBatchActionConfirm::OnButtonCancelClicked()
{
	OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::NoChange, true);
}

void UUC_ItemBatchActionConfirm::OnButtonOKClicked()
{
	// override me
	ensure(false);
}

// void UUC_ItemSellConfirm::OnButtonOKClicked()
// {
// 	// Do the batch sell
// 	check(SelectedItemUDs.Num() + SelectedWalletKeys.Num() != 0);
// 
// 	RGAMEINSTANCE(this)->HttpClient->OnInventorySell.AddUObject(this, &UUC_ItemSellConfirm::OnInventorySellRp);
// 
// 	// adapt to TArray<FString>
// 	TArray<FString> WalletIdList;
// 	for (FName WalletKey : SelectedWalletKeys)
// 	{
// 		// TODO: dubious. server expects case-sensitive keys. but FName are case-insensitive. Thank you Epic.
// 		WalletIdList.Add(WalletKey.ToString());
// 	}
// 
// 	UPacketFunctionLibrary::INVENTORY_SELL_RQ(SelectedItemUDs, WalletIdList, SelectedWalletAmounts);
// }

void UUC_WalletItemSell::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ParentPopup))
	{
		ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_WalletItemSell::OnButtonCancelClicked);
	}

	// hook up delegates
	Button_Cancel->OnClicked.AddDynamic(this, &UUC_WalletItemSell::OnButtonCancelClicked);
	//Button_OK->OnClicked.AddDynamic(this, &UUC_WalletItemSell::OnButtonOKClicked);

	AmountSelectSlider->OnCountChangedDelegate.BindUObject(this, &UUC_WalletItemSell::OnCountChanged);
}

void UUC_WalletItemSell::NativeDestruct()
{
	if (IsValid(AmountSelectSlider))
		AmountSelectSlider->OnCountChangedDelegate.Unbind();

	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_OK))
		Button_OK->OnClicked.RemoveAll(this);

	if (IsValid(ParentPopup))
	{
		ParentPopup->Button_Close->OnClicked.RemoveAll(this);
	}
	
	Super::NativeDestruct();
}

void UUC_WalletItemSell::InvalidateData()
{
	Super::InvalidateData();

	FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);
	check(WalletInfo != nullptr);

	SellPrice = WalletInfo->SellPrice;

	// CurrencyIcon
	int32 OwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WalletKey);
	ItemIcon->RefreshWalletIcon(WalletKey, WalletInfo, OwnedAmount);

	Text_OwnedAmount->SetText(FText::AsNumber(OwnedAmount));

	// Initialize Slider
	AmountSelectSlider->CurrentCount = SelectedAmount;
	AmountSelectSlider->MaxCount = OwnedAmount;

	InvalidateSelection();
}

void UUC_WalletItemSell::OnButtonCancelClicked()
{
	OnItemInventoryUpdatedDelegate.ExecuteIfBound(EItemInventoryUpdate::NoChange, true);
}

// void UUC_WalletItemSell::OnButtonOKClicked()
// {
// 	// delegate to UP_Popup_ItemSellConfirm
// 	check(SelectedAmount != 0);
// 
// 	// Show BP UP_Popup_ItemSellConfirm
// 	UUC_GenericPopup* Popup = UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_ItemSellGrindWarning);
// 	auto PopupContent = Cast<UUC_ItemSellConfirm>(Popup->GetContentWidget());
// 
// 	PopupContent->SelectedItemUDs = {};
// 	PopupContent->SelectedWalletKeys = { WalletKey };
// 	PopupContent->SelectedWalletAmounts = { SelectedAmount };
// 	PopupContent->OnItemInventoryUpdated.BindUObject(this, &UUC_WalletItemSell::OnItemInventoryUpdated);
// 
// 	Popup->InvalidateData();
// 
// 	// Not doing it myself
// 	RGAMEINSTANCE(this)->HttpClient->OnInventorySell.AddUObject(this, &UUC_WalletItemSell::OnInventorySellRp);
// 
// 	UPacketFunctionLibrary::INVENTORY_SELL_RQ({}, { WalletKey.ToString() }, { SelectedAmount });
// }

void UUC_WalletItemSell::OnCountChanged(int32 NewCount)
{
	SelectedAmount = NewCount;

	InvalidateSelection();
}

// void UUC_WalletItemSell::OnInventorySellRp(bool bSuccess, TArray<FREWARD> Rewards)
// {
// 	RGAMEINSTANCE(this)->HttpClient->OnInventorySell.RemoveAll(this);
// 
// 	if (bSuccess)
// 	{
// 		UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_RewardsSplash);
// 		check(Splash);
// 		UUC_RewardsSplash* RewardsSplash = Cast<UUC_RewardsSplash>(Splash->GetContentWidget());
// 		check(RewardsSplash);
// 
// 		RewardsSplash->RefreshRewards(Rewards, EItemInventoryRewardOrigin::Sell);
// 
// 		OnItemInventoryUpdatedDelegate.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
// 	}
// 	else
// 	{
// 		// TODO
// 		ensure(false);
// 	}
// }

void UUC_WalletItemSell::OnItemInventoryUpdated(EItemInventoryUpdate Result, bool WidgetFinished)
{
	if (WidgetFinished)
	{
		RGAMEINSTANCE(this)->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemSellGrindWarning);
	}

	// coming from UC_ItemSellConfirm. either cancel or deed done
	if (Result == EItemInventoryUpdate::NoChange)
	{
		// cancel
		// nothing
	}
	else
	{
		// done deed
		check(WidgetFinished);
		OnItemInventoryUpdatedDelegate.ExecuteIfBound(Result, true);
	}
}

void UUC_WalletItemSell::InvalidateSelection()
{
	Text_SelectedAmount->SetText(FText::AsNumber(SelectedAmount));
	Text_GoldRewardAmount->SetText(FText::AsNumber(SelectedAmount * SellPrice));

	Button_OK->SetIsEnabled(SelectedAmount != 0);
}

void UUC_BagExtend::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Cancel->OnClicked.AddDynamic(this, &UUC_BagExtend::OnButtonCancelClicked);
	Button_OK->OnClicked.AddDynamic(this, &UUC_BagExtend::OnButtonOKClicked);

	if (IsValid(ParentPopup))
	{
		ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_BagExtend::OnButtonCancelClicked);
	}
}

void UUC_BagExtend::NativeDestruct()
{
	if (IsValid(ParentPopup))
	{
		if (IsValid(ParentPopup->Button_Close))
			ParentPopup->Button_Close->OnClicked.RemoveAll(this);
	}

	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_OK))
		Button_OK->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_BagExtend::InvalidateData()
{
	// these checks are getting dangerous..
	check(RGAMEINSTANCE(this)->RInventory->CanBagBeExtendedFurther());

	int32 BluestoneCost = RGAMEINSTANCE(this)->RInventory->NextBagExtensionBluestoneCost();

	FConstantVariablesTableInfo* ConstantInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("ITEM_INVENTORY_AddRate")));
	if (!ensure(ConstantInfo != nullptr))
		return;

	int32 InventoryAddRate = 0;
	ConstantInfo->GetValue(InventoryAddRate);

	int32 CurrentBagCapacity = RGAMEINSTANCE(this)->RInventory->GetBagCapacity();
	int32 NextBagCapacity = CurrentBagCapacity + InventoryAddRate;

	Text_BagCapacityBefore->SetText(FText::AsNumber(CurrentBagCapacity));
	Text_BagCapacityAfter->SetText(FText::AsNumber(NextBagCapacity));

	Text_BluestoneCost->SetText(FText::AsNumber(BluestoneCost));

	bool CanExtend = HasEnoughBluestones();
	Button_OK->SetIsEnabled(CanExtend);

	// Text_Description
	FText DescriptionTemplateText = DescriptionText.GetText();
	FFormatNamedArguments Args;
	Args.Add(TEXT("bluestone"), FText::AsNumber(BluestoneCost));
	Args.Add(TEXT("slot"), FText::AsNumber(InventoryAddRate));
	FText FormattedText = FText::Format(DescriptionTemplateText, Args);
	Text_Description->SetText(FormattedText);
}

void UUC_BagExtend::OnButtonCancelClicked()
{
	OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::NoChange, true);
}

void UUC_BagExtend::OnButtonOKClicked()
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryUpgrade.AddUObject(this, &UUC_BagExtend::OnInventoryUpgradeRp);

	int32 CurrentSlotTokens = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_ITEM_INVENTORY_SLOT_TOKEN);
	// 임시로 ITEM을 넣음
	// 박승진
	UPacketFunctionLibrary::INVENTORY_UPGRADE_RQ(CurrentSlotTokens + 1, (int32)(EINVENTORY_UPGRADE_TYPE::ITEM));
}

void UUC_BagExtend::OnInventoryUpgradeRp(bool bSuccess)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryUpgrade.RemoveAll(this);

	if (bSuccess)
	{
		// display UP_Splash_BagExtendResult

		UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_BagExtendResult);
		check(Splash);

		OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
	}
	else
	{
		ensure(false);
	}
}

bool UUC_BagExtend::HasEnoughBluestones()
{
	int32 OwnedBluestoneCount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GEM);
	int32 NextCost = RGAMEINSTANCE(this)->RInventory->NextBagExtensionBluestoneCost();
	return OwnedBluestoneCount >= NextCost;
}
