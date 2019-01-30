// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_Coloseum_RankUp_Splash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Coloseum_RankUp_Splash : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	//properties
	UPROPERTY()
		UWidgetSwitcher* RankSwitcher = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_RankRaised = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_ExtraMessage = nullptr;
	UPROPERTY()
		UButton* Button_MatchingCancel = nullptr;
};
