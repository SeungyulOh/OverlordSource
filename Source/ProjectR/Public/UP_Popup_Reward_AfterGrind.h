// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UP_Popup_Reward_AfterGrind.generated.h"

DECLARE_DELEGATE(FDelegate_OnRewardPopupClickClose);


class UUC_RewardAfterGrindScrollView;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Popup_Reward_AfterGrind : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	UFUNCTION()
	void OnClickClose();

	void Refresh();


	FDelegate_OnRewardPopupClickClose OnClickCloseEvent;
	FORCEINLINE UUC_RewardAfterGrindScrollView* GetScrollView() { return ScrollView; }
	
private:
	UPROPERTY()
	UButton*	Button_Close = nullptr;
	
	UPROPERTY()
	UUC_RewardAfterGrindScrollView*	ScrollView = nullptr;
};
