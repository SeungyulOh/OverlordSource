// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_Popup_Splash_Treasure.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Popup_Splash_Treasure : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void RefreshUI(ETreasureSplashType InType);

	UFUNCTION()
	void OnClick_Close();

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_Popup_Splash_Treasure)
	UTextBlock*				Text_Caption = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_Popup_Splash_Treasure)
	ULocalizingTextBlock*	LocalText_Desc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Popup_Splash_Treasure)
	UImage*					Image_Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Popup_Splash_Treasure)
	UButton*				Button_Click = nullptr;


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Popup_Splash_Treasure)
//	UWidgetAnimation*		AppearAni = nullptr;

private:
	bool					bPlayStart = false;

	ETreasureSplashType		PopupType;
};
