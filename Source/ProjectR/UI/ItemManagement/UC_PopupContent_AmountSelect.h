// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UI/ItemManagement/UC_GenericPopupContent.h"
#include "UC_PopupContent_AmountSelect.generated.h"


class UUC_GenericItemIcon;

/**
 * For allowing user to select an amount of a wallet-type item she owns for selling
 */
UCLASS()
class PROJECTR_API UUC_PopupContent_AmountSelect : public UUC_GenericPopupContent
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	FName WalletKey = NAME_None;

	int32 SelectedAmount = 0;
	int32 MaxAmount = 0;

	void InvalidateData() override;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnComplete, bool /* bCanceled */, int32 /* amount selected */);
	FOnComplete OnComplete;

	//// BP Widgets
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	//ULocalizingTextBlock* Text_Description;

	// tentative. Doesn't display HeroPieces properly.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UUC_GenericItemIcon* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UTextBlock* Text_SelectedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UTextBlock* Text_OwnedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UTextBlock* Text_GoldRewardAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	USlider* Slider_SelectedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UButton* Button_Slider_Minus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UButton* Button_Slider_Plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UButton* Button_Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UButton* Button_OK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_AmountSelect)
	UButton* Button_Cancel;

	//// Delegates
	
	UFUNCTION()
	void OnButtonCancelClicked();
	UFUNCTION()
	void OnButtonOKClicked();
	UFUNCTION()
	void OnButtonDecreaseClicked();
	UFUNCTION()
	void OnButtonIncreaseClicked();
	UFUNCTION()
	void OnButtonMaxClicked();

	UFUNCTION()
	void OnSliderSelectedAmountValueChanged(float Value);

private:
	// Invalidate helper
	void InvalidateSelection();
	void InvalidateSlider();

	// saved
	int32 SellPrice = 0;
};
