// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Shop/FloorShop/UC_Popup_FloorShop_Item.h"

#include "UC_FloorShop_ScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_FloorShop_ScrollItem : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetItemInfo(FSHOP_FLOORSHOP_SLOT ItemInfo);

	UFUNCTION()
	void OnClickItemBuy();

public:
	UPROPERTY()
		class UUC_GenericItemIcon* UC_GenericItemIcon = nullptr;
	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher_Visible	= nullptr;
	UPROPERTY()
		UWidgetSwitcher* WidgetSwitcher_Title = nullptr;
	UPROPERTY()
	UButton* Button_Buy	= nullptr;
	UPROPERTY()
	UTextBlock* Text_Currency_Enable = nullptr;
	UPROPERTY()
		UTextBlock* Text_Currency_Disable = nullptr;
	UPROPERTY()
	UOverlay* LOCK = nullptr;
	UPROPERTY()
	UImage*	Image_Currency_Enable =	nullptr;
	UPROPERTY()
		UImage*	Image_Currency_Disable = nullptr;
	UPROPERTY()
	UTextBlock*	Text_Name = nullptr;
	
private:
	FSHOP_FLOORSHOP_SLOT		ItemInfo;
};