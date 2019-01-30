// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemInventoryFilter.h"
#include "ItemFilter.h"

#include "UC_GenericPopup.h"
#include "UtilFunctionIntegrated.h"

void UUC_PopupContent_ItemInventoryFilter::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(ParentPopup))
		ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_PopupContent_ItemInventoryFilter::OnButtonCancelClicked);

	if(IsValid(Button_Cancel))
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_PopupContent_ItemInventoryFilter::OnButtonCancelClicked);
	if (IsValid(Button_Apply))
		Button_Apply->OnClicked.AddDynamic(this, &UUC_PopupContent_ItemInventoryFilter::OnButtonApplyClicked);
	if (IsValid(Button_Reset))
		Button_Reset->OnClicked.AddDynamic(this, &UUC_PopupContent_ItemInventoryFilter::OnButtonResetClicked);
}

void UUC_PopupContent_ItemInventoryFilter::NativeDestruct()
{
	if (IsValid(Button_Reset))
		Button_Reset->OnClicked.RemoveAll(this);
	if (IsValid(Button_Apply))
		Button_Apply->OnClicked.RemoveAll(this);
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	
	if(IsValid(ParentPopup))
		ParentPopup->Button_Close->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_PopupContent_ItemInventoryFilter::InvalidateData()
{
	Super::InvalidateData();

	if (Filter->Rows.Num() > Categories.Num())
	{
		UE_LOG(LogUI, Warning, TEXT("UUC_PopupContent_ItemInventoryFilter::InvalidateData: there are more filter rows than the UI has"));
	}

	for (UUC_ItemInventoryFilter_Category* Row : Categories)
	{
		Row->SetVisibility(ESlateVisibility::Collapsed);
	}

	// Set up categories and factor labels and checked states
	int32 CategoryUIIndex = 0;
	int32 FilterRowIndex = 0;
	while (FilterRowIndex < Filter->Rows.Num())
	{
		if (CategoryUIIndex >= Categories.Num())
			break;

		int32 NumRows = 0;
		int32 NextFilterRowIndex = GetNextCategoryRowIndex(FilterRowIndex, Filter->Rows, NumRows);

		Categories[CategoryUIIndex]->Refresh(FilterRowIndex, NumRows, Filter->Rows, Filter->CheckedFactors);
		Categories[CategoryUIIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		CategoryUIIndex++;

		FilterRowIndex = NextFilterRowIndex;
	}
}

void UUC_PopupContent_ItemInventoryFilter::OnButtonCancelClicked()
{
	OnReturnToItemInventoryDelegate.ExecuteIfBound(EItemInventoryUpdate::NoChange, true);
	SetVisibility(ESlateVisibility::Hidden);
}

void UUC_PopupContent_ItemInventoryFilter::OnButtonApplyClicked()
{
	// 임시 코드
	SetVisibility(ESlateVisibility::Hidden);
	return;

	Filter->CheckedFactors.Empty();
	
	int32 Index = 0;
	int32 UIIndex = 0;
	while (Index < Filter->Rows.Num())
	{
		if (UIIndex >= Categories.Num())
			break;

		int32 NumRows = 0;
		int32 NextIndex = GetNextCategoryRowIndex(Index, Filter->Rows, NumRows);

		UUC_ItemInventoryFilter_Category* UICategory = Categories[UIIndex];

		UICategory->AddUncheckedFactors(Index, NumRows, Filter->Rows, Filter->CheckedFactors);

		UIIndex++;
		Index = NextIndex;
	}

	OnReturnToItemInventoryDelegate.ExecuteIfBound(EItemInventoryUpdate::FilterChanged, true);
}

void UUC_PopupContent_ItemInventoryFilter::OnButtonResetClicked()
{
	// 임시 코드
	SetVisibility(ESlateVisibility::Hidden);
	return;
	// set all check boxes to 'checked'
	for (UUC_ItemInventoryFilter_Category* Row : Categories)
	{
		Row->Reset();
	}
}

int32 UUC_PopupContent_ItemInventoryFilter::GetNextCategoryRowIndex(int32 CurrentIndex, const TArray<FItemFilterRow>& FilterRows, int32& OutNumRows)
{
	FName Category = FilterRows[CurrentIndex].Category;
	check(!Category.IsNone());

	int32 Index = CurrentIndex + 1;
	for (; Index < FilterRows.Num(); Index++)
	{
		if (FilterRows[Index].Category != Category)
			break;
	}

	OutNumRows = Index - CurrentIndex;

	return Index;
}

void UUC_ItemInventoryFilter_FactorsRow::Refresh(const FItemFilterRow& FilterRow, const TArray<FItemFilterFactor>& UncheckedFactors, bool bHideLabel)
{
	if (bHideLabel)
	{
		Text_CategoryLabel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Text_CategoryLabel->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (FilterRow.Category_UIStringKey.IsNone())
		{
			Text_CategoryLabel->SetText(FText::FromString(FilterRow.Category.ToString()));
		}
		else
		{
			Text_CategoryLabel->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FilterRow.Category_UIStringKey));
		}
	}

	if (FilterRow.Factors.Num() > FactorLabels.Num())
	{
		UE_LOG(LogUI, Warning, TEXT("UUC_ItemInventoryFilter_Row::Refresh: there are more factors than the row UI has"));
	}

	for (int32 FactorWidgetIndex = 0; FactorWidgetIndex < FactorLabels.Num(); FactorWidgetIndex++)
	{
		if (FactorWidgetIndex >= FilterRow.Factors.Num())
		{
			FactorCheckBoxes[FactorWidgetIndex]->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			FactorCheckBoxes[FactorWidgetIndex]->SetVisibility(ESlateVisibility::Visible);

			const FString& Factor = FilterRow.Factors[FactorWidgetIndex];

			if (FilterRow.FactorUIStrings[FactorWidgetIndex].IsNone())
			{
				FactorLabels[FactorWidgetIndex]->SetText(FText::FromString(Factor));
			}
			else
			{
				FactorLabels[FactorWidgetIndex]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FilterRow.FactorUIStrings[FactorWidgetIndex]));
			}

			bool bChecked = false;
			for (const FItemFilterFactor& UncheckedFactor : UncheckedFactors)
			{
				if (UncheckedFactor.Category == FilterRow.Category && UncheckedFactor.Factor == Factor)
				{
					bChecked = true;
					break;
				}
			}

			FactorCheckBoxes[FactorWidgetIndex]->SetCheckedState(bChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		}
	}
}

void UUC_ItemInventoryFilter_Category::Refresh(int32 FilterRowIndex, int32 NumRows, const TArray<FItemFilterRow>& FilterRows, const TArray<FItemFilterFactor>& UncheckedFactors)
{
	for (UUC_ItemInventoryFilter_FactorsRow* FactorsRow : FactorsRows)
	{
		FactorsRow->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 Index = 0; Index < NumRows; Index++)
	{
		if (Index >= FactorsRows.Num())
			break;

		FactorsRows[Index]->Refresh(FilterRows[Index + FilterRowIndex], UncheckedFactors, Index != 0);
		FactorsRows[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUC_ItemInventoryFilter_Category::Reset()
{
	for (UUC_ItemInventoryFilter_FactorsRow* FactorsRow : FactorsRows)
	{
		FactorsRow->Reset();
	}
}

void UUC_ItemInventoryFilter_Category::AddUncheckedFactors(int32 FilterRowIndex, int32 NumRows, const TArray<FItemFilterRow>& FilterRows, TArray<FItemFilterFactor>& InOutUncheckedFactors)
{
	for (int32 Index = 0; Index < NumRows; Index++)
	{
		if (Index >= FactorsRows.Num())
			break;

		FactorsRows[Index]->AddUncheckedFactors(FilterRows[Index + FilterRowIndex], InOutUncheckedFactors);
	}
}

void UUC_ItemInventoryFilter_FactorsRow::Reset()
{
	for (UCheckBox* FactorCheckBox : FactorCheckBoxes)
	{
		FactorCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UUC_ItemInventoryFilter_FactorsRow::AddUncheckedFactors(const FItemFilterRow& FilterRow, TArray<FItemFilterFactor>& InOutUncheckedFactors)
{
	for (int32 Index = 0; Index < FilterRow.Factors.Num(); Index++)
	{
		if (Index >= FactorCheckBoxes.Num())
			break;

		if (FactorCheckBoxes[Index]->GetCheckedState() == ECheckBoxState::Checked)
		{
			FItemFilterFactor FilterFactor;
			FilterFactor.Category = FilterRow.Category;
			FilterFactor.Factor = FilterRow.Factors[Index];
			InOutUncheckedFactors.Add(FilterFactor);
		}
	}
}
