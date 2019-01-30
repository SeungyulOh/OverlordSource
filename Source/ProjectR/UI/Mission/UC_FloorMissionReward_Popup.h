// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_FloorMissionReward_Popup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_FloorMissionReward_Popup : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
		void OnClicked_ButtonClose();

	void SetRewardPreviewList();

	//property
	UPROPERTY()
		UButton* Button_Close;
	UPROPERTY()
		UButton* Button_OK;
	UPROPERTY()
		TArray<class UUC_Item_Icons*> rewardPreviewIconList;
	UPROPERTY()
		TArray<USizeBox*> SizeBoxList;
};
