// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_ProgressAchievement.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_ProgressAchievement : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

// 	UFUNCTION()
// 	void OnModRoomGuildRaidProgress();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ProgressAchievement)
	TArray<UWidgetSwitcher*>		Switcher_Progress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ProgressAchievement)
	TArray<UWidgetSwitcher*>		Switcher_BossKill;
};
