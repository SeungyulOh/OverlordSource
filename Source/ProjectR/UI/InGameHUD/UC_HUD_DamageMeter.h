// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_DamageMeter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_DamageMeter : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	bool Initialize();
	void PrepareWidget();

	UFUNCTION()
	void OnModRoomDamageMeter(bool bEnable);
	UFUNCTION()
	void RefreshDamageMeter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DamageMeter)
	TArray<UCanvasPanel*>	Panel_BlueTeamList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DamageMeter)
	TArray<UWidgetSwitcher*>	Swicher_BlueTeamMyDataList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DamageMeter)
	TArray<UTextBlock*>		Text_BlueTeamNameList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DamageMeter)
	TArray<UTextBlock*>		Text_BlueTeamDamageList;

private:
	UPROPERTY()
	FTimerHandle RefreshUI_TimeHandler;
	UPROPERTY()
	float RefreshUITime = 1.0f;
	UPROPERTY()
	int32 MyUserKID = KID_NONE;
};
