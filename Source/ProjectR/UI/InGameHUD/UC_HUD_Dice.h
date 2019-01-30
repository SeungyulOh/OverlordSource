// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_Dice.generated.h"

class  UUC_Item_Icons;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_Dice : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void OnModRoomGuildRaidDiceReady(int32 InIndex);
	UFUNCTION()
	void EndDiceTimer();
	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Dice)
	void OnClick_Bidding(bool Participate);

	UFUNCTION()
	void OnModRoomGuildRaidDice(int32 DiceScore);
	void UpdateFlipBook(float InDeltaTime);

	UFUNCTION()
	void OnModRoomGuildRaidDicePaidReward();
	void ShowDiceReward();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	UTextBlock*				TextItemName = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	UUC_Item_Icons*			UC_Item_Icons = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	UPanelWidget*			PanelDiceEndTime = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	UTextBlock*				DiceEndTime = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	TArray<UTextBlock*>		Text_DiceNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	TArray<UImage*>			Image_DiceNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	UWidgetSwitcher*		Switcher_Binding = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Dice)
	UButton*				Button_Giveup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Raid_Notify)
	float					FlipBookTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Raid_Notify)
	TArray<float>			FlipBookExecuteTime;

private:
	UPROPERTY()
	bool DiceBiddingReady = false;
	UPROPERTY()
	int32 KillBossRewardIndex = 0;
	UPROPERTY()
	FTimerHandle DiceBidding_TimeHandler;

	UPROPERTY()
	bool DiceActive = false;
	UPROPERTY()
	float DiceActiveTime = 0.0f;
	UPROPERTY()
	TArray<float> FlipBookActiveTime;
	UPROPERTY()
	FName ScalarParmName = FName("Period");

	UPROPERTY()
	bool ReadyToRewardUI = false;
};
