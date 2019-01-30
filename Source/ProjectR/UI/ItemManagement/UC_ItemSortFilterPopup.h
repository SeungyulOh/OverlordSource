// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "UI/RBaseWidget.h"
#include "UI/ItemManagement/UC_InventoryScrollView.h"

#include "UC_ItemSortFilterPopup.generated.h"


class ULocalizingTextBlock;
class UWidgetSwitcher;
class UButton;
class UCanvasPanel_RadioGroup;
class UCheckBox_Radio;


/**
 * BP: UC_Hero_Item_Filter
 * WBP_ItemSortFilterPopup
 */
UCLASS()
class PROJECTR_API UUC_ItemSortFilterPopup : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

public:	
	UFUNCTION(BlueprintCallable, Category = UUC_ItemSortFilterPopup)
	void OnDownRadioChange(bool _ischecked);

	UFUNCTION(BlueprintCallable, Category = UUC_ItemSortFilterPopup)
	void OnUpRadioChange(bool _ischecked);

	UFUNCTION(BlueprintCallable, Category = UUC_ItemSortFilterPopup)
	void OnClick_Ok();

	UFUNCTION(BlueprintCallable, Category = UUC_ItemSortFilterPopup)
	void OnClick_Close();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCanvasPanel_RadioGroup*			UpRadioGroup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCanvasPanel_RadioGroup*			DownRadioGroup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCheckBox_Radio*			CheckBox_descending = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCheckBox_Radio*			CheckBox_ascending = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCheckBox_Radio*			CheckBox_Basic = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCheckBox_Radio*			CheckBox_Grade = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCheckBox_Radio*			CheckBox_EnchantLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UCheckBox_Radio*			CheckBox_ItemType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UButton*			Button_OK = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemSortFilterPopup)
	UButton*			Button_Close = nullptr;

private:
	bool				SortDescType = true;
	int32				SortStandType = 0;
};	
