// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemInventoryBase.h"

#include "UI/Control/RScrollView.h"
#include "GlobalIntegrated.h"

#include "GameInfo/RInventory.h"

#include "Table/ItemTableInfo.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "UI/Common/ItemInventory_ScrollData.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "Table/CharacterTableInfo.h"
#include "UI/Control/RScrollItem.h"
#include "UC_GenericPopup.h"
#include "UI/RWidgetManager.h"
#include "UI/Component/CheckBox_Radio.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroItemInfo.h"
#include "UC_ItemInventoryFilter.h"
#include "ItemFilter.h"
#include "Table/InventoryFilterTableInfo.h"
#include "UC_PopupContent_AmountSelect.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UC_WalletItemInfo.h"
#include "UI/RBaseWidget.h"
#include "UIFunctionLibrary.h"

const int32 MAX_ITEM_COUNT = 100;

void UUC_ItemInventory_ScrollItem::InvalidateData()
{
	auto InventoryItemData = Cast<UInventoryScrollDataBase>(ItemData);
	if (InventoryItemData == nullptr)
	{
		return;
	}
	if(Image_Disabled)
		Image_Disabled->SetVisibility(InventoryItemData->bDisabled ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	auto EquipmentScrollData = Cast<UInventoryScrollDataBase>(InventoryItemData);
	if (EquipmentScrollData != nullptr && !EquipmentScrollData->ItemUD.IsEmpty())
	{
		FITEM* ItemInventoryData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(EquipmentScrollData->ItemUD);
		if (ItemInventoryData != nullptr)
		{
			FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemInventoryData->itemId);
			if (ItemInfo)
			{
				if (GenericItemIcon)
				{
					GenericItemIcon->RefreshEquipmentIcon(ItemInfo, ItemInventoryData, EquipmentScrollData->IsSelected, EquipmentScrollData->IsInLockMode);
				}
			}
		}
		else
		{
			GenericItemIcon->RefreshHeroIcon(EquipmentScrollData->ItemUD);
		}
			

		return;
	}
	auto WalletScrollData = Cast<UInventoryScrollDataBase>(InventoryItemData);
	if (WalletScrollData != nullptr)
	{
		FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletScrollData->WalletKey);
		int32 OwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WalletScrollData->WalletKey);
		GenericItemIcon->RefreshWalletIcon(WalletScrollData->WalletKey, WalletInfo, OwnedAmount, WalletScrollData->IsSelected, WalletScrollData->SelectedAmount);
		return;
	}

	
}

void UUC_ItemInventory_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_ItemInventory_ScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_ItemInventory_ScrollItem::SetSelected(bool bSelected)
{
	IsSelected = bSelected;
	auto EquipmentScrollData = Cast<UInventoryScrollDataBase>(ItemData);
	if (EquipmentScrollData)
		GenericItemIcon->SetSelectedPanel(bSelected, true);
	else
		GenericItemIcon->SetSelectedPanel(bSelected, false);
}

void UUC_ItemInventoryBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (InventoryScrollView)
	{
        UE_LOG(LogClass, Log ,TEXT("Before Init"));
		if (!InventoryScrollView->ScrollItemList.Num())
			InventoryScrollView->Init_NoClear(30);
		if (!CachedItemDataList.Num())
		{
			for (size_t i = 0; i < MAX_ITEM_COUNT; ++i)
			{
				URScrollItemData* ItemData = InventoryScrollView->CreateScrollItemData(UInventoryScrollDataBase::StaticClass());
				ItemData->WidgetSize = InventoryScrollView->GetItemInitSize();
				CachedItemDataList.Emplace(ItemData);
			}
		}
		InventoryScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUC_ItemInventoryBase::OnInventoryScrollViewSelectedItem);
		InventoryScrollView->OnScrollViewSelectedItem.AddDynamic(this, &UUC_ItemInventoryBase::OnInventoryScrollViewSelectedItem);
	}
    UE_LOG(LogClass, Log ,TEXT("After Init"));
    
	RGAMEINSTANCE(this)->RInventory->OnClickInvenFilterButton.RemoveDynamic(this, &UUC_ItemInventoryBase::ReceiveFilterClicked);
	RGAMEINSTANCE(this)->RInventory->OnClickInvenFilterButton.AddDynamic(this, &UUC_ItemInventoryBase::ReceiveFilterClicked);
	// Hook up delegates

	for (int32 i = 0; i < TabCheckBoxes.Num(); i++)
	{
		TabCheckBoxes[i]->OnCheckRadioCheck_StaticDelegate.BindUObject(this, &UUC_ItemInventoryBase::OnTabCheckStateChanged, i);

		// hook up UCheckBox_Radio.Switcher to switchers so that toggling UI will work
		TabCheckBoxes[i]->Switcher = BagTabs[i]->Switcher_Button;
	}
	RGAMEINSTANCE(this)->RInventory->OnClickInvenSortingButton.RemoveDynamic(this, &UUC_ItemInventoryBase::OnClickSortingButton);
	RGAMEINSTANCE(this)->RInventory->OnClickInvenSortingButton.AddDynamic(this, &UUC_ItemInventoryBase::OnClickSortingButton);

	if (IsValid(Button_Sort_Descending))
	{
		Button_Sort_Descending->OnClicked.RemoveDynamic(this, &UUC_ItemInventoryBase::OnClickShowSortButton);
		Button_Sort_Descending->OnClicked.AddDynamic(this, &UUC_ItemInventoryBase::OnClickShowSortButton);
	}
		
	if (IsValid(Button_Sort_Ascending))
	{
		Button_Sort_Ascending->OnClicked.RemoveDynamic(this, &UUC_ItemInventoryBase::OnClickShowSortButton);
		Button_Sort_Ascending->OnClicked.AddDynamic(this, &UUC_ItemInventoryBase::OnClickShowSortButton);
	}

	if (IsValid(Button_Filter_Show))
	{
		Button_Filter_Show->OnClicked.AddDynamic(this, &UUC_ItemInventoryBase::OnButtonFilterShowClicked);
	}

	if (IsValid(CheckBox_Filter_Active))
	{
		CheckBox_Filter_Active->OnCheckStateChanged.RemoveDynamic(this, &UUC_ItemInventoryBase::OnCheckBoxFilterActiveChanged);
		CheckBox_Filter_Active->OnCheckStateChanged.AddDynamic(this, &UUC_ItemInventoryBase::OnCheckBoxFilterActiveChanged);
	}
	//
	SortAscending = true;

#if WITH_EDITOR
	RGAMEINSTANCE(this)->RGameUserSetting->LoadSettings(true);
#endif
}

void UUC_ItemInventoryBase::NativeDestruct()
{
	if (IsValid(Button_Sort_Descending))
		Button_Sort_Descending->OnClicked.RemoveAll(this);
	if (IsValid(Button_Sort_Ascending))
		Button_Sort_Ascending->OnClicked.RemoveAll(this);

	if (IsValid(CheckBox_Filter_Active))
	{
		CheckBox_Filter_Active->OnCheckStateChanged.RemoveAll(this);
	}

	if (IsValid(Button_Filter_Show))
	{
		Button_Filter_Show->OnClicked.RemoveAll(this);
	}

	if (IsValid(InventoryScrollView))
	{
		InventoryScrollView->OnScrollViewSelectedItem.RemoveAll(this);
	}

	for (int32 i = 0; i < TabCheckBoxes.Num(); i++)
	{
		if (IsValid(TabCheckBoxes[i]))
		{
			TabCheckBoxes[i]->OnCheckRadioCheck_StaticDelegate.Unbind();
		}
	}

	Super::NativeDestruct();
}

void UUC_ItemInventoryBase::InvalidateItemInventoryScrollView()
{
	if (bInventoryUpdated)
	{
		PlayAnimation_InventoryUpdated();
		bInventoryUpdated = false;
	}

	TotalNumberOfItemsInCurrentInventory = 0; // Mind that doesn't take into account filtering

	bool bHideFilter = false;
	switch (CurrentTab)
	{
	case EItemInventoryEnum::Weapons:
	case EItemInventoryEnum::Defense:
	case EItemInventoryEnum::Accessories:
		ResetVars();
		InvalidateItemInventoryScrollView_EquipmentItems();
		break;
	case EItemInventoryEnum::HeroPieces:
		InvalidateItemInventoryScrollView_HeroPieces();
		break;
	case EItemInventoryEnum::Runes:
		InvalidateItemInventoryScrollView_Runes();
		bHideFilter = true;
		break;
	case EItemInventoryEnum::Misc:
		InvalidateItemInventoryScrollView_Material();
		bHideFilter = true;
		break;
	case EItemInventoryEnum::Costume:
		InventoryScrollView->ClearScrollItemData();
		InventoryScrollView->InvalidateData();
		bHideFilter = true;
		break;
	case EItemInventoryEnum::Heros:
		InvalidateItemInventoryScrollView_Heros();
		bHideFilter = true;
		break;
		
	default:
		UE_LOG(LogUI, Log, TEXT("UUC_ItemInventoryBase::InvalidateItemInventoryScrollView: unhandled EItemInventoryEnum"));
		return;
	}
	if(Panel_Filter)
		Panel_Filter->SetVisibility(bHideFilter ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	if(Switcher_Sort)
		Switcher_Sort->SetActiveWidgetIndex(SortAscending ? 0 : 1);


	// Do not show selected count when in Wallet-type tab. A requirement for UP_Bag
	// (design) also do not sure for lock mode
	if (IsValid(Panel_SelectedCount)) // null for non-Bag inventories
		Panel_SelectedCount->SetVisibility(IsCurrentlyInEquipmentInventory() && !IsInLockMode ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (IsInMultiSelectMode)
	{
		if (IsCurrentlyInEquipmentInventory())
		{
			// update selected items count / max count based on what are in the scroll view
			OnSelectionChanged();
			if(IsValid(Text_BatchMaxCount))
				Text_BatchMaxCount->SetText(FText::AsNumber(TotalNumberOfItemsInCurrentInventory));
		}
	}
}


void UUC_ItemInventoryBase::InvalidateTextBatchSelectedCount()
{
	if(Text_BatchSelectedCount)
		Text_BatchSelectedCount->SetText(FText::AsNumber(GetNumberOfSelectedItems()));
}

void UUC_ItemInventoryBase::ResetVars()
{
	FilteringItemList.Reset();
// 	SelectedItemUDs.Reset();
// 	SelectedWalletKeys.Reset();
	GetEquipmentItemUDs(FilteringItemList);
}

void UUC_ItemInventoryBase::OnSelectionChanged()
{
	InvalidateTextBatchSelectedCount();
}

void UUC_ItemInventoryBase::OnTabCheckStateChanged(bool bIsChecked, int32 Index)
{
	if (bIsChecked)
	{
		EItemInventoryEnum SelectedInventory = TabIndexToInventoryEnum(Index);
		CurrentTab = SelectedInventory;
		InvalidateData();
	}
}


void UUC_ItemInventoryBase::FilterVisibility(bool visible)
{
	if (visible)
	{
		UC_Popup_Item_Filter_Total->SetVisibility(ESlateVisibility::Visible);
		UC_Popup_ItemSorting->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UC_Popup_Item_Filter_Total->SetVisibility(ESlateVisibility::Collapsed);
		UC_Popup_ItemSorting->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UUC_ItemInventoryBase::OnClickShowSortButton()
{
	if (UC_Popup_ItemSorting->GetVisibility() == ESlateVisibility::Collapsed)
	{
		Switcher_Sort->SetActiveWidgetIndex(0);
		UC_Popup_ItemSorting->SetVisibility(ESlateVisibility::Visible);
		BatchSelectBtnControl.Broadcast(false);

		if (UC_Popup_Item_Filter_Total->GetVisibility() == ESlateVisibility::Visible)
			UC_Popup_Item_Filter_Total->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Switcher_Sort->SetActiveWidgetIndex(1);
		UC_Popup_ItemSorting->SetVisibility(ESlateVisibility::Collapsed);
		BatchSelectBtnControl.Broadcast(true);
	}
	UC_Popup_ItemSorting->SetTypeNDescending(CurrentSortType, SortAscending);
}

void UUC_ItemInventoryBase::OnClickSortingButton(int32 ButtonIdx)
{
	auto rInven = RGAMEINSTANCE(this)->RInventory;
	//ButtonIdx is button location from upperside

	if (CurrentSortType == (EFilterSortTypeEnum)ButtonIdx)
		SortAscending = !SortAscending;
	else
		SortAscending = false;

	switch ((EFilterSortTypeEnum)ButtonIdx)
	{
	case EFilterSortTypeEnum::VE_Grade:
		CurrentSortType = EFilterSortTypeEnum::VE_Grade;
		break;
	case EFilterSortTypeEnum::VE_AttackPower:
		CurrentSortType = EFilterSortTypeEnum::VE_AttackPower;
		break;
	case EFilterSortTypeEnum::VE_SortingOrder:
		CurrentSortType = EFilterSortTypeEnum::VE_SortingOrder;
		break;
	default:
		break;
	}
	if(IsValid(Switcher_Sort))
		Switcher_Sort->SetActiveWidgetIndex(1);

	UC_Popup_ItemSorting->SetVisibility(ESlateVisibility::Collapsed);
	BatchSelectBtnControl.Broadcast(true);
	InvalidateItemInventoryScrollView();
}

void UUC_ItemInventoryBase::OnButtonFilterShowClicked()
{
	if (UC_Popup_Item_Filter_Total->GetVisibility() == ESlateVisibility::Visible)
	{
		UC_Popup_Item_Filter_Total->SetVisibility(ESlateVisibility::Collapsed);
		BatchSelectBtnControl.Broadcast(true);
	}
	else
	{
		UC_Popup_Item_Filter_Total->SetVisibility(ESlateVisibility::Visible);
		BatchSelectBtnControl.Broadcast(false);

		if (UC_Popup_ItemSorting->GetVisibility() == ESlateVisibility::Visible)
			UC_Popup_ItemSorting->SetVisibility(ESlateVisibility::Collapsed);
	}

	switch (CurrentTab)
	{
	case EItemInventoryEnum::Weapons:
		UC_Popup_Item_Filter_Total->Filter_Switcher->SetActiveWidgetIndex(0);
		break;
	case EItemInventoryEnum::Defense:
		UC_Popup_Item_Filter_Total->Filter_Switcher->SetActiveWidgetIndex(1);
		break;
	case EItemInventoryEnum::Accessories:
		UC_Popup_Item_Filter_Total->Filter_Switcher->SetActiveWidgetIndex(2);
		break;
	default:
		break;
	}
}

void UUC_ItemInventoryBase::OnCheckBoxFilterActiveChanged(bool bIsChecked)
{
	bInventoryUpdated = true;
	InvalidateItemInventoryScrollView();
}

void UUC_ItemInventoryBase::OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom)
{
	if (bWidgetFinished)
	{
		RGAMEINSTANCE(this)->RWidgetManager->HideUserWidget(WidgetReturnedFrom);
	}

	switch (Result)
	{
	case EItemInventoryUpdate::NoChange:
		break;
	case EItemInventoryUpdate::OneItemUpdated:
		if (IsValid(UpdatedScrollItem))
		{
			check(UpdatedScrollItem->GetClass()->IsChildOf<UUC_ItemInventory_ScrollItem>());
			// it may have been taken out of widget tree? I don't know this is not sound.
			UpdatedScrollItem->InvalidateData();
		}
		else
		{
			// errorneous
			bInventoryUpdated = true;
			InvalidateItemInventoryScrollView();
		}
		break;
	case EItemInventoryUpdate::Updated:
		bInventoryUpdated = true;
		InvalidateItemInventoryScrollView();
		break;
	case EItemInventoryUpdate::FilterChanged:
		StoreCurrentInventoryFilter();

		// don't update if filter is inactive
		if (IsValid(CheckBox_Filter_Active))
		{
			if (CheckBox_Filter_Active->GetCheckedState() == ECheckBoxState::Checked)
			{
				bInventoryUpdated = true;
				InvalidateItemInventoryScrollView();
			}
		}
		else
		{
			bInventoryUpdated = true;
			InvalidateItemInventoryScrollView();
		}
		break;
	default:
		check(false);
		break;
	}
}

void UUC_ItemInventoryBase::OnSelectAmountComplete(bool bCanceled, int32 Amount, URScrollItem* SavedScrollItem)
{
	RGAMEINSTANCE(this)->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_AmountSelect);
	//PopupContent->OnComplete.RemoveAll(this);

	if (!bCanceled)
	{
		auto ScrollData = Cast<UInventoryScrollDataBase>(SavedScrollItem->ItemData);
		check(IsValid(ScrollData));

		ScrollData->IsSelected = Amount != 0;
		ScrollData->SelectedAmount = Amount;

		// Update SelectedWalletKeys and SelectedWalletAmounts

		if (ScrollData->IsSelected)
		{
			int32 Index = SelectedWalletKeys.Find(ScrollData->WalletKey);
			if (Index == INDEX_NONE)
			{
				SelectedWalletKeys.Emplace(ScrollData->WalletKey);
				SelectedWalletAmounts.Emplace(Amount);
			}
			else
			{
				SelectedWalletAmounts[Index] = Amount;
			}
		}
		else
		{
			int32 Index = SelectedWalletKeys.Find(ScrollData->WalletKey);
			if (Index != INDEX_NONE)
			{

				SelectedWalletKeys.RemoveAtSwap(Index);
				SelectedWalletAmounts.RemoveAtSwap(Index);
			}
		}

		SavedScrollItem->InvalidateData();

		OnSelectionChanged();
	}

}

void UUC_ItemInventoryBase::ReceiveFilterClicked()
{
	auto RIntstance = RGAMEINSTANCE(this);
	
	ResetVars();

	switch (CurrentTab)
	{
	case EItemInventoryEnum::Weapons:
		if (RIntstance->RInventory->AttackTypeFilter.Num() > 0)
			RIntstance->RInventory->Filter_Class(FilteringItemList);
	case EItemInventoryEnum::Defense:
		if (RIntstance->RInventory->EnchantStateFilter.Num() > 0)
			RIntstance->RInventory->Filter_EnchantType(FilteringItemList);
	case EItemInventoryEnum::Accessories:
		if (RIntstance->RInventory->itemGradeFilter.Num() > 0)
			RIntstance->RInventory->Filter_Grade(FilteringItemList);
		if (RIntstance->RInventory->UseStateFilter.Num() > 0)
			RIntstance->RInventory->Filter_UseStateType(FilteringItemList);
		if (RIntstance->RInventory->itemTierFilter.Num() > 0)
			RIntstance->RInventory->Filter_Tier(FilteringItemList);
		if (RIntstance->RInventory->EquipTypeFilter.Num() > 0)
			RIntstance->RInventory->Filter_EquipType(FilteringItemList);
		break;
	default:
		break;
	}

	InvalidateItemInventoryScrollView_EquipmentItems();
}

bool UUC_ItemInventoryBase::IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	return true;
}

void UUC_ItemInventoryBase::OnInventoryScrollViewSelectedItem(URScrollItem* SelectedItem)
{
	FilterVisibility(false);

	if (IsInMultiSelectMode)
	{
		// Toggle selected
		auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
		check(IsValid(ScrollData));

		if (!ScrollData->bDisabled)
		{
			if (!ScrollData->ItemUD.IsEmpty())
			{
				// Toggle the item select
				if (ScrollData->IsSelected)
				{
					SelectedItemUDs.RemoveSwap(ScrollData->ItemUD);

					ScrollData->IsSelected = false;
				}
				else
				{
					SelectedItemUDs.AddUnique(ScrollData->ItemUD);

					ScrollData->IsSelected = true;
				}

				SelectedItem->InvalidateData();

				OnSelectionChanged();
			}
			else
			{
				// Wallet item
				// Prompt with UC_PopupContent_AmountSelect to let user specify selected amount for batch operation

				auto WalletScrollData = ScrollData;
				check(WalletScrollData != nullptr);

				UUC_GenericPopup* Popup = UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_AmountSelect);
				auto PopupContent = Cast<UUC_PopupContent_AmountSelect>(Popup->GetContentWidget());
				check(IsValid(PopupContent));

				PopupContent->WalletKey = WalletScrollData->WalletKey;
				PopupContent->SelectedAmount = WalletScrollData->SelectedAmount;

				int32 OwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WalletScrollData->WalletKey);
				PopupContent->MaxAmount = OwnedAmount;

				PopupContent->OnComplete.AddUObject(this, &UUC_ItemInventoryBase::OnSelectAmountComplete, SelectedItem);

				Popup->InvalidateData();
			}
		}
	}
	else
	{
		auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
		check(IsValid(ScrollData));

		if (!ScrollData->bDisabled)
		{
			if (!ScrollData->ItemUD.IsEmpty())
			{
				FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ScrollData->ItemUD);
				if (ItemData)
				{
					auto ItemScrollItem = Cast<UUC_ItemInventory_ScrollItem>(SelectedItem);
					check(IsValid(ItemScrollItem));
					OnInventoryScrollViewSelectedItem_EquipmentItem(ItemScrollItem);
				}
				else
				{
					auto ItemScrollItem = Cast<UUC_ItemInventory_ScrollItem>(SelectedItem);
					check(IsValid(ItemScrollItem));
					OnInventoryScrollViewSelectedItem_HeroItem(ItemScrollItem);
				}
			}
			else
			{
				auto ItemScrollItem = Cast<UUC_ItemInventory_ScrollItem>(SelectedItem);
				check(IsValid(ItemScrollItem));
				OnInventoryScrollViewSelectedItem_WalletItem(ItemScrollItem);
			}
		}
	}
}

void UUC_ItemInventoryBase::OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	// Override this from subclass.
}

void UUC_ItemInventoryBase::OnInventoryScrollViewSelectedItem_WalletItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	// Show UP_Popup_BagItemInfo

	auto WalletScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
	check(IsValid(WalletScrollData));

	UUC_GenericPopup* Popup = UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_WalletItemInfo);
	if (Popup)
	{
		auto PopupContent = Cast<UUC_WalletItemInfo>(Popup->GetContentWidget());
		check(IsValid(PopupContent));

		PopupContent->WalletKey = WalletScrollData->WalletKey;
		PopupContent->OnItemInventoryUpdatedDelegate.BindUObject(this, &UUC_ItemInventoryBase::OnItemInventoryUpdated, (URScrollItem*)SelectedItem, EWidgetBluePrintEnum::WBP_Popup_WalletItemInfo);

		Popup->InvalidateData();
	}
}

void UUC_ItemInventoryBase::OnInventoryScrollViewSelectedItem_HeroItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{

}

void UUC_ItemInventoryBase::UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData)
{
	if(ScrollItemData)
		ScrollItemData->bDisabled = false;
}

void UUC_ItemInventoryBase::UpdateScrollItemData_HeroPiece(UInventoryScrollDataBase* ScrollItemData)
{
	if (ScrollItemData)
	{
		FWalletTableInfo* walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(ScrollItemData->WalletKey);
		if (InWalletSellMode)
			ScrollItemData->bDisabled = walletInfo->SellAvailable == 1 ? false : true;
	}
}

void UUC_ItemInventoryBase::UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData)
{
	if (ScrollItemData)
	{
		FWalletTableInfo* walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(ScrollItemData->WalletKey);
		if (InWalletSellMode)
			ScrollItemData->bDisabled = walletInfo->SellAvailable == 1 ? false : true;
	}
}

void UUC_ItemInventoryBase::UpdateScrollItemData_Hero(UInventoryScrollDataBase* ScrollItemData)
{
	if (ScrollItemData)
		ScrollItemData->bDisabled = false;
}

EItemInventoryEnum UUC_ItemInventoryBase::TabIndexToInventoryEnum(int32 Index)
{
	return (EItemInventoryEnum)Index;
}

void UUC_ItemInventoryBase::EnterMultiSelectMode()
{
	IsInMultiSelectMode = true;
	InventoryScrollView->MultiSelect = true;
}

void UUC_ItemInventoryBase::ExitMultiSelectMode()
{
	SelectedItemUDs.Empty();
	SelectedWalletKeys.Empty();
	SelectedWalletAmounts.Empty();

	IsInMultiSelectMode = false;
	InventoryScrollView->MultiSelect = false;
}

int32 UUC_ItemInventoryBase::GetNumberOfSelectedItems() const
{
	return SelectedItemUDs.Num() + SelectedWalletKeys.Num();
}

void UUC_ItemInventoryBase::GetEquipmentItemUDs(TArray<FString>& OutItemUDs)
{
	// Handle inventories All, Weapons, Defence, Accessories
	ItemFilter = GetItemInventoryFilter(CurrentTab);
	
	if (CheckBox_Filter_Active == nullptr && ItemFilter == nullptr)
		return;

	TArray<FString> AllItems;
	RGAMEINSTANCE(this)->RInventory->GetItemEquipmentKeys(AllItems, EItemTypeEnum::VE_None);

	for (const FString& ItemUD : AllItems)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if(ItemInfo == nullptr)
			continue;
		bool ApplyFilter = false;
		if (IsValid(CheckBox_Filter_Active))
		{
			ApplyFilter = CheckBox_Filter_Active->GetCheckedState() == ECheckBoxState::Checked;
		}

		bool ToInclude = IsIncludingEquipmentItem(ItemInfo, ItemData);

		if (ToInclude)
		{
			switch (CurrentTab)
			{
			case EItemInventoryEnum::Weapons:
				ToInclude = ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Weapon;
				if (ToInclude)
				{
					TotalNumberOfItemsInCurrentInventory++;
				}
				if (ToInclude && ApplyFilter)
				{
					ToInclude = ItemFilter->Predicate(ItemInfo, ItemData, nullptr);
				}
				break;
			case EItemInventoryEnum::Defense:
				ToInclude = ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Armor || ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Pants;
				if (ToInclude)
				{
					TotalNumberOfItemsInCurrentInventory++;
				}
				if (ToInclude && ApplyFilter)
				{
					ToInclude = ItemFilter->Predicate(ItemInfo, ItemData, nullptr);
				}
				break;
			case EItemInventoryEnum::Accessories:
				ToInclude = ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Necklace || ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Ring;
				if (ToInclude)
				{
					TotalNumberOfItemsInCurrentInventory++;
				}
				if (ToInclude && ApplyFilter)
				{
					ToInclude = ItemFilter->Predicate(ItemInfo, ItemData, nullptr);
				}
				break;
			default:
				break;
			}
		}

		if (ToInclude)
		{
			OutItemUDs.Emplace(ItemUD);
		}
	}
	RGAMEINSTANCE(this)->RInventory->SetItemSort(OutItemUDs, EFilterSortTypeEnum::VE_Grade, SortAscending);
	// Sort Equipment Inventory
	// (design) sort order rule -> 정렬 방식_김현주.docx
// 
// 	OutItemUDs.Sort([&](const FString& ItemUD_A, const FString& ItemUD_B) {
// 		FITEM* ItemData_A = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD_A);
// 		check(ItemData_A != nullptr);
// 		FItemTableInfo* ItemInfo_A = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData_A->itemId);
// 		check(ItemInfo_A != nullptr);
// 
// 		FITEM* ItemData_B = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD_B);
// 		check(ItemData_B != nullptr);
// 		FItemTableInfo* ItemInfo_B = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData_B->itemId);
// 		check(ItemInfo_B != nullptr);
// 
// 		// Grade
// 		if ((int32)ItemInfo_A->ItemGrade < (int32)ItemInfo_B->ItemGrade)
// 		{
// 			return SortAscending;
// 		}
// 		else if ((int32)ItemInfo_A->ItemGrade > (int32)ItemInfo_B->ItemGrade)
// 		{
// 			return !SortAscending;
// 		}
// 
// 		// CombatPower, descending
// 		int32 CombatPower_A = FItemTableInfo::GetCombatPower(ItemData_A);
// 		int32 CombatPower_B = FItemTableInfo::GetCombatPower(ItemData_B);
// 		if (CombatPower_A < CombatPower_B)
// 		{
// 			return SortAscending;
// 		}
// 		else if (CombatPower_A > CombatPower_B)
// 		{
// 			return !SortAscending;
// 		}
// 
// 		// CreationNumber "획득시점"
// 		int32 CreationNumber_A = ItemUDToCreationNumber(ItemUD_A);
// 		int32 CreationNumber_B = ItemUDToCreationNumber(ItemUD_B);
// 
// 		if (CreationNumber_A < CreationNumber_B)
// 		{
// 			return SortAscending;
// 		}
// 		else if (CreationNumber_A > CreationNumber_B)
// 		{
// 			return !SortAscending;
// 		}
// 
// 		return false;
// 	});
}

void UUC_ItemInventoryBase::GetCurrencies(TArray<FName>& OutCurrencies, EWalletPropertyType WalletPropertyType)
{
	// precursory code...

	TArray<FName> WalletKeys = UTableManager::GetInstancePtr()->DTWallet->GetRowNames();
	for (FName WalletKey : WalletKeys)
	{
		FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);
		if (!ensure(WalletInfo != nullptr))
			continue;

		if (WalletInfo->PropertyType == WalletPropertyType || WalletPropertyType == EWalletPropertyType::None)
		{
			int32 Amount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WalletKey);
			if (Amount > 0)
			{
				OutCurrencies.Emplace(WalletKey);
				TotalNumberOfItemsInCurrentInventory++;
			}
		}
	}

	// Sort
	OutCurrencies.Sort([&](FName WalletKey_A, FName WalletKey_B) {
		FWalletTableInfo* WalletInfo_A = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey_A);
		check(WalletInfo_A != nullptr);
		FWalletTableInfo* WalletInfo_B = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey_B);
		check(WalletInfo_B != nullptr);

		if (WalletInfo_A->Sorting_Order < WalletInfo_B->Sorting_Order)
		{
			return SortAscending;
		}
		else if (WalletInfo_A->Sorting_Order > WalletInfo_B->Sorting_Order)
		{
			return !SortAscending;
		}

		return false;
	});
}

void UUC_ItemInventoryBase::InvalidateItemInventoryScrollView_EquipmentItems()
{
	if (InventoryScrollView && Switcher_ItemInventoryScrollView)
	{
		if (FilteringItemList.Num() == 0)
		{
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(1);
		}
		else
		{
			InventoryScrollView->ClearScrollItemData(MAX_ITEM_COUNT);

			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(0);
			RGAMEINSTANCE(this)->RInventory->SetItemSort(FilteringItemList, CurrentSortType, SortAscending);
			for (size_t i = 0 ; i < FilteringItemList.Num() ; ++i)
			{
				if (i >= MAX_ITEM_COUNT)
					break;

				FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(FilteringItemList[i]);
				checkf(ItemData != nullptr, TEXT("failed to get ItemData"));
				FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
				if (ItemInfo == nullptr)
				{
					UE_LOG(LogUI, Log, TEXT("UUC_ItemInventoryBase::InvalidateItemInventoryScrollView: got itemId from inventory that is missing in the item table: %s"), *FilteringItemList[i]);
					continue;
				}

				UInventoryScrollDataBase* ScrollItemData = nullptr;
				if (CachedItemDataList.IsValidIndex(i))
				{
					ScrollItemData = Cast<UInventoryScrollDataBase>(CachedItemDataList[i]);
					InventoryScrollView->ItemDataList.Emplace(ScrollItemData);
				}
				checkf(ScrollItemData, TEXT("failed to AddScrollItemData"));

				ScrollItemData->ItemUD = FilteringItemList[i];
				ScrollItemData->WalletKey = NAME_None;
				ScrollItemData->IsInLockMode = IsInLockMode;

				// Selected
				if (IsInMultiSelectMode)
				{
					ScrollItemData->IsSelected = SelectedItemUDs.Contains(FilteringItemList[i]);
				}
				else
				{
					ScrollItemData->IsSelected = false;
				}
				UpdateScrollItemData_EquipmentItem(ScrollItemData);
				//ScrollItemData->IsLocked = ItemData->locked;
			}
			InventoryScrollView->InvalidateDataWithoutResetSelect();
		}
	}
}

void UUC_ItemInventoryBase::InvalidateItemInventoryScrollView_HeroPieces()
{
	if (InventoryScrollView && Switcher_ItemInventoryScrollView)
	{
		InventoryScrollView->ClearScrollItemData(MAX_ITEM_COUNT);

		TArray<FName> HeroPieces;
		GetCurrencies(HeroPieces, EWalletPropertyType::HeroPiece);

		if (HeroPieces.Num() == 0)
		{
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(1);
		}
		else
		{
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(0);

			for (size_t i = 0 ; i<HeroPieces.Num() ; ++i)
			{
				//FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(HeroPiece.Key);
				//if (WalletInfo == nullptr)
				//{
				//	UE_LOG(LogUI, Error, TEXT("UUC_ItemInventoryBase::InvalidateItemInventoryScrollView_HeroPieces: failed to get WalletInfo"));
				//	continue;
				//}

				UInventoryScrollDataBase* ScrollItemData = nullptr;
				if (CachedItemDataList.IsValidIndex(i))
				{
					ScrollItemData = Cast<UInventoryScrollDataBase>(CachedItemDataList[i]);
					InventoryScrollView->ItemDataList.Emplace(ScrollItemData);
				}
				checkf(ScrollItemData, TEXT("failed to AddScrollItemData"));

				ScrollItemData->ItemUD = TEXT("");
				ScrollItemData->WalletKey = HeroPieces[i];

				// Selected
				ScrollItemData->IsSelected = false;

				UpdateScrollItemData_HeroPiece(ScrollItemData);
			}

			InventoryScrollView->InvalidateData();
		}
	}
}

void UUC_ItemInventoryBase::InvalidateItemInventoryScrollView_Runes()
{
	if (InventoryScrollView && Switcher_ItemInventoryScrollView) 
	{
		TArray<FName> Currencies;
		GetCurrencies(Currencies, EWalletPropertyType::Rune);

		if (Currencies.Num() == 0)
		{
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(1);
		}
		else
		{
			InventoryScrollView->ClearScrollItemData(MAX_ITEM_COUNT);
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(0);

			for (size_t i = 0 ; i <Currencies.Num() ; ++i)
			{
				if (i >= MAX_ITEM_COUNT)
					break;

				UInventoryScrollDataBase* ScrollItemData = nullptr;
				if (CachedItemDataList.IsValidIndex(i))
				{
					ScrollItemData = Cast<UInventoryScrollDataBase>(CachedItemDataList[i]);
					InventoryScrollView->ItemDataList.Emplace(ScrollItemData);
				}
				checkf(ScrollItemData, TEXT("failed to AddScrollItemData"));

				ScrollItemData->ItemUD = TEXT("");
				ScrollItemData->WalletKey = Currencies[i];

				// Selected
				ScrollItemData->IsSelected = false;

				UpdateScrollItemData_Material(ScrollItemData);
			}

			InventoryScrollView->InvalidateData();
		}
	}
}

void UUC_ItemInventoryBase::InvalidateItemInventoryScrollView_Material()
{
	if (InventoryScrollView && Switcher_ItemInventoryScrollView)
	{

		TArray<FName> Currencies;
		GetCurrencies(Currencies, EWalletPropertyType::Material);

		if (Currencies.Num() == 0)
		{
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(1);
		}
		else
		{
			InventoryScrollView->ClearScrollItemData(MAX_ITEM_COUNT);
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(0);

			for (size_t i = 0; i <Currencies.Num(); ++i)
			{
				if (i >= MAX_ITEM_COUNT)
					break;

				UInventoryScrollDataBase* ScrollItemData = nullptr;
				if (CachedItemDataList.IsValidIndex(i))
				{
					ScrollItemData = Cast<UInventoryScrollDataBase>(CachedItemDataList[i]);
					InventoryScrollView->ItemDataList.Emplace(ScrollItemData);
				}
				checkf(ScrollItemData, TEXT("failed to AddScrollItemData"));

				ScrollItemData->ItemUD = TEXT("");
				ScrollItemData->WalletKey = Currencies[i];

				UpdateScrollItemData_Material(ScrollItemData);
			}

			InventoryScrollView->InvalidateData();
		}
	}
}

void UUC_ItemInventoryBase::InvalidateItemInventoryScrollView_Heros()
{
	
}

UItemFilterBase* UUC_ItemInventoryBase::GetItemInventoryFilter(EItemInventoryEnum InItemInventoryEnum)
{
	if (ItemFilter)
		ItemFilter = nullptr;

	if (EItemInventoryEnum::Weapons == InItemInventoryEnum)
	{
		ItemFilter = NewObject<UEquipmentFilter>();
		FInventoryFilterTableInfo::LookUp(ItemFilter->Rows, EItemInventoryEnum::Weapons);
		ItemFilter->LoadCheckedFactors(RGAMEINSTANCE(this)->RGameUserSetting->ItemInventoryFilter_Weapons_CheckedFactors);
	}

	if (EItemInventoryEnum::Defense == InItemInventoryEnum)
	{
		ItemFilter = NewObject<UEquipmentFilter>();
		FInventoryFilterTableInfo::LookUp(ItemFilter->Rows, EItemInventoryEnum::Defense);
		ItemFilter->LoadCheckedFactors(RGAMEINSTANCE(this)->RGameUserSetting->ItemInventoryFilter_Defense_CheckedFactors);
	}
	
	if (EItemInventoryEnum::Accessories == InItemInventoryEnum)
	{
		ItemFilter = NewObject<UEquipmentFilter>();
		FInventoryFilterTableInfo::LookUp(ItemFilter->Rows, EItemInventoryEnum::Accessories);
		ItemFilter->LoadCheckedFactors(RGAMEINSTANCE(this)->RGameUserSetting->ItemInventoryFilter_Accessories_CheckedFactors);
	}

	if (EItemInventoryEnum::HeroPieces == InItemInventoryEnum)
	{
		ItemFilter = NewObject<UHeroPieceFilter>();
		FInventoryFilterTableInfo::LookUp(ItemFilter->Rows, EItemInventoryEnum::HeroPieces);
		ItemFilter->LoadCheckedFactors(RGAMEINSTANCE(this)->RGameUserSetting->ItemInventoryFilter_HeroPieces_CheckedFactors);
	}

	return ItemFilter;
}

bool UUC_ItemInventoryBase::IsCurrentlyInEquipmentInventory()
{
	switch (CurrentTab)
	{
	case EItemInventoryEnum::Weapons:
	case EItemInventoryEnum::Defense:
	case EItemInventoryEnum::Accessories:
		return true;
		break;
	case EItemInventoryEnum::Runes:
	case EItemInventoryEnum::HeroPieces:
	case EItemInventoryEnum::Misc:
	case EItemInventoryEnum::Costume:
		return false;
		break;
	default:
		// error
		return false;
		break;
	}
}

void UUC_ItemInventoryBase::StoreCurrentInventoryFilter()
{
	if (ItemFilter == nullptr)
		return;
	switch (CurrentTab)
	{
	case EItemInventoryEnum::Weapons:
		ItemFilter->StoreCheckedFactors(RGAMEINSTANCE(this)->RGameUserSetting->ItemInventoryFilter_Weapons_CheckedFactors);
		RGAMEINSTANCE(this)->RGameUserSetting->ApplySettings(true);
		break;
	case EItemInventoryEnum::Defense:
		ItemFilter->StoreCheckedFactors(RGAMEINSTANCE(this)->RGameUserSetting->ItemInventoryFilter_Defense_CheckedFactors);
		RGAMEINSTANCE(this)->RGameUserSetting->ApplySettings(true);
		break;
	case EItemInventoryEnum::Accessories:
		ItemFilter->StoreCheckedFactors(RGAMEINSTANCE(this)->RGameUserSetting->ItemInventoryFilter_Accessories_CheckedFactors);
		RGAMEINSTANCE(this)->RGameUserSetting->ApplySettings(true);
		break;
	default:
		// TODO
		break;
	}
}

int32 UUC_ItemInventoryBase::ItemUDToCreationNumber(const FString& InItemUD)
{
	FString _Left;
	FString Right;
	if (!InItemUD.Split(TEXT("-"), &_Left, &Right))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_ItemInventoryBase::ItemUDToCreationNumber: Unrecognized ItemUD format: %s"), *InItemUD);
		return 0;
	}

	return FCString::Atoi(*Right);
}
