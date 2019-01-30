// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"
#include "HeroUIforCrew.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API URHeroUIforCrew : public URHeroUIforBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo) override;
	void UpdateIcon() override;
	void UpdateHp(float InHp);
	void SetRTSHeroSlot(FHeroSlotInfo& InHeroSlotInfo) override;
	void RTSUpdateIcon() override;


// 	UFUNCTION()
// 	void OnSwapAniFinished();
public:
	UPROPERTY()
	UImage*						Image_Hero = nullptr;
	
	UPROPERTY()
	UOverlay*					HeroDeathOverlay = nullptr;
	
	UPROPERTY()
	UTextBlock*					Level_Text = nullptr;

	UPROPERTY()
	UTextBlock*					HpPercentText = nullptr;
	
	UPROPERTY()
	UImage*						Image_HP_Progress = nullptr;

	bool bBattleCrew = false;
};