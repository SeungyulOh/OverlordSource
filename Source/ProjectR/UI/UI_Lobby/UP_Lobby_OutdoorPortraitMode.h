// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_Lobby_OutdoorPortraitMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Lobby_OutdoorPortraitMode : public URBaseWidget
{
	GENERATED_BODY()	
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Outdoor)
	UButton* Button_Back;

	//// Delegates

	UFUNCTION()
	void OnButtonBackClicked();
};