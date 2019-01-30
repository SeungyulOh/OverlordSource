// Fill out your copyright notice in the Description page of Project Settings.

/**
*/

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UC_ItemInventoryBase.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UP_ItemInventoryPage.generated.h"

class UUP_ItemInventoryPage;

/**
 * item operation content for UP_ItemInventoryPage. Expected content are:
 * * ItemEnchant
 * * ItemLevelUp
 * * ItemUpgrade
 * * ItemOptionReset
 * * RuneManagement
 */
UCLASS()
class PROJECTR_API UUP_ItemInventoryPage_ContentBase : public URBaseWidget
{
	GENERATED_BODY()
public:
	// override these

	virtual void SetItemUD(FString InItemUD) {};
	virtual void SetWalletKey(FName InWalletKey) {};

	virtual EItemInventoryEnum GetDefaultTab() { return EItemInventoryEnum::Weapons; };

	virtual bool IsInventoryIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData) { return true; };

	virtual void OnInventoryEquipmentItemSelected(FString SelectedItemUD) {};
	virtual void OnInventoryWalletItemSelected(FName SelectedWalletKey) {};

	virtual void UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData) {};
	virtual void UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData) {};

	// InvalidateData

	// helper
	UFUNCTION()
	void ClosePage();

	UPROPERTY()
	UUP_ItemInventoryPage* ItemInventoryPage = nullptr;
};

/**
 * A UI page with an item operation content panel on the left side and item inventory ScrollView on the right.
 * lets the item content widget to control the item inventory - which items are shown and stuff
 */
UCLASS()
class PROJECTR_API UUP_ItemInventoryPage : public UUC_ItemInventoryBase
{
	GENERATED_BODY()
	
public:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	//// BP Config
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	TSubclassOf<UUserWidget> ContentWidgetClass;

	//// Interface

	void InvalidateData() override;

	UUserWidget* GetContentWidget();

	//// ItemInventoryBase interface

	/*
	* Tabs come in order for now
	* * Weapons
	* * Defense
	* * Accessories
	*/
protected:
	EItemInventoryEnum TabIndexToInventoryEnum(int32 Index) override;

	bool IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData) override;

	void OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem) override;
	void OnInventoryScrollViewSelectedItem_WalletItem(UUC_ItemInventory_ScrollItem* SelectedItem) override;

	void UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData) override;
	void UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData) override;

	// invalidate helper
	void InvalidateContentWidget();

	/** nullable */
	UUP_ItemInventoryPage_ContentBase* GetPageContent();
	
	UPROPERTY()
		UUserWidget* ContentWidget = nullptr;
};

// temp for rune operation pages, only has Runes tab
UCLASS()
class PROJECTR_API UUP_RuneInventoryPage : public UUP_ItemInventoryPage
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	EItemInventoryEnum TabIndexToInventoryEnum(int32 Index) override;

};
