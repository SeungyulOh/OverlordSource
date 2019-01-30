// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_KillNotify.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_KillNotify : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	UFUNCTION()
	void OnBattleRecord_UpdateKillCount(AActor* DeadActor, AActor* FromActor);
	void ShowKillCount(int32 KillCount = 0);
	UFUNCTION()
	void ShowBattleGroundMessage(AActor* TargetActor, FText msg);

	void PrepareWidget();

public:
	// Kill Message
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UOverlay*			Kill_Message = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UWidgetSwitcher*	LeftSwitcher = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UWidgetSwitcher*	RightSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UImage*				LeftImage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UImage*				RightImage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UTextBlock*			TextBlock_LeftAccount = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UTextBlock*			TextBlock_RightAccount = nullptr;

	// KillCount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UOverlay*			Kill_Count = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UTextBlock*			TextBlock_KillCount = nullptr;

	// BattleGround Message
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UOverlay*			BattleGroundMessage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UImage*				BattleGroundImage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UWidgetSwitcher*	SwitcherBattleGround = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UTextBlock*			TextBlock_Notify = nullptr;

	// Animation
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	//UWidgetAnimation* Count_Kill = nullptr;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UWidgetAnimation* Notification_Kill = nullptr;*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	UWidgetAnimation* BattleGround_Reward = nullptr;*/

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	FLinearColor			Color_Friend = FLinearColor(0.040915, 0.238398, 0.854993, 1.000000);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDKillNotify)
	FLinearColor			Color_Enemy = FLinearColor(0.854993, 0.018007, 0.000000, 1.000000);

private:
	UPROPERTY()
	EGamePlayModeTypeEnum	CurGamePlayMode = EGamePlayModeTypeEnum::VE_None;

};
