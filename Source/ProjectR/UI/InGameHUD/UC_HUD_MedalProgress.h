// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_MedalProgress.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_MedalProgress : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

// 	UFUNCTION()
// 	void OnModRoomProgressMedal(bool bActive);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_MedalProgress)
	UProgressBar*			BP_ProgressBar_Success = nullptr;

	UPROPERTY()
	bool					ActiveProgress = false;
	UPROPERTY()
	float					ProgressMaxTime = 3.0f;
	UPROPERTY()
	float					ProgressCheckTime = 0.0f;
	UPROPERTY()
	float					PercentValue = 0.0f;
};
