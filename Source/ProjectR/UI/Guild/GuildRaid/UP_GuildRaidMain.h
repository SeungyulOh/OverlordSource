// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_MemberData.h"
#include "UP_GuildRaidMain.generated.h"

class UUC_GuildRaid_SeasonInfo;
class UUC_GuildRaid_Setting;
class UUC_GuildRaidSeasonReward;
class URScrollView;
class UUC_GuildRaid_StageButton;
class UCanvasPanel_RadioGroup;
class UWidgetSwitcher;
//class UUC_GuildRaid_MemberData;
class UWidget;

UENUM(BlueprintType)
enum class ERaidBossTypeEnum : uint8
{
	VE_RaidBoss_1		UMETA(DisplayName = "RaidBoss_1"),
	VE_RaidBoss_2		UMETA(DisplayName = "RaidBoss_2"),
	VE_RaidBoss_3		UMETA(DisplayName = "RaidBoss_3"),
};

#define BOSS_SUMMON_LIMIT_LEVEL1 3
#define BOSS_SUMMON_LIMIT_LEVEL2 6
#define BOSS_SUMMON_LIMIT_LEVEL3 9

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_GuildRaidMain : public URBaseWidget
{
	GENERATED_BODY()
	

public:

	void NativeConstruct() override;
	void NativeDestruct() override;

// 	void Init();
// 
// 	void ShowSeasonRewardPopup(int32 RewardIndex, bool NextRewardIndex);
// 	//////////////////////////////////////////////////////////////////////////
// 	// new
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
// 
// 	void SeasonTimeTick(float InDeltaTime);
// 	void OpenRaidTimeTick(float InDeltaTime);
// 
// public:
// 	UFUNCTION()
// 	void			OnSelectRewardInfoScrollItem(URScrollItemData* selectItem);
// 	UFUNCTION()
// 	void			OnSelectGetableRewardScrollItem(URScrollItemData* selectItem);
// 	UFUNCTION()
// 	void			OnSelectApperMonsterScrollItem(URScrollItemData* selectItem);
// 	UFUNCTION()
// 	void			OnSelectMemberHistoryScrollItem(URScrollItemData* selectItem);
// 
// 	UFUNCTION()
// 	void			OnRefreshRaidUI();
// 
// 	UFUNCTION()
// 	void			OnRefreshRaidHistory();
// 
// 	UFUNCTION()
// 	void			OnReceiveRealServerInfo();
// 	//////////////////////////////////////////////////////////////////////////
// 	// new
// 	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = UUP_GuildRaidMain)
// 	void			OnClickChallengeButton();
// 	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = UUP_GuildRaidMain)
// 	void			OnClickSummonBossButton();
// 	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = UUP_GuildRaidMain)
// 	void			OnClickBossInfoButton();
// 	
// 	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = UUP_GuildRaidMain)
// 	void			OnClickRaidBossRadio(ERaidBossTypeEnum InType);
// 	
// 	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = UUP_GuildRaidMain)
// 	void			OnClick_Difficulty(int32 InDifficulty);
// 
// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UUC_GuildRaid_SeasonInfo*			RaidSeasonInfo;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UUC_GuildRaid_Setting*				RaidSetting;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UUC_GuildRaidSeasonReward*			RaidSeasonReward;
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// new
// 	
// 	// left, top side
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	TArray< UUC_GuildRaid_StageButton* >	Button_Stages;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UCanvasPanel_RadioGroup*				BossRadioGroup = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	TArray< UWidgetSwitcher* >				BossStatus; // 0 : open, 1 : close
// 
// 	// bottom side
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UWidgetSwitcher*						EnterButtonSwitcher = nullptr; // 0 : Challenge, 1 : SummonBoss, 2 : Complete
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	ULocalizingTextBlock*					LocDisableText = nullptr;
// 
// 	// right side
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	ULocalizingTextBlock*					RaidBossNameText = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UTextBlock*								SeasonOut_RemainHours = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UTextBlock*								SeasonOut_RemainMinutes = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UTextBlock*								SeasonOut_RemainSeconds = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	TArray<UWidget*>						SeasonOut_TimeControls;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	ULocalizingTextBlock*					AttackTeamLeaderNameText = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UWidgetSwitcher*						MemberHistorySwitcher = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UTextBlock*								MemberHistoryCount = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UTextBlock*								MemberHistoryCountMax = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	URScrollView*							MemberHistoryScrollView = nullptr;
// 
// 	// center
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UImage*									BossImage = nullptr;
// 
// 	// scrollview
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	URScrollView*							RewardInfoScrollView = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	URScrollView*							GetableRewardScrollView = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	URScrollView*							MonsterScrollView = nullptr;
// 
// 	// popup
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UCanvasPanel*							RaidSettingPanel = nullptr;
// 
// 	// OpenRaidInfo
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UTextBlock*								OpenRaidTime_Min = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	UTextBlock*								OpenRaidTime_Sec = nullptr;
// 
// 	// guild raid status
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	TArray< UWidgetSwitcher* >				RaidProgressBar;	// 0 : live, 1 : dead
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildRaidMain)
// 	TArray< UWidgetSwitcher* >				RaidBossStatus;		// 0 : live, 1 : dead
// 
// private:
// 	void InvalidateScrollView();
// 	//void InvalidateRaidListScrollView();
// 	void InvalidateMemberHistoryScrollView();
// 	void InvalidateRewardInfoScrollView();
// 	void InvalidateGetableRewardScrollView();
// 	void InvalidateMonsterScrollView();
// 
// 	void InvalidateSeasonInfo();
// 	void InvalidateRaidSetting();
// 
// 	bool CheckEnoughPoints(FName ModID);
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// new
// 	void SetStageButton(bool bInit = false);
// 	void SetRaidBossTab(int32 InBossIndex);
// 	void SetRaidBossInfo(int32 InActiveBossType);
// 	void SetMemberHistory();
// 	void SetBossThumnail(ERaidBossTypeEnum InBossType);
// 	void SetSeasonOutTimeText(int32 InHour, int32 InMin, int32 InSec);
// 	void SetOpenRaidTimeText(int32 InMin, int32 InSec);
// 	void SetUIbySeasonStatus(bool bSeasonEnd);
// 	int32 GetModID();
// 	void SetGuildRaidStageInfos(TArray<FGUILD_RAID>& rRaidInfos);
// 	void SetRaidBossStatus();
// 	bool IsAbleSummonBoss(int32 InModID);
// 	int32 GetBossSummonLimitLevel(int32 InBossType);
// private:
// 	TArray<bool>						LockedDifficulty;
// 	int32								RaidBossDifficultyIndex = 0;
// 	ERaidBossTypeEnum					RaidBossType = ERaidBossTypeEnum::VE_RaidBoss_1;
// 	int32								BossTypeIndex = 0;
// 	TArray<FGUILD_RAID>					RaidBossInfo;
// 	TArray<FGUILD_RAID_CHALLENGE>		CurrentRaidHistory;
// 	//TArray<UUC_GuildRaid_MemberData>		ChallengeMemberDataList;
// 	int32								OpenRaidmodId = 0;
// 
// 	// seasontime
// 	bool								bSeasonTimeCount = false;
// 	int64								nCurrentSeasonOutTime = 1;
// 
// 	// openraidtime
// 	bool								bOpenRaidTimeCount = false;
// 	int64								nCurrentOpenRaidTime = 1;

};
