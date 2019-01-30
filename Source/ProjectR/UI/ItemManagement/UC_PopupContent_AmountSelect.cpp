// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_PopupContent_AmountSelect.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "UC_GenericPopup.h"
#include "UtilFunctionIntegrated.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "Table/ItemTableInfo.h"

#include "UI/Common/UC_ItemIcon.h"

void UUC_PopupContent_AmountSelect::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ParentPopup))
	{
		ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_PopupContent_AmountSelect::OnButtonCancelClicked);
	}

	// hook up delegates
	Button_Cancel->OnClicked.AddDynamic(this, &UUC_PopupContent_AmountSelect::OnButtonCancelClicked);
	Button_OK->OnClicked.AddDynamic(this, &UUC_PopupContent_AmountSelect::OnButtonOKClicked);
	Button_Slider_Minus->OnClicked.AddDynamic(this, &UUC_PopupContent_AmountSelect::OnButtonDecreaseClicked);
	Button_Slider_Plus->OnClicked.AddDynamic(this, &UUC_PopupContent_AmountSelect::OnButtonIncreaseClicked);
	Button_Max->OnClicked.AddDynamic(this, &UUC_PopupContent_AmountSelect::OnButtonMaxClicked);

	Slider_SelectedAmount->OnValueChanged.AddDynamic(this, &UUC_PopupContent_AmountSelect::OnSliderSelectedAmountValueChanged);
}

void UUC_PopupContent_AmountSelect::NativeDestruct()
{
	Slider_SelectedAmount->OnValueChanged.RemoveAll(this);

	Button_Max->OnClicked.RemoveAll(this);
	Button_Cancel->OnClicked.RemoveAll(this);
	Button_OK->OnClicked.RemoveAll(this);
	Button_Slider_Minus->OnClicked.RemoveAll(this);
	Button_Slider_Plus->OnClicked.RemoveAll(this);

	if (IsValid(ParentPopup))
	{
		ParentPopup->Button_Close->OnClicked.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UUC_PopupContent_AmountSelect::InvalidateData()
{
	Super::InvalidateData();

	FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);
	check(WalletInfo != nullptr);

	SellPrice = WalletInfo->SellPrice;

	// CurrencyIcon
	int32 OwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WalletKey);
	ItemIcon->RefreshWalletIcon(WalletKey, WalletInfo, OwnedAmount);

	Text_OwnedAmount->SetText(FText::AsNumber(OwnedAmount));
	
	// Selection
	InvalidateSelection();
	InvalidateSlider();
}

void UUC_PopupContent_AmountSelect::OnButtonCancelClicked()
{
	OnComplete.Broadcast(true, 0);
}

void UUC_PopupContent_AmountSelect::OnButtonOKClicked()
{
	OnComplete.Broadcast(false, SelectedAmount);
}

void UUC_PopupContent_AmountSelect::OnButtonDecreaseClicked()
{
	if (SelectedAmount > 0)
	{
		SelectedAmount -= 1;
		InvalidateSelection();
		InvalidateSlider();
	}
}

void UUC_PopupContent_AmountSelect::OnButtonIncreaseClicked()
{
	if (SelectedAmount < MaxAmount)
	{
		SelectedAmount += 1;
		InvalidateSelection();
		InvalidateSlider();
	}
}

void UUC_PopupContent_AmountSelect::OnButtonMaxClicked()
{
	SelectedAmount = MaxAmount;

	InvalidateSelection();
	InvalidateSlider();
}

void UUC_PopupContent_AmountSelect::OnSliderSelectedAmountValueChanged(float Value)
{
	SelectedAmount = FMath::RoundToInt((float)MaxAmount * Value);

	InvalidateSelection();
	InvalidateSlider();
}

void UUC_PopupContent_AmountSelect::InvalidateSelection()
{
	Text_SelectedAmount->SetText(FText::AsNumber(SelectedAmount));
	Text_GoldRewardAmount->SetText(FText::AsNumber(SelectedAmount * SellPrice));
}

void UUC_PopupContent_AmountSelect::InvalidateSlider()
{
	Slider_SelectedAmount->SetValue((float)SelectedAmount/MaxAmount);
}
