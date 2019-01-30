// Fill out your copyright notice in the Description page of Project Settings.

// Cloned from UP_CommonPopup.h 

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UP_Popup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_PopupOnButtonClick);

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Popup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = ContentDownloadPopup)
	void SetPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType );

	UFUNCTION(BlueprintCallable, Category = ContentDownloadPopup)
	void RefreshUI();

	UFUNCTION(BlueprintImplementableEvent, Category = ContentDownloadPopup)
	void SetCancelButtonVisibility(ESlateVisibility InVisibility);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadPopup)
	UTextBlock*					CaptionTextBlock;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadPopup)
	UTextBlock*					ContentsTextBlock;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadPopup)
	UButton*					OkButton;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadPopup)
	UButton*					CancelButton;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ContentDownloadPopup)
	FDelegate_PopupOnButtonClick		OnClickOK;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ContentDownloadPopup)
	FDelegate_PopupOnButtonClick		OnClickCancel;	

private:
	UPROPERTY()
	FText Caption;
	UPROPERTY()
	FText Contents;
	UPROPERTY()
	ECommonPopupType PopupType;
};

