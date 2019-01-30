// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "SharedConstants/GlobalConstants.h"
#include "RHeroIcon_ForInGame.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTR_API URHeroIcon_ForInGame : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	
	void SetHeroSlot(FHeroSlotInfo& HeroSlotInfo);
	void UpdateIcon();
	void SetRTSHeroSlot(FHeroSlotInfo& HeroSlotInfo);
	void RTSUpdateIcon();
	void SetTeamColorSwitcherIndex(int32 InIndex);
public:
	UPROPERTY()
	UImage*		Image_Hero = nullptr;

	UPROPERTY()
	UImage*		Noti_Danger = nullptr;

	UPROPERTY()
	UProgressBar* HpProgerssBar = nullptr;

	UPROPERTY()
	UTextBlock* HpText = nullptr;

	UPROPERTY()
	UOverlay* HeroDeathPanel = nullptr;

	UPROPERTY()
	ULocalizingTextBlock* Level_Text = nullptr;

	UPROPERTY()
	UImage*			ImageBack = nullptr;

	UPROPERTY()
	UImage*			BG = nullptr;

	UPROPERTY()
	UWidgetSwitcher*	TeamColorOutLineSwitcher = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = URHeroIcon_ForInGame)
	ECharacterRoleEnum		Role = ECharacterRoleEnum::VE_None;

	UPROPERTY()
	ECharacterCrewStateEnum	BattleState = ECharacterCrewStateEnum::VE_None;

private:
	UPROPERTY()
	FHeroSlotInfo		HeroSlotInfo;

	float	DangerValue = 0.3;

	bool				bRTS = false;

};
