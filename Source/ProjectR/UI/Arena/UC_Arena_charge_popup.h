// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIDefs.h"
#include "UI/RBaseWidget.h"
#include "UC_Arena_charge_popup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Arena_charge_popup : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetText(int32 InGem, int32 InCurency, bool InDaily);
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
public:
	UPROPERTY()
	UButton*		Button_OK = nullptr;
	UPROPERTY()
	UButton*		Button_Back = nullptr;
	UPROPERTY()
	UButton*		Button_Back2 = nullptr;
	UPROPERTY()
	UTextBlock*		Text_BottomCount = nullptr;
	UPROPERTY()
	UTextBlock*		Text_TopCount = nullptr;
	UPROPERTY()
	UTextBlock*		Text_Currency = nullptr;
	UPROPERTY()
	UTextBlock*		Text_Change = nullptr;

	UPROPERTY()
	UTextBlock*		Text_Topbar = nullptr;
	UPROPERTY()
	UTextBlock*		Text_AddTooltip = nullptr;
	UPROPERTY()
	UTextBlock*		Text_DailyBonus = nullptr;

	bool			IsDaily = false;
};