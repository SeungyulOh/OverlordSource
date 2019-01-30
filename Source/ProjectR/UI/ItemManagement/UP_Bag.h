// Fill out your copyright notice in the Description page of Project Settings.

/**
* 오버로드 아이템 UI 기획서 - 가방
* 아이템 UI_김현주(2).pdf.II 가방 UI
*/

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UC_ItemInventoryBase.h"
#include "Network/PacketDataStructures.h"
#include "UC_GenericPopupContent.h"
#include "UI/Common/Popup/UC_Popup_BagExtend.h"
#include "UI/Common/Popup/UC_Popup_Item_Sell.h"
#include "UI/Common/Popup/UC_Popup_Item_Grind.h"

#include "UI/Common/Popup/UC_Popup_HeroInfo_Lock.h"

#include "UP_Bag.generated.h"

class UCanvasPanel_RadioGroup;
class URScrollView;
class UTextBlock;
class UCheckBox_Radio;
class URScrollItemData;
class UButton;
class URScrollItem;
class URScrollItem;
class UWidgetSwitcher;
class UUC_AmountSelectSlider;
class UUC_Popup_HeroInfo_Lock;

UENUM(BlueprintType)
enum class EItemInventoryBatchOperation : uint8
{
	Lock,
	Grind,
	Sell,
};


/**
 *
 */
UCLASS()
class PROJECTR_API UUP_Bag : public UUC_ItemInventoryBase
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void InvalidateData() override;

	void InvalidateButtonExecute();

	void OnSelectionChanged() override;

	//// BP Config
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	FSlateColor Color_BagCapacityCurrentCountWhenExceeded;

	//// Toolbar widgets

	// Batch operations

	/** index 0: normal mode, 1: multi select mode*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UWidgetSwitcher* Switcher_ControlPanel;
	/** 0: Button_Select for equipment, 1: Button_Sell for wallet types */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UWidgetSwitcher* Switcher_Sell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_Select;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_BatchLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_BatchSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_Sell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_Grind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_Lock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	ULocalizingTextBlock* Text_BatchExecuteLabel;

	// Bag Capacity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UTextBlock* Text_BagCapacityCurrentCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UTextBlock* Text_BagCapacityMaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_BagExtend;
	
	//popup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	TWeakObjectPtr<UUC_Popup_BagExtend> UC_Popup_Bag_Extend = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	TWeakObjectPtr<UUC_Popup_Item_Sell> UC_Popup_Item_Sell = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	TWeakObjectPtr<UUC_Popup_Item_Grind> UC_Popup_Item_Grind = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UUC_Popup_HeroInfo_Lock* Popup_HeroInfo_Lock = nullptr;

	
	//// Delegates

	UFUNCTION()
	void OnButtonSelect();
	UFUNCTION()
	void OnButtonAllSelect();

	UFUNCTION()
	void OnButtonBatchOperation();

	UFUNCTION()
	void OnButtonBatchCancelClicked();
	UFUNCTION()
	void OnButtonBatchSellClicked();
	UFUNCTION()
	void OnButtonBatchGrindClicked();
	UFUNCTION()
	void OnButtonBatchLockClicked();
	UFUNCTION()
	void OnButtonBagAllSelectClicked();

	UFUNCTION()
	void OnInventoryItemHeroLockRp(bool bSuccess);

	UFUNCTION()
	void OnButtonBagExtendClicked();
	UFUNCTION()
	void OnButtonSellClicked();

	UFUNCTION()
		void BatchButtonControl(bool enabled);

	//// ItemInventoryBase interface
	void OnTabCheckStateChanged(bool bIsChecked, int32 Index) override;
	void OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, UObject* WidgetReturnedFrom);
	void OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom) override;

protected:
	void UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData) override;
	void UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData) override;

	void OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem) override;
	void OnInventoryScrollViewSelectedItem_WalletItem(UUC_ItemInventory_ScrollItem* SelectedItem) override;

private:
	void EnterBatchMode();
	/** handles execution or cancellation of batch operation */
	void ExitBatchMode();

	void EnterSellMode();
	void ExitSellMode();

	//// Invalidate helpers
	void InvalidateBagCapacity();

	void OpenGrindConfirmPopup();
	void OpenSellConfirmPopup();

	//// Batch Operation state
	EItemInventoryBatchOperation CurrentBatchOperation = EItemInventoryBatchOperation::Grind;

	// saved
	FSlateColor Color_BagCapacityCurrentCountNormal;
};


/**
* Common things over UUC_ItemSellConfirm and UUC_WalletItemSell because the UIs look basically the same.
*/
UCLASS()
class PROJECTR_API UUC_ItemBatchActionConfirm : public UUC_GenericPopupContent
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	TArray<FString> SelectedItemUDs;
	// Below only used for ItemSell
	TArray<FName> SelectedWalletKeys;
	TArray<int32> SelectedWalletAmounts;
	FOnItemInventoryUpdated OnItemInventoryUpdated;
	////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_OK;

	UFUNCTION()
	void OnButtonCancelClicked();
	UFUNCTION()
	virtual void OnButtonOKClicked();

};

/**
 * Confirm dialog for selling equipment items
 */
// UCLASS()
// class PROJECTR_API UUC_ItemSellConfirm : public UUC_ItemBatchActionConfirm
// {
// 	GENERATED_BODY()
// public:
// 
// 	void OnButtonOKClicked() override;
// 
// 	UFUNCTION()
// 	void OnInventorySellRp(bool bSuccess, TArray<FREWARD> Rewards);
// };

/**
* Confirm dialog for grinding equipment items
*/
// UCLASS()
// class PROJECTR_API UUC_ItemGrindConfirm : public UUC_ItemBatchActionConfirm
// {
// 	GENERATED_BODY()
// public:
// 
// 	void OnButtonOKClicked() override;
// 	
// 	UFUNCTION()
// 	void OnInventoryGrindRp(bool bSuccess, TArray<FREWARD> Rewards);
// };


/**
 * For allowing user to select an amount of a wallet-type item she owns for selling
 */
UCLASS()
class PROJECTR_API UUC_WalletItemSell : public UUC_GenericPopupContent
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	FName WalletKey = NAME_None;

	int32 SelectedAmount = 0;
	int32 MaxAmount = 0;

	void InvalidateData() override;

	//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnComplete, bool /* bCanceled */, int32 /* amount selected */);
	//FOnComplete OnComplete;

	FOnItemInventoryUpdated OnItemInventoryUpdatedDelegate;

	//// BP Widgets
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	//ULocalizingTextBlock* Text_Description;

	// tentative. Doesn't display HeroPieces properly.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UUC_GenericItemIcon* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UTextBlock* Text_SelectedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UTextBlock* Text_OwnedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UTextBlock* Text_GoldRewardAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UUC_AmountSelectSlider* AmountSelectSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UButton* Button_OK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UButton* Button_Cancel;

	//// Delegates
	
	UFUNCTION()
	void OnButtonCancelClicked();
// 	UFUNCTION()
// 	void OnButtonOKClicked();

	UFUNCTION()
	void OnCountChanged(int32 NewCount);
	
	UFUNCTION()
	//void OnInventorySellRp(bool bSuccess, TArray<FREWARD> Rewards);

	void OnItemInventoryUpdated(EItemInventoryUpdate Result, bool WidgetFinished);

private:
	// Invalidate helper
	void InvalidateSelection();

	// saved
	int32 SellPrice = 0;
};

/**
* STUB
*/
UCLASS()
class PROJECTR_API UUC_BagExtend : public UUC_GenericPopupContent
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void InvalidateData() override;

	FOnItemInventoryUpdated OnItemInventoryUpdated;

	//// BP Config

	/** "블루 스톤 {bluestone}개를 소모하여 가방 칸 {slot}을 확장할 수 있습니다." */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	FOptionalUIStringKey DescriptionText;

	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UTextBlock* Text_Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UTextBlock* Text_BagCapacityBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UTextBlock* Text_BagCapacityAfter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UTextBlock* Text_BluestoneCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
	UButton* Button_OK;

	//// Delegates
	UFUNCTION()
	void OnButtonCancelClicked();
	UFUNCTION()
	void OnButtonOKClicked();

	void OnInventoryUpgradeRp(bool bSuccess);

private:
	bool HasEnoughBluestones();
};
