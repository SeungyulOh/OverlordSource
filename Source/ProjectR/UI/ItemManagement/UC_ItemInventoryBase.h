// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItemData.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "UI/Control/RScrollItem.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UI/Common/Popup/UC_Popup_ItemSorting.h"
#include "UI/Common/Popup/UC_Popup_Item_Filter.h"
#include "Global/EventManager.h"
#include "UC_ItemInventoryBase.generated.h"

class URScrollView;
class UTextBlock;
class UCheckBox_Radio;
class URScrollItemData;
class UButton;
class URScrollItem;
class UC_ItemInventory_ScrollItem;
class UWidgetSwitcher;
class UEquipmentScrollData;
class UHeroPieceScrollData;
class UMaterialScrollData;
class UItemFilterBase;
// class UEquipmentFilter;
// class UHeroPieceFilter;
class UUC_GenericItemIcon;
class UHorizontalBox_RadioGroup;


UENUM(BlueprintType)
enum class EItemInventoryEnum : uint8
{
	Weapons,		// DT_CharacterItem.EquipPosition of VE_Weapon
	Defense,		// DT_CharacterItem.EquipPosition of VE_Armor and VE_Pants
	Accessories,	// DT_CharacterItem.EquipPosition of VE_Necklace and VE_Ring
	HeroPieces,		// DT_Wallet.PropertyType of HeroPiece
	Runes,			// DT_Wallet.PropertyType of Rune
	Misc,			// DT_Wallet.PropertyType of Material and Consumable (design tentative)
	Costume,		// DT_Wallet.PropertyType of Material and Consumable (design tentative)
	Heros,

	Max,
};

UCLASS()
class PROJECTR_API UInventoryScrollDataBase : public URScrollItemData
{
	GENERATED_BODY()

public:
	bool bDisabled = false;

	bool IsSelected = false;

	FName WalletKey = NAME_None;

	// for batch multi select mode
	int32 SelectedAmount = 0;

	FString ItemUD;

	// hackish
	bool IsInLockMode = false;
};

//UCLASS()
//class PROJECTR_API UWalletInventoryScrollData : public UInventoryScrollDataBase
//{
//	GENERATED_BODY()
//
//public:
//	FName WalletKey = NAME_None;
//
//	// for batch multi select mode
//	int32 SelectedAmount = 0;
//};
//
//
//UCLASS()
//class PROJECTR_API UEquipmentScrollData : public UInventoryScrollDataBase
//{
//	GENERATED_BODY()
//
//public:
//	FString ItemUD;
//
//	// hackish
//	bool IsInLockMode = false;
//};

/*
* used by UC_ItemInventoryBase
* expects as ScrollItemData either UWalletInventoryScrollData or UEquipmentScrollData
* displays UC_GenericItemIcon and a few other widgets on top of it
* * disabled
* * (selected is handled by item icons for reasons annoying)
*/
UCLASS()
class PROJECTR_API UUC_ItemInventory_ScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:

	void InvalidateData() override;

	//// BP Widgets

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemInventory_ScrollItem)
    UUC_GenericItemIcon* GenericItemIcon;
//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemInventory_ScrollItem)
	UImage* Image_Disabled;


	virtual void NativeConstruct() override;


	virtual void NativeDestruct() override;

	UFUNCTION()
		virtual void SetSelected(bool bSelected) override;

private:
};

/**
* Tab widget
*/
UCLASS()
class PROJECTR_API UUC_Bag_Tab : public URBaseWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Bag_Tab)
	FName LocalizingKey = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Bag_Tab)
	EStringTableType StringTableType = EStringTableType::VE_UI;
	
	// Overriding if not empty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Bag_Tab)
	FText OverrideText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Bag_Tab)
	UWidgetSwitcher* Switcher_Button;
};

UCLASS()
class PROJECTR_API UUC_Toolbar_Button : public URBaseWidget
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Toolbar_Button)
	UButton* The_Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Toolbar_Button)
	UImage* Image_Button;

	DECLARE_MULTICAST_DELEGATE(FOnButtonClickedDelegate)
	FOnButtonClickedDelegate OnButtonClickedDelegate;
	
	UFUNCTION()
	void OnButtonClicked();
};


UENUM(BlueprintType)
enum class EItemInventoryUpdate : uint8
{
	NoChange,
	OneItemUpdated,
	Updated,
	FilterChanged,
};

DECLARE_DELEGATE_TwoParams(FOnItemInventoryUpdated, EItemInventoryUpdate, bool /* bWidgetComplete */);

UCLASS()
class PROJECTR_API UUC_ItemInventoryBase : public URBaseWidget
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void InvalidateItemInventoryScrollView();

	void InvalidateTextBatchSelectedCount();
	void ResetVars();
	virtual void OnSelectionChanged();

	// UI update state
	// set this to true and next InvalidateItemInventoryScrollView will play 'inventory updated' animation
	bool bInventoryUpdated = false;

	//// BP Widgets

	// Assumes the tabs correspond to (int32)EItemInventoryEnum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UHorizontalBox_RadioGroup* RadioGroup_Tabs;

	// Tab checkboxes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	TArray<UCheckBox_Radio*> TabCheckBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	TArray<UUC_Bag_Tab*> BagTabs;

	/** 0: ItemInventory, 1: UC_Item_Info_None */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UWidgetSwitcher* Switcher_ItemInventoryScrollView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	URScrollView* InventoryScrollView;

	UPROPERTY()
	TArray<URScrollItemData*> CachedItemDataList;

	//
	FDelegate_OnOneParamBool BatchSelectBtnControl;

	// Sort and Filter
	// WARNING: these may not be bound for some widgets

	/** index 0: descending, 1: ascending */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UWidgetSwitcher* Switcher_Sort;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UButton* Button_Sort_Descending;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UButton* Button_Sort_Ascending;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
		UUC_Popup_ItemSorting* UC_Popup_ItemSorting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
		UUC_Popup_Item_Filter_Total* UC_Popup_Item_Filter_Total;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UWidget* Panel_Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UButton* Button_Filter_Show;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UCheckBox* CheckBox_Filter_Active;
	
	// For multi select mode
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UWidget* Panel_SelectedCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UTextBlock* Text_BatchSelectedCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryBase)
	UTextBlock* Text_BatchMaxCount;

	//// BP Configs

	//// BP Effect

	UFUNCTION(BlueprintImplementableEvent, Category = UUC_ItemInventoryBase)
	void PlayAnimation_InventoryUpdated();

	//// Delegates
	
	UFUNCTION()
	virtual void OnTabCheckStateChanged(bool bIsChecked, int32 Index);

	UFUNCTION()
		void FilterVisibility(bool visible);
	UFUNCTION()
		void OnClickShowSortButton();
	UFUNCTION()
		void OnClickSortingButton(int32 ButtonIdx);
	UFUNCTION()
	void OnButtonFilterShowClicked();

	UFUNCTION()
	void OnCheckBoxFilterActiveChanged(bool bIsChecked);

	UFUNCTION()
	virtual void OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom);

	UFUNCTION()
	void OnSelectAmountComplete(bool bCanceled, int32 Amount, URScrollItem* SavedScrollItem);

	UFUNCTION()
		void ReceiveFilterClicked();

	//can call in other class
	void EnterMultiSelectMode();
	void ExitMultiSelectMode();

	virtual void InvalidateItemInventoryScrollView_Heros();

protected:
	// extend these
	
	// for filtering inventory item from child class
	virtual bool IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData);

	UFUNCTION()
	virtual void OnInventoryScrollViewSelectedItem(URScrollItem* SelectedItem);

	UFUNCTION()
	virtual void OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem);
	UFUNCTION()
	virtual void OnInventoryScrollViewSelectedItem_WalletItem(UUC_ItemInventory_ScrollItem* SelectedItem);
	UFUNCTION()
	virtual void OnInventoryScrollViewSelectedItem_HeroItem(UUC_ItemInventory_ScrollItem* SelectedItem);

	// overridable to allow subclass to modify items displayed
	virtual void UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData);
	virtual void UpdateScrollItemData_HeroPiece(UInventoryScrollDataBase* ScrollItemData);
	virtual void UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData);
	virtual void UpdateScrollItemData_Hero(UInventoryScrollDataBase* ScrollItemData);


	virtual EItemInventoryEnum TabIndexToInventoryEnum(int32 Index);

	//// Principal state variables
	EItemInventoryEnum CurrentTab = EItemInventoryEnum::Max;
	//// state
	/** For selling wallet type items. disjoint with batch multiselect mode because this is not multiselect */
	UPROPERTY()
	bool InWalletSellMode = false;
	//// Multi select state
	UPROPERTY()
	bool IsInMultiSelectMode = false;
	UPROPERTY()
	TArray<FString> SelectedItemUDs;
	UPROPERTY()
	TArray<FName> SelectedWalletKeys;
	UPROPERTY()
	TArray<int32> SelectedWalletAmounts;
	UFUNCTION()
	int32 GetNumberOfSelectedItems() const;
	// value cached on scroll view invalidation. Currently only updated for equipment items
	// only for multi select mode
	UPROPERTY()
	int32 TotalNumberOfItemsInCurrentInventory = 0;
	
	// hacky state variable to differently render the select state of equipment item
	UPROPERTY()
	bool IsInLockMode = false;

	/** (design) Descending by default */
	UPROPERTY()
	bool SortAscending = true;

	// helper
	bool IsCurrentlyInEquipmentInventory();

	// Inventory-specific things
	// Filters and sorts
	void GetEquipmentItemUDs(TArray<FString>& OutItemUDs);
	/** Return currency keys for owned amount > 0, sorted but not filtered. No regard for DT_Wallet.PropertyType if EWalletPropertyType::None */
	void GetCurrencies(TArray<FName>& OutCurrencies, EWalletPropertyType WalletPropertyType);

	
private:
	void InvalidateItemInventoryScrollView_EquipmentItems();
	void InvalidateItemInventoryScrollView_HeroPieces();
	void InvalidateItemInventoryScrollView_Runes();
	void InvalidateItemInventoryScrollView_Material();
	

	UItemFilterBase*	GetItemInventoryFilter(EItemInventoryEnum InItemInventoryEnum);

// Inventory filters
	UPROPERTY()
	UItemFilterBase*	ItemFilter = nullptr;
	TArray<FString> FilteringItemList;
	EFilterSortTypeEnum CurrentSortType = EFilterSortTypeEnum::VE_Grade;


// 	UPROPERTY()
// 	UEquipmentFilter* ItemFilter_Weapons = nullptr;
// 	UPROPERTY()
// 	UEquipmentFilter* ItemFilter_Defense = nullptr;
// 	UPROPERTY()
// 	UEquipmentFilter* ItemFilter_Accessories = nullptr;
// 	UPROPERTY()
// 	UHeroPieceFilter* ItemFilter_HeroPieces = nullptr;

	// filter load/store helpers
	void StoreCurrentInventoryFilter();

	/** "I123-7" to 7 */
	static int32 ItemUDToCreationNumber(const FString& InItemUD);

};
