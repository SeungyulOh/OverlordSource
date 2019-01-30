// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "ShopScrollItem.generated.h"


class UUC_Item_Icons;
class UUC_CurrencyDisplay;
class UShopScrollData;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestPurchaseDelegate, FName, InProductID );
/**
 * 
 */
UCLASS()
class PROJECTR_API UShopScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void		NativeConstruct() override;
	void		NativeDestruct() override;
	void		SetSelected(bool bSelected) override;

	UFUNCTION(BlueprintCallable, Category = ShopScrollItem)
	void SetShopScrollItem(URScrollItemData* InOtherData = nullptr );

	UFUNCTION(BlueprintCallable, Category = ShopScrollItem)
	void OnClick_ShowPurchasePopup();

	UFUNCTION(BlueprintCallable, Category = ShopScrollItem)
	FString GetGoogleProductID();
	
	bool IsValidData();
	void Clear();

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ShopScrollItem")
	void OnSetCashInfo();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Event")
	FRequestPurchaseDelegate	RequestPurchaseDelegate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UUC_Item_Icons*			Icon_ProductIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UTextBlock*				Text_ProductName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UTextBlock*				Text_ProductDesc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UTextBlock*				Text_PossiblePurchaseCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UTextBlock*				Text_MaxPurchaseCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UUC_CurrencyDisplay*	CurrencyDisplay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UWidgetSwitcher*		Switcher_SellingStatus = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UWidgetSwitcher*		Switcher_ButtonStatus = nullptr;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	//UProgressBar*			ProgressBar_RemainTime = nullptr; //none
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UTextBlock*				Text_RemainTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UTextBlock*				Text_BuyCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UTextBlock*				Text_ButtonTime = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopScrollItem|Ref")
	UButton*				Button_Click;

private:
	FName					ProductID;
	int32					RemainSellCount;

	
	
private:
	float	ComputeCashPrice();

};