// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
// #include "UI/UIDefs.h"
// #include "UI/Component/LocalizingTextBlock.h"
#include "UP_CommonPopup.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnButtonClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnBuyButtonClick, int32, InPayType);
class ULocalizingTextBlock;
class URenewal_BaseHeroIcon;
class UUC_CurrencyDisplay;
/**
 * 
 */
//UC_Popup_HeroBuy
UCLASS()
class PROJECTR_API UUP_CommonPopup : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = CommonPopup)
	void SetCommonPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType );

	UFUNCTION(BlueprintCallable, Category = CommonPopup)
	void SetHeroPopup(FName InHeroID, FText InCaption, FText InContents);

	UFUNCTION(BlueprintCallable, Category = CommonPopup)
	void RefreshUI();

	void SetHeroCurrency();
	void SetShortcutPopupType();

	void SetCurrency(FName InCurrencyType, int32 InPrice);
	void HideCurrency();

	UFUNCTION()
	void OnClickButtonOK();
	UFUNCTION()
	void OnClickButtonClose();

	void SetBlockHidePopup() { bBlockHidePopup = true; }
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UTextBlock*					Text_Caption;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UTextBlock*					BP_Text_Body;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	URRichTextBlock*			ContentsRichTextBlock;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	ULocalizingTextBlock*		LocalizingTextBlock_OK;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	ULocalizingTextBlock*		LocalizingTextBlock_Cancel;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UButton*					Button_OK;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UButton*					Button_Cancel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UButton*					Button_Close;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	USizeBox*					SizeBox_OK;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	USizeBox*					SizeBox_Cancel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	URenewal_BaseHeroIcon*		HeroIcon = nullptr;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = CommonPopup)
	FDelegate_OnButtonClick		OnClickOK;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = CommonPopup)
	FDelegate_OnButtonClick		OnClickCancel;	


	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = CommonPopup)
	FDelegate_OnBuyButtonClick		OnBuyClickOK;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UWidgetSwitcher*			ButtonSwitcher;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UUC_CurrencyDisplay*		Currency1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UUC_CurrencyDisplay*		Currency2;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UButton*					Currency2Controls;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommonPopup)
	UOverlay*					CurrencyBox = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommonPopup)
	UImage*						CurrencyImage = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	ULocalizingTextBlock*		PriceText = nullptr;
private:
	UPROPERTY()
	FText Caption;
	UPROPERTY()
	FText Contents;
	UPROPERTY()
	ECommonPopupType PupupType;

	FName HeroID;

	bool bBlockHidePopup = false;
};

