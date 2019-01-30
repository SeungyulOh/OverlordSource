// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItemData.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "UI/ItemManagement/UC_GenericPopupContent.h"
#include "UC_ItemInventoryBase.h"
#include "ItemFilter.h"
#include "UC_ItemInventoryFilter.generated.h"

class UTextBlock;
class UButton;
class URScrollItem;
class UCheckBox;
class UItemFilterBase;

UCLASS()
class PROJECTR_API UUC_ItemInventoryFilter_FactorsRow : public URBaseWidget
{
	GENERATED_BODY()
public:

	void Refresh(const FItemFilterRow& FilterRow, const TArray<FItemFilterFactor>& UncheckedFactors, bool bHideLabel);

	void Reset();

	void AddUncheckedFactors(const FItemFilterRow& FilterRow, TArray<FItemFilterFactor>& InOutUncheckedFactors);

	//// BP Widgets

	// Assumes the tabs correspond to EItemInventoryEnum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryFilter_Category)
	UTextBlock* Text_CategoryLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryFilter_Category)
	TArray<UCheckBox*> FactorCheckBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryFilter_Category)
	TArray<UTextBlock*> FactorLabels;

};

UCLASS()
class PROJECTR_API UUC_ItemInventoryFilter_Category : public URBaseWidget
{
	GENERATED_BODY()
public:

	//void NativeConstruct() override;
	//void NativeDestruct() override;

	//// Interface
	
	void Refresh(int32 FilterRowIndex, int32 NumRows, const TArray<FItemFilterRow>& FilterRows, const TArray<FItemFilterFactor>& UncheckedFactors);

	void Reset();

	void AddUncheckedFactors(int32 FilterRowIndex, int32 NumRows, const TArray<FItemFilterRow>& FilterRows, TArray<FItemFilterFactor>& InOutUncheckedFactors);

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemInventoryFilter_Category)
	TArray<UUC_ItemInventoryFilter_FactorsRow*> FactorsRows;

};

UCLASS()
class PROJECTR_API UUC_PopupContent_ItemInventoryFilter : public UUC_GenericPopupContent
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	UPROPERTY()
	UItemFilterBase* Filter = nullptr;

	FOnItemInventoryUpdated OnReturnToItemInventoryDelegate;

	void InvalidateData() override;

	//// BP Widgets

	// 5 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemInventoryFilter)
	TArray<UUC_ItemInventoryFilter_Category*> Categories;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemInventoryFilter)
	UButton* Button_Apply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemInventoryFilter)
	UButton* Button_Cancel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemInventoryFilter)
	UButton* Button_Reset;

	//// Delegates
	
	UFUNCTION()
	void OnButtonCancelClicked();
	UFUNCTION()
	void OnButtonApplyClicked();
	UFUNCTION()
	void OnButtonResetClicked();

private:
	// helper
	/** returns FilterRows.Num() if at the end */
	int32 GetNextCategoryRowIndex(int32 CurrentIndex, const TArray<FItemFilterRow>& FilterRows, int32& OutNumRows);
};
