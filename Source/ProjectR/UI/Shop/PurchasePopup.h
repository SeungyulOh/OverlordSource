// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "PurchasePopup.generated.h"


class UUC_Item_Icons;
class UUC_CurrencyDisplay;
class UShopScrollData;

/**
 * 
 */
UCLASS()
class PROJECTR_API UPurchasePopup : public URBaseWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = UShopScrollItem)
	void InvalidateData();
	
public:
		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UPurchasePopup)
	UUC_Item_Icons* Icon_ProductIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UPurchasePopup)
	UTextBlock*		Text_ProductName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UPurchasePopup)
	UTextBlock*		Text_ProductDesc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UPurchasePopup)
	UUC_CurrencyDisplay*	CurrencyDisplay = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UPurchasePopup)
	UShopScrollData*		ProductData;
	
};