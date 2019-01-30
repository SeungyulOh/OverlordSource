// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "UI/RBaseWidget.h"

#include "UP_ItemShortagePopup.generated.h"


class ULocalizingTextBlock;
class UUC_Item_Icons;
class UTextBlock;
class UUC_CurrencyDisplay;

/**
 * BP: UP_Popup_Item_Shortage
 * WBP_Popup_ItemShortage
 */

UCLASS()
class PROJECTR_API UUP_ItemShortagePopup : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
		
	UFUNCTION(BlueprintCallable, Category = UP_ItemShortagePopup)
	void						OnClickOK();

	UFUNCTION(BlueprintCallable, Category = UP_ItemShortagePopup)
	void						OnClickShop();

	UFUNCTION(BlueprintCallable, Category = UP_ItemShortagePopup)
	void						SetNecessaryGold(int32 InGold);

	UFUNCTION(BlueprintCallable, Category = UP_ItemShortagePopup)
	void						SetNecessaryGem(int32 InGold);

	UFUNCTION(BlueprintCallable, Category = UP_ItemShortagePopup)
	void						SetCurrency(EREWARD_TYPE InType, int32 InValue);

	void SetChangeTextBlock(FName InCaption, FName InDesc);
	void SetItem(UObject* InBrushResource, int32 InValue);

	void SetIsClickCustom(bool InValue) { IsClickCustom = InValue; }

	void SetPopup(FText InCaption, FText InDesc, EREWARD_TYPE InType, int32 InValue);
	void SetHideCurrency();
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ItemShortagePopup)
	ULocalizingTextBlock*	LocalText_Caption = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ItemShortagePopup)
	ULocalizingTextBlock*	LocalText_Desc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemShortagePopup)
	UTextBlock*				Text_NecessaryGold = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemShortagePopup)
	UUC_CurrencyDisplay*	LocalCurrency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemShortagePopup|Ref")
	UButton*				Button_Store;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemShortagePopup|Ref")
	UButton*				Button_OK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemShortagePopup|Ref")
	UButton*				Button_Close;

private:
	//int32					NecessaryGold = 0;
	
	bool					IsClickCustom = false;
	bool					IsHideCurrency = false;

	EREWARD_TYPE			RewardType;
};	
