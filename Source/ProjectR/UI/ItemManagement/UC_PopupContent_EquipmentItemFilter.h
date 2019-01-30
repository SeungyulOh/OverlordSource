// Fill out your copyright notice in the Description page of Project Settings.

/**
* 아이템 UI_김현주(3).pdf.V.필터
*/

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UC_PopupContent_EquipmentItemFilter.generated.h"

class UCanvasPanel_RadioGroup;
class URScrollView;
class UTextBlock;
class UCheckBox_Radio;
class URScrollItemData;
class UButton;
class URScrollItem;
class URScrollItem;
class UWidgetSwitcher;
class UCheckBox;

/**
 * a checkbox
 */
UCLASS()
class PROJECTR_API UUC_FilterItem : public URBaseWidget
{
	GENERATED_BODY()

public:
	
	void NativePreConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_FilterItem)
	FName LocalizingKey = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_FilterItem)
	EStringTableType StringTableType = EStringTableType::VE_UI;
	
	// Overriding if not empty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_FilterItem)
	FText OverrideText;

	/** WARN: design parameter set from UMG BP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_FilterItem)
	bool CheckedByDefault = false;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_FilterItem)
	UCheckBox* CheckBox_Filter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_FilterItem)
	ULocalizingTextBlock* Text_Filter;

};

/**
 * the popup
 */
UCLASS()
class PROJECTR_API UUC_PopupContent_EquipmentItemFilter : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void InvalidateData() override;

	//// BP Widgets

	// Arrays of FilterItems by category

	/** indexed by (int32)EItemGradeEnum */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_EquipmentItemFilter)
	TArray<UUC_FilterItem*> Category_Grade;
	/** indexed by (int32)EClassTypeEnum */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_EquipmentItemFilter)
	TArray<UUC_FilterItem*> Category_HeroClass;
	/** indexed by (int32)EItemEquipTypeEnum - 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_EquipmentItemFilter)
	TArray<UUC_FilterItem*> Category_EquipType;
	/** 0: enchant level == 0, 1: enchant level >= 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_EquipmentItemFilter)
	TArray<UUC_FilterItem*> Category_ItemEnchant;
	/** 0: rune socketed, 1: locked, 2: all? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_EquipmentItemFilter)
	TArray<UUC_FilterItem*> Category_MiscFilters;
};
