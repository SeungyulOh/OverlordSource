// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_PhaseProgress.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_PhaseProgress : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_PhaseProgress|Ref")
	UWidgetSwitcher*		Switcher_Phase1 = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_PhaseProgress|Ref")
	UWidgetSwitcher*		Switcher_Phase2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_PhaseProgress|Ref")
	UWidgetSwitcher*		Switcher_Phase3 = nullptr;
	UFUNCTION()
	void ProgressPhase();

private:
	int32	PhaseValue = -1;
};