// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UP_Arena_Lobby.generated.h"


const int MAX_MATCH_COUNT = 5;
const int MAX_SEASON_TIME = 604800; // second : // 60sec * 60min *24Hour * 7Day
const int TOTAL_SEASON_REWARD_COUNT = 30; // 1 ~ 10, 3개씩
const int MAX_SEASON_REWARD_RANK_COUNT = 10; // 1 ~ 10, 3개씩
const int SEASON_REWARD_EACH_COUNT = 3;

class UUC_Arena_ranking_list;
class UUC_Arena_reward_popup;
class UUC_Arena_charge_popup;
class UCheckBox_Radio;

UCLASS()
class PROJECTR_API UUP_Arena_Lobby : public URBaseWidget
{
	GENERATED_BODY()
public:
	// 전투 기록 및 랭킹
	UPROPERTY()
	class UCanvasPanel_RadioGroup*	Tab_Quest = nullptr;
	UPROPERTY()
	UCheckBox_Radio*			Radio_Info = nullptr;
	UPROPERTY()
	UCheckBox_Radio*			Radio_HistoryRecord = nullptr;
	UPROPERTY()
	UCheckBox_Radio*			Radio_Ranking = nullptr;

	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_List = nullptr;
	
	UPROPERTY()
	UCanvasPanel*				GameInformation = nullptr;
	
	// 정보
	UPROPERTY()
	UUserWidget*				UC_Arena_Tier = nullptr;
	UPROPERTY()
	UTextBlock*					Text_Rank = nullptr;
	UPROPERTY()
	UTextBlock*					Text_MyPoint = nullptr;
	UPROPERTY(EditAnywhere ,BlueprintReadWrite ,Category = "Arena_Lobby")
	UTextBlock*					TextBlock_SeasonEnd = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_Day = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_Hour = nullptr;
	UPROPERTY()
	UTextBlock*					Text_DefenceCount = nullptr;
	UPROPERTY()
	UProgressBar*				ProgressBar_TimeRemaining = nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_Season = nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_SeasonCount = nullptr;
	UPROPERTY()
	UButton*					Button_RewardInfo = nullptr;
	UPROPERTY()
	UButton*					Button_DefendingTeam = nullptr;
	UPROPERTY()
	UButton*					Button_BattlePreparation = nullptr;
	UPROPERTY()
	UButton*					Button_Reward = nullptr;
	UPROPERTY()
	UUC_Arena_ranking_list*		UC_Arena_ranking_list = nullptr;
	UPROPERTY()
	UButton*					Button_ArenaCountCharge = nullptr;
	UPROPERTY()
	UButton*					Button_DefenceReward = nullptr;
	UPROPERTY()
	UUC_Arena_reward_popup*		UC_Arena_reward_popup = nullptr;
	UPROPERTY()
	UTextBlock*					Text_DefenceTime = nullptr;
	UPROPERTY()
	UUC_Arena_charge_popup*		UC_Arena_charge_popup = nullptr;
	UPROPERTY()
	UTextBlock*					Text_NextDefenceReward = nullptr;

	UPROPERTY()
	UWidgetSwitcher*			InfoRadioSwitcher;
	UPROPERTY()
	UWidgetSwitcher*			RankingRadioSwitcher;
	UPROPERTY()
	UWidgetSwitcher*			HistoryRadioSwitcher;
	
	UPROPERTY()
	TArray< TWeakObjectPtr<UWidgetSwitcher> >	TabButtonRadio;
private:
	int32						m_nNumMatchList = 0;
	bool						m_bMatchEnable = true;
	bool						m_bMatchListRefresh = false;
	float						m_InitialRefreshLeftTime = 0.0f;
	int32						MatchIndex = 0;
	FTimerHandle				DelayTimer;

// 	UPROPERTY()
// 	TArray<FITEM_ICON_INFO>		SeasonTop10RewardList;
// 
// 	UPROPERTY()
// 	TArray<FITEM_ICON_INFO>		SeasonSectionRewardList;

public:

	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Init();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Reset();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Button_ClickRewardInfoPopUp();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Button_ClickDefendingTeam();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Button_ClickBattlePreparation();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Button_ClickSeasonReward();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void SetArenaUserInfo();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void LoadSeasonRewardListData();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void SetTextArenaHistoryAvatar();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void OnClickInformation(bool isCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void OnClickHistory(bool isCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void OnClickRanking(bool isCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Button_ClickDailyBonusAdd();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void Button_ClickDefenceReward();
	UFUNCTION(BlueprintCallable, Category = UUP_Arena_Lobby)
	void ShowRejectRequestPopupDelay();
	UFUNCTION()
	void CountResetRp(bool bResult);
	UFUNCTION()
	void CountResetStatusRp(int32 nResetCount);
};
