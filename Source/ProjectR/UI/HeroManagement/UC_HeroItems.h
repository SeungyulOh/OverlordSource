// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UI/RBaseWidget.h"

#include "UC_HeroItems.generated.h"

class URGameInstance;
class UUC_HeroItems_Icon;
class UButton;

/**
 * NewDesign
 * embedded in UP_Mediator_HeroManagement (BP: UP_Hero_Manager)
 *
 * represents currently equipped items by EquipPosition
 * interfacing with legacy Mediator.
 * OnClickItem_ItemEquipPosition_HeroManagement
 * OnClick_AutoEquipItem_HeroManagement
 *
 * need to be updated on item inventory update, by UP_Mediator_HeroManagement
 */
UCLASS()
class PROJECTR_API UUC_HeroItems : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void InvalidateData() override;

	void SetHeroUD(FString InHeroUD) { CurrentHeroUD = InHeroUD; }
	FString GetHeroUD() { return CurrentHeroUD; }

	void Reset();

	//// Events for UP_Mediator_HeroManagement

	// OnClickItem_ItemEquipPosition_HeroManagement
	// OnClick_AutoEquipItem_HeroManagement

	DECLARE_DELEGATE_OneParam(FOnEquipmentItemClicked, FString /* ItemUD */);
	FOnEquipmentItemClicked OnEquipmentItemClicked;
	
	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
	UButton*				Button_AutoEquip = nullptr;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
	UUC_HeroItems_Icon*				Icon_Weapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
	UUC_HeroItems_Icon*				Icon_Armor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
	UUC_HeroItems_Icon*				Icon_Pants = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
	UUC_HeroItems_Icon*				Icon_Necklace = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
	UUC_HeroItems_Icon*				Icon_Ring = nullptr;

 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
 	UButton*				Button_Weapon = nullptr;
 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
 	UButton*				Button_Armor = nullptr;
 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
 	UButton*				Button_Pants = nullptr;
 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
 	UButton*				Button_Necklace = nullptr;
 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems|Ref")
 	UButton*				Button_Ring = nullptr;
	
	// Thank you for dynamic delegates
	UFUNCTION()
	void OnButtonWeaponClicked();
	UFUNCTION()
	void OnButtonArmorClicked();
	UFUNCTION()
	void OnButtonPantsClicked();
	UFUNCTION()
	void OnButtonNecklaceClicked();
	UFUNCTION()
	void OnButtonRingClicked();

private:
	// empty ItemUD to mean no icon at the EquipPosition
	void InvalidateItemIcon(int32 Index, FString ItemUD);

	// indexed by EItemEquipPosition with 0 at VE_Weapon
	UPROPERTY()
	TArray<UUC_HeroItems_Icon*> ItemIcons;

	UPROPERTY()
	TArray<UButton*> ItemButtons;

	UPROPERTY()
	class UUC_HeroManager_Info* UC_HeroInfo;

	//// Principal state
	FString CurrentHeroUD;


};
