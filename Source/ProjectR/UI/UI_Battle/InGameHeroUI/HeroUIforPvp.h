// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"
#include "HeroUIforPvp.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API URHeroUIforPvp : public URHeroUIforBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo) override;
	void UpdateIcon() override;

	void SetHero(AActor* InOwnerActor, FName InHeroID, int32 InLevel);

	void SetOwnerActor(AActor* InActor) { OwnerActor = InActor; }

	void SetEnemyHPColor();
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
	UImage*						Leader_HP = nullptr;

	UPROPERTY()
	ULocalizingTextBlock*		NameText = nullptr;

	UPROPERTY()
	UImage*						Noti_Danger_2 = nullptr;

private:
	UPROPERTY()
	AActor*						OwnerActor = nullptr;
};
