// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"
#include "HeroUIforLeader.generated.h"

class UUC_Hero_Skill_Icon;
/**
*
*/
UCLASS()
class PROJECTR_API URHeroUIforLeader : public URHeroUIforBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


	void SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo) override;
	void UpdateIcon() override;
	void SetRTSHeroSlot(FHeroSlotInfo& InHeroSlotInfo) override;
	void RTSUpdateIcon() override;

	// for arena & mirror
	void SetTeamColorSwitcherIndex(int32 InIndex);

	void SetUIForCampaign(bool InbPlayAlone);
	void SetUIForSingleRaid();
	void SetUIForMultiRaid();
	void SetActiveSpiritSkillIcon(int32 InIndex, FName InSpiritSkillName);
	void HideUnUsedSpiritSkillIcon(bool InbUsedSpirit1, bool InbUsedSpirit2);
public:
	UPROPERTY()
	ULocalizingTextBlock*		UserNameText = nullptr;

	UPROPERTY()
	UImage*						Image_Hero = nullptr;

	UPROPERTY()
	UImage*						Leader_HP = nullptr;

	UPROPERTY()
	UOverlay*					HeroDeathOverlay = nullptr;

	UPROPERTY()
	UImage*						LeaderLine = nullptr;

	UPROPERTY()
	UTextBlock*					Level_Text = nullptr;

	UPROPERTY()
	UWidgetSwitcher*			TeamColorOutLineSwitcher = nullptr;

	UPROPERTY()
	UTextBlock*					HpPercentText = nullptr;

	UPROPERTY()
	UUC_Hero_Skill_Icon*		ActiveSpiritSkill01 = nullptr;
	UPROPERTY()
	UUC_Hero_Skill_Icon*		ActiveSpiritSkill02 = nullptr;

private:
// 	UPROPERTY()
// 	FHeroSlotInfo		HeroSlotInfo;

	UPROPERTY()
	bool				bPlayAlone = false;
};