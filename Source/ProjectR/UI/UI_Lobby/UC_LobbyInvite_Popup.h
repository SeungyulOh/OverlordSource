// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_LobbyInvite_Popup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_LobbyInvite_Popup : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyInvite_Popup)
		ULocalizingTextBlock* TextBlock_WhoInvited;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyInvite_Popup)
		ULocalizingTextBlock* TextBlock_WhereInvited;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyInvite_Popup)
		UButton* Button_OK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyInvite_Popup)
		UButton* Button_Cancel;
	
};
