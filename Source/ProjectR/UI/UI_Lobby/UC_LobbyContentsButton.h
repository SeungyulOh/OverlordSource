// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "UC_LobbyContentsButton.generated.h"

class ULocalizingTextBlock;
class UImage;
class UButton;

/**
 * Represents a button to go to a content in Lobby UI
 */
UCLASS()
class PROJECTR_API UUC_LobbyContentsButton : public URBaseWidget
{
	GENERATED_BODY()	
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyContentsButton)
	ULocalizingTextBlock* Text_Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyContentsButton)
	UImage* Image_Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyContentsButton)
	UButton* Button_Contents;

	//// BP Config

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyContentsButton)
	EUIMenuIDEnum UIMenuID = EUIMenuIDEnum::VE_HeroInventory;
	
private:
	UFUNCTION()
	void OnButtonContentsClicked();
};