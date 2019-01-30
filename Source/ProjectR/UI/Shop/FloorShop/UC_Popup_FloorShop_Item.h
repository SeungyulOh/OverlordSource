// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Popup_FloorShop_Item.generated.h"




/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_FloorShop_Item : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init();
	void SetItemInfo(FSHOP_FLOORSHOP_SLOT InItemInfo);

	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickOK();

public:
	UPROPERTY()
	UButton*					Button_Cancel		=	nullptr;
	UPROPERTY()
	UButton*					Button_Close		=	nullptr;
	UPROPERTY()
	UButton*					Button_Ok			=	nullptr;
	UPROPERTY()
		class UUC_GenericItemIcon* UC_GenericItemIcon = nullptr;
	UPROPERTY()
	UTextBlock*					Text_Currency		=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_ItemName		=	nullptr;
	UPROPERTY()
	UImage*						Image_Currency		=	nullptr;
	UPROPERTY()
		UWidgetSwitcher* WidgetSwitcher_ContentsText;

private:
	FSHOP_FLOORSHOP_SLOT	ItemInfo;
};