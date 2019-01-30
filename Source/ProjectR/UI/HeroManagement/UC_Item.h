// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"

#include "UI/Common/UC_Item_Icons.h"
#include "UC_Item.generated.h"

class URGameInstance;
class UUC_Item_Icons;

/**
 * BP: UC_Item
 * embedded in UP_Mediator_HeroManagement (BP: UP_Hero_Manager)
 *
 * represents currently equipped items by EquipPosition
 */
UCLASS()
class PROJECTR_API UUC_Item : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
	void OnWidgetRebuilt() override;

	void InvalidateByHeroUD(const FString& InHeroUD);
	//void SetClearEquipPosition();

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
	bool GetItemIconInfoByEquipPosition(EItemEquipPositionEnum InEquipPosition, FITEM_ICON_INFO& InTargetItemIconInfo);
	FString GetItemUDByEquipPosition(EItemEquipPositionEnum InEquipPosition);
	EItemEquipPositionEnum GetCurrentSelectItemPosition() { return CurrentItemEquipPosition; }

	/************************************************************************/
	/* UMG Button Event                                                     */
	/************************************************************************/
	UFUNCTION(BlueprintCallable, Category = UC_Item)
	void OnClick_Item(EItemEquipPositionEnum InItemEquipPosition, bool bEnableClose = true);

	UFUNCTION(BlueprintCallable, Category = UC_Item)
	void OnClick_AutoItemEquip();

	UFUNCTION()
	void ClearSelected();

public:

	// Expect Blueprint to fill this in in index EItemEquipPositionEnum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	TArray<UUC_Item_Icons*> ItemIcons;

	// Expect Blueprint to fill this in in index EItemEquipPositionEnum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	TArray<UButton*> ItemButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	UButton*				Button_AutoEquip = nullptr;

	UPROPERTY()
	TArray<FITEM_ICON_INFO>	ItemIconInfoArray;

	FITEM_ICON_INFO			EquippedItemIconInfo;

	FITEM_ICON_INFO			SelectedItemIconInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	UButton*				WeaponItem_Btn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	UButton*				Armor_Btn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	UButton*				NecklaceItem_Btn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	UButton*				RingItem_Btn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_Item|Ref")
	UButton*				Button_Awaken = nullptr;
	
private:
	void Init();

	void InvalidateItemIconInfo();

	void Refresh();

	UFUNCTION()
	void OnClick_Weapon();

	UFUNCTION()
	void OnClick_Armor();

	UFUNCTION()
	void OnClick_Necklace();

	UFUNCTION()
	void OnClick_Ring();

	UFUNCTION()
	void OnClick_Awaken();

private:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	UPROPERTY()
	FString					CurrentHeroUD;

	UPROPERTY()
	EItemEquipPositionEnum	CurrentItemEquipPosition = EItemEquipPositionEnum::VE_Weapon;
};
