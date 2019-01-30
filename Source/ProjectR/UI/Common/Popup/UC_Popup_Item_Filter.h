// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_Popup_Item_Filter.generated.h"

class UUC_Popup_Item_Filter_Weapon;
class UUC_Popup_Item_Filter_Armor;
class UUC_Popup_Item_Filter_Accessories;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_Item_Filter_Total : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void CheckBoxReset(int32 tab);

	UFUNCTION()
		void Close();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter_Total)
		UWidgetSwitcher* Filter_Switcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter_Total)
		UUC_Popup_Item_Filter_Weapon* UC_Popup_Item_Filter_Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter_Total)
		UUC_Popup_Item_Filter_Armor* UC_Popup_Item_Filter_Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter_Total)
		UUC_Popup_Item_Filter_Accessories* UC_Popup_Item_Filter_Accessories;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_CloseFilter, bool, IsValid);
	FDelegate_CloseFilter CloseFilter;
};

UCLASS()
class PROJECTR_API UUC_Popup_Item_Filter : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void CheckBoxReset(TArray<UCheckBox*> list);

	UFUNCTION()
		void FilteringAllConditions(bool isChecked);

//property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UButton*						Button_Reset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UButton*						Button_Close;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Rare;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Epic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Unique;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Tower;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Using;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Locking;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_T1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_T2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_T3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_T4;
	
};

UCLASS()
class PROJECTR_API UUC_Popup_Item_Filter_Weapon : public UUC_Popup_Item_Filter
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Reset();
	UFUNCTION()
		void OnClickButton_Close();
	UFUNCTION()
		void OnClickButton_Reset();

	UFUNCTION()
		void FilteringAllConditions(bool isChecked);

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Tanker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_MeleeDealer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_RangeDealer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_MagicDealer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Supporter;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Rune_Equiped;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_NoEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_HaveEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_MaxEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_BlcokEnchant;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Sword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Dagger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Mace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Spear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Bow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_CrossBow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Gun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Staff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Orb;

	TArray<UCheckBox*> CheckBoxList;
};

UCLASS()
class PROJECTR_API UUC_Popup_Item_Filter_Armor : public UUC_Popup_Item_Filter
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Reset();

	UFUNCTION()
		void OnClickButton_Close();
	UFUNCTION()
		void OnClickButton_Reset();
	UFUNCTION()
		void FilteringAllConditions(bool isChecked);

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Rune_Equiped;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_NoEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_HaveEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_MaxEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_BlcokEnchant;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_HeavyArmorTop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_LightArmorTop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_ClothesTop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_HeavyArmorPants;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_LightArmorPants;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_ClothesPants;

	TArray<UCheckBox*> CheckBoxList;
};

UCLASS()
class PROJECTR_API UUC_Popup_Item_Filter_Accessories : public UUC_Popup_Item_Filter
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Reset();

	UFUNCTION()
		void OnClickButton_Close();
	UFUNCTION()
		void OnClickButton_Reset();
	UFUNCTION()
		void FilteringAllConditions(bool isChecked);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Necklace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Filter)
		UCheckBox*						CheckBox_Ring;

	TArray<UCheckBox*> CheckBoxList;
};