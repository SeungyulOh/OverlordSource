// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataEnum.h"
#include "UC_CurrencyDisplay.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_CurrencyDisplay : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	void OnlyTextDisplay();
//	void InvalidateQuantity();

	UFUNCTION(BlueprintCallable, Category = UC_CurrencyDisplay)
	void SetCurrency(FName InCurrencyType, float InQuantity, bool bInCash = false, bool bInGuildGold = false);

	UFUNCTION(BlueprintCallable, Category = UC_CurrencyDisplay)
	void SetQuantity(float InQuantity);

	UFUNCTION(BlueprintCallable, Category = UC_CurrencyDisplay)
	bool IsHaveCurrency();

	UFUNCTION(BlueprintCallable, Category = UC_CurrencyDisplay)
	void ShowMessageBox();

	void SetItemImage(UObject* InBrushResource);

private:
	int32	GetIconSwitchIndex(FName InCurrencyType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_CurrencyDisplay)
	FName		CurrentCurrencyType = WALLET_GOLD;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_CurrencyDisplay)
	UWidgetSwitcher*	IconType = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_CurrencyDisplay)
	UTextBlock*			Text_Quantity = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_CurrencyDisplay)
	UImage*				Image_Item = nullptr;

private:
	FName			CurCurrencyType = WALLET_GOLD;
	float			CurValue = 0;
	bool			bGuildGold = false;
	bool			bCash = false;
};
