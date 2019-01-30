// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Popup_FloorShop_Default.generated.h"

UENUM(BlueprintType)
enum class EPopupStatus : uint8
{
	FloorActive		=	0,
	FreeRefresh		=	1,
	PayRefresh		=	2,
};
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_FloorShop_Default : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init();

	void FloorActivePopup();
	void FreeRefreshPopup();
	void PayRefreshPopup();

	UFUNCTION()
	void OnButtonClickOk();
	UFUNCTION()
	void OnButtonClickClose();

	UFUNCTION()
	void OnButtonTicketFail();

public:
	UPROPERTY()
	UWidgetSwitcher*		WidgetSwitcher_State	=	nullptr;
	UPROPERTY()
	UTextBlock*				Text_ActiveCurrency		=	nullptr;
	UPROPERTY()
	UTextBlock*				Text_RefreshCurrency	=	nullptr;
	UPROPERTY()
	UButton*				Button_Close			=	nullptr;
	UPROPERTY()
	UButton*				Button_Cancel			=	nullptr;
	UPROPERTY()
	UButton*				Button_Ok				=	nullptr;
};