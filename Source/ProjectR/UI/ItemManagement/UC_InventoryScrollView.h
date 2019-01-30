// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollView.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_InventoryScrollView.generated.h"

UENUM(BlueprintType)
enum class EInventorySortTypeEnum : uint8
{
	VE_Descending			UMETA(DisplayName = "Descending"),
	VE_Ascending			UMETA(DisplayName = "Ascending"),
};

UENUM(BlueprintType)
enum class EInventorySortStandardTypeEnum : uint8
{
	VE_Base					UMETA(DisplayName = "Base"),
	VE_Grade				UMETA(DisplayName = "Grade"),
	VE_Enchant				UMETA(DisplayName = "Enchant"),
	VE_ItemType				UMETA(DisplayName = "ItemType"),
	VE_Level				UMETA(DisplayName = "Level"),
};


/**
 * BP: UC_InventoryScrollView
 *
 * Mind that all the function names are either unhelpful or misleading, if grammatically decipherable
 */
UCLASS()
class PROJECTR_API UUC_InventoryScrollView : public URScrollView
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;

	// GetItemEquipmentKeys and sort them
	void InventoryDataReComposition(FString itemUD = TEXT(""), FString heroUD = TEXT(""), int32 equipPosition = -1);
	void InventoryDataReCompositionNew(int32 equipPosition = -1);

	// Select the item and bring it to the first position & scroll?
	void SelectItemUI(FString itemUD);
	// Select item
	void SelectItem(FString itemUD);

	// Set the selection to these items
	void SelectItems(TArray<FString>& itemUDs);

	void SelectItemByItemUD(const FString& InItemUD);
	void UpdateTargetItem(const FString& InItemKey);

	
	virtual void	SetSelected(URScrollItem* InSelecteItem) override;
	
	
	void			AddDisAbleItem(FString ud);
	void			ClearDisAbleItem();

	FORCEINLINE void SetItemType(EItemTypeEnum Type) { ItemType = Type; }
	FORCEINLINE void SetItemEquipPosition(EItemEquipPositionEnum Position) { ItemEquipPosition = Position; }
	FORCEINLINE void SetItemEquipType(EItemEquipTypeEnum Type) { ItemEquipType = Type; }

	FORCEINLINE int32			GetLimitSelectCount() { return LimitSelectCount; }
	FORCEINLINE void			SetLimitSelectCount(int32 Count) { LimitSelectCount = Count; }

	FORCEINLINE EItemInventoryTypeEnum GetInventoryType() { return InventoryType; }

private:
	float			GetItemValue(FName tableKey, int32 enchantLevel, int32 InItemLevel = 1);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ItemInventory)
	EItemInventoryTypeEnum	InventoryType = EItemInventoryTypeEnum::VE_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ItemInventory)
	EItemTypeEnum ItemType = EItemTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ItemInventory)
	EItemEquipPositionEnum ItemEquipPosition = EItemEquipPositionEnum::VE_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ItemInventory)
	EItemEquipTypeEnum ItemEquipType = EItemEquipTypeEnum::VE_None;


	//// Sorting

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ItemInventory)
	EInventorySortTypeEnum	InventorySortType = EInventorySortTypeEnum::VE_Descending;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ItemInventory)
	EInventorySortStandardTypeEnum InventorySortStandardType = EInventorySortStandardTypeEnum::VE_Base;

private:
	URScrollItem* FindItemWithItemUD(const FString& InItemUD);
	
private:
	// The maximum number of items that can be selected in case of MultiSelect
	int32				LimitSelectCount = -1;

	UPROPERTY()
	TArray<FString>		DisableItemList;
public:
	bool				bSkipSelectProcess = false;
	bool				bisTesting = false;
};
