// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_Popup_Avatar.generated.h"

class UUC_LevelUp;
//class UUC_ResultEffect;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Popup_Avatar : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UTextBlock*					Text_After_AvatarLevel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UTextBlock*					BP_Text_Before_Stamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UTextBlock*					BP_Text_After_Stamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UUC_LevelUp*				UC_LevelUp;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
// 	UTextBlock*					Text_NickName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UTextBlock*					BP_Text_Before_PVPStamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UTextBlock*					BP_Text_After_PVPStamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
	UButton*					VerticalBox_QuestGuide;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPopup)
// 	UUC_ResultEffect*			ResultEffect = nullptr;
public:
	UFUNCTION(BlueprintCallable, Category = UUP_Popup_Avatar)
	void RefreshUI();

	UFUNCTION(BlueprintCallable, Category = UUP_Popup_Avatar)
	void OnClick_OK();
};
