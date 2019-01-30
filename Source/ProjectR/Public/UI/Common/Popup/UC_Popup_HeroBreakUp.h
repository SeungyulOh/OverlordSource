// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "Button.h"
#include "UC_Popup_HeroBreakUp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_HeroBreakUp : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	void SetBreakUpList(TArray<FString> list);

	UFUNCTION()
		void OnClickCancel();
	UFUNCTION()
		void OnClickOK();

public:
	UPROPERTY()
		UButton*					Button_Cancel = nullptr;
	UPROPERTY()
		UButton*					Button_Ok = nullptr;
	UPROPERTY()
		UTextBlock*  Text_SelectedCount = nullptr;

private:
	TArray<FString>					SelectedHeros;
};
