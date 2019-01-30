// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_Donation.generated.h"

class UUC_Item_Icons;
class UUC_CurrencyDisplay;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_Donation : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

	void OpenDonation();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void CloseDonation();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void OnClick_Donation(EGUILD_CONTRIBUTION_TYPE SelectType);

	UFUNCTION(BlueprintImplementableEvent, Category = UUC_Guild_Attendance)
	void BP_RefreshDonation();
	UFUNCTION()
	void OnRefreshGuildDonationUI();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void RefreshDonationType(EGUILD_CONTRIBUTION_TYPE SelectType, UWidgetSwitcher* CompleteSwitcher, UCanvasPanel* CompletePanel,
		UUC_CurrencyDisplay* CurrencyDisplay, UTextBlock* TextPoint, UTextBlock* TextGold, UTextBlock* TextAvatarExp);
//
//	bool IsEnableContribute(EGUILD_CONTRIBUTION_TYPE SelectType);
//
};
