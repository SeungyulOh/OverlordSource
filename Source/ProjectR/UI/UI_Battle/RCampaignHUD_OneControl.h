// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "ProjectRGameMode.h"
#include "Public/UC_FriendHUD.h"


#include "RCampaignHUD_OneControl.generated.h"


class ABasePlayerController;
class URSkillSet;
class URBossBar;
class URBattleOptionMenu;
class URCameraIcon;
class URAutoSkillIcon;
class URMeetingIcon;
class URInputIndicator;
class URHudStar;
class UUC_SkillIndicator;
class UUC_Chat_Ingame;
class UUC_HUD_PVP_TotalHP;
class UUC_HUD_PhaseProgress;
class UUC_HUD_TowerHP;
class UUC_HUD_Goal;
class URMultiBossBar;
class URHeroUIforBase;
class URHeroUIforLeader;
struct FHeroSlotInfo;

UCLASS()
class PROJECTR_API URBaseIngameHUD : public URBaseWidget
{
	GENERATED_BODY()

	/*Functions*/
public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void	SetTimeText(int32 InRemainTime);

	virtual void	UpdateHeroIconSlot(){}
	virtual void	HideUnusedCharacterUI(){}

	virtual int32 GetSlotPosition(FHeroSlotInfo& InSlot);
	virtual void	SetStarHUD(EPlayStageStarEnum StarCnt);
	virtual void	ShowSkillIndicator(FName id, float time);

	/*UFunctions*/
public:
	UFUNCTION(BlueprintCallable, Category = RCampaignHUD)
	virtual void	OnSkillIndicator(FName id);

	/*UProperties*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	URHudStar*				UC_HUD_Star = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UUC_SkillIndicator*		SkillIndicator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCampaignHUD)
	ULocalizingTextBlock*	RemainMinuteText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCampaignHUD)
	ULocalizingTextBlock*	RemainSecondText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCampaignHUD)
	ULocalizingTextBlock*	LocalizingTextBlock_TitleText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCampaignHUD)
	ULocalizingTextBlock*	LocalizingTextBlock_SubTitle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref|Camp")
	UUC_FriendHUD*			UI_ProtctHero_01 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref|Camp")
	UUC_FriendHUD*			UI_ProtctHero_02 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref|Camp")
	UUC_HUD_TowerHP*		UI_ProtectMode = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref|Camp")
	UUC_HUD_TowerHP*		UI_ProtectMode2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	URAutoSkillIcon*		AutoSkillIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	class URSkillSet_New*	SkillSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UWidgetSwitcher*		WidgetSwitcher_0 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	class USafeZone*		RSafeZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UPanelWidget*			Hero_Icons = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UImage*					Noti_Danger = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UWidgetSwitcher*		HPBarSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	URBattleOptionMenu*		OptionMenu;

	UPROPERTY()
	UUserWidget* JoyStickDirection = nullptr;

	UPROPERTY()
	URHeroUIforLeader*				LeaderIconUI; //리더 actor 정보 ui

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCampaignHUD)
	float					CharacterFocusCameraTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCampaignHUD)
	float					CharacterFocusCameraExp = 0.5f;

protected:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	UPROPERTY()
	bool					bPause = false;
	UPROPERTY()
	float					ShowTime = 0.0f;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSpiritHelper
{
	GENERATED_USTRUCT_BODY()

	// 영혼갯수 1개일경우 스폰위치 구분용 변수
	bool					bActiveSpiritAll = false;
	UPROPERTY()
	TMap<ECharacterRoleEnum, class ACrewSpiritParticleActor*>	CrewSpiritMap;

	void SetSpiritEffect(ECharacterRoleEnum InRole);
	void Update(ECharacterRoleEnum InRole);

	UPROPERTY()
	class URCampaignHUD_OneControl* ParentWidget;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API URCampaignHUD_OneControl : public URBaseIngameHUD
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
	virtual void	UpdateHeroIconSlot();
	virtual void	HideUnusedCharacterUI();

	FSpiritHelper SpiritHelper;
 	


};
