// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UP_Shop.generated.h"


class URScrollView;
class UShopScrollItem;
class UCanvasPanel_RadioGroup;
class UUC_HeroBox;
class URScrollItem;
class UUC_Item_Icons;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Shop : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	
	
	void UpdateProductList();

	UFUNCTION(BlueprintCallable, Category = UUP_Shop)
	void SetShopDisplayPage(int32 InDisplayIndex, bool bForceUpdate = false );


	/*
	Click event
	*/
	UFUNCTION(BlueprintCallable, Category = UUP_Shop)
	void OnClick_Purchase( FName InProductID);

	UFUNCTION(BlueprintCallable, Category = UUP_Shop)
	void OnClick_ResetRequest();

	UFUNCTION(BlueprintCallable, Category = UUP_Shop)
	void OnClick_Close();

	
	UFUNCTION(BlueprintCallable, Category = UP_Shop)
	void ShowPurchasePopup(UShopScrollData* InSelectedData);

	UFUNCTION(BlueprintCallable, Category = UP_Shop)
	void ClosePurchasePopup();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UP_Shop")
	void OnCashEvent();


	UFUNCTION()
	void	OnChangeShopData();

	void InitProductScrollList();
	int32 GetMaxResetCount(int32 InDisplayIndex);

public:
	UPROPERTY()
	FTimerHandle				UpdateResetRemainTimeHandler;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop|Test)
	//UTextBlock*				ResetMaxCount;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop|Test)
	//UTextBlock*				ResetCurrentCount;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	//UTextBlock*				ResetMaxCount_ResetPopup;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	//UTextBlock*				ResetCurrentCount_ResetPopup;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop|Test)
	//UTextBlock*				ResetRemainedTime;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_Shop|Test")
	//UWidgetSwitcher*		CurrencySelectSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Shop)
	UCanvasPanel_RadioGroup*	RadioGroup = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	//UWidgetSwitcher*		TypeSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	URScrollView*			ProductListScrollView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	UPanelWidget*			HeroPanelWidget = nullptr;
	
	/*
	purchase popup
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	UCanvasPanel*			PurchasePopupPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	UShopScrollItem*		ScrollItemInPurchasePopup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	UUC_Item_Icons*			PopupItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	UTextBlock*				PopupTitleText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	UTextBlock*				PopupDescText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	FName					CurPopupProductID;

	UPROPERTY()
	class UUC_Particle*		UC_Particle;


	/*
	Reset popup
	*/
	///UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	//UCanvasPanel*			ResetPopupPanel;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	//UUC_CurrencyDisplay*	ResetCurrencyDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop)
	int32					CurrentShopDisplayIndex = 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Shop | Test)
	//TArray< UUC_CurrencyDisplay* >			CurrencyDisplayList;

	UPROPERTY()
	TArray<class UShopScrollData*> CachedItemDataList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "01 Base")
	TArray< TSoftObjectPtr<URBaseWidget> >			SubPopupList;

private:
	void InvalidateCurrencyDisplay();

private:
	//bool bFirst = false;

};