// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_ReSpawn.generated.h"

class UUC_CurrencyDisplay;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_ReSpawn : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ReSetData();

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_ReSpawn)
	void OnClick_SpawnZone();
	UFUNCTION(BlueprintCallable, Category = UUC_HUD_ReSpawn)
	void OnClick_QuickSpawn();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ReSpawn)
	UTextBlock*				TextBlock_KillerName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ReSpawn)
	UProgressBar*			ProgressBar_ReSpawnTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ReSpawn)
	UTextBlock*				TextBlock_RemainTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ReSpawn)
	UUC_CurrencyDisplay*	ReSpawnCurrency = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ReSpawn)
	UButton*				Button_QuickSpawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_ReSpawn)
	UButton*				Button_SpawnZone = nullptr;

private:
	UPROPERTY()
	bool ActiveReSpawnTimer = false;
	UPROPERTY()
	float MaxTime = 0.0f;
	UPROPERTY()
	float RemainTime = 0.0f;
};
