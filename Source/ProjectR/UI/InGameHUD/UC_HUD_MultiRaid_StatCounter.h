// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_HUD_MultiRaid_StatCounter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_MultiRaid_StatCounter_Line: public URBaseWidget
{
	GENERATED_BODY()


public:
	void NativeConstruct() override;
	void NativeDestruct() override;


	//property
	UPROPERTY()
		UTextBlock* Text_Rank = nullptr;
	UPROPERTY()
		UTextBlock* Text_NickName = nullptr;
	UPROPERTY()
		UTextBlock* Text_Score = nullptr;
};


UCLASS()
class PROJECTR_API UUC_HUD_MultiRaid_StatCounter : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
		void OnRefreshUI(TArray<FString> nickList, TArray<int32> damageList);
	UFUNCTION()
		void OpenExtend();
	UFUNCTION()
		void CloseExtend();

	//property
	UPROPERTY()
		UButton* Button_Open;
	UPROPERTY()
		UButton* Button_Close;
	UPROPERTY()
		UWidgetSwitcher* DamageCount_Switcher;
	UPROPERTY()
		UUC_HUD_MultiRaid_StatCounter_Line* UC_HUD_MultiRaid_Stat_S;
	UPROPERTY()
		TArray<UUC_HUD_MultiRaid_StatCounter_Line*> UC_HUD_MultiRaid_Stat_Mlist;
};
