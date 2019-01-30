// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"

#include "UI/HeroInfo/HeroUpgrade/UC_HeroInfo_Upgrade.h"

#include "UC_Popup_HeroInfo_Upgrade_Splash.generated.h"

UCLASS()
class PROJECTR_API UUC_Popup_HeroInfo_Upgrade_Splash : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;

	void SetHero(FHERO InHero);
	void SetResult(FHERO InHero);

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
protected:
private:

public:
	UPROPERTY()
	FHERO								Hero;
	UPROPERTY()
	URenewal_BaseHeroIcon*				PrevIcon			=	nullptr;
	UPROPERTY()
	URenewal_BaseHeroIcon*				NextIcon			=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_PrevName		=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_NextName		=	nullptr;
protected:
private:
};
