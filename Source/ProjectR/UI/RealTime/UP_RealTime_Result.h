// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/RTSProtocol/rts.struct_generated.h"
#include "UP_RealTime_Result.generated.h"

class UUC_ResultStatistics;
class UUC_ResultEffect;
class ULocalizingTextBlock;
class UUC_Item_Icons;
/**
 * BP: UP_Result_RealTime
 * WBP_RealTimeResult
 */
UCLASS()
class PROJECTR_API UUP_RealTime_Result : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = UUP_RealTime_Result)
	void OnClick_GotoLobby();

	UFUNCTION(BlueprintCallable, Category = UUP_RealTime_Result)
	void OnClick_BattleRecord();
	void SetGameEndTime();
	void RefreshResultState();
	void RefreshRewardData();
	void RefreshRewardIcon(TArray<UUC_Item_Icons*>& ItemIconList, TArray<FITEM_ICON_INFO>& RewardsItemIconInfo);
	void RefreshRankData();
	void BattleGroundRankData();
	void BattleGroundMember(OVERLORD::RTS::STRUCT::PLAYER_SCORET& PlayerInfo, UPanelWidget* PanelRankList, UPanelWidget* PanelMember, UWidgetSwitcher* SwicherMyData,
		UTextBlock* MemberName, UTextBlock* MemberGuild, UTextBlock* MemberPoint, UTextBlock* MemberKillCount);
	void GuildRaidRankData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UWidgetSwitcher*			FirstTitleSwitcher = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UWidgetSwitcher*			FirstItemSwitcher = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UUC_Item_Icons*>		FirstItemIconList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UWidgetSwitcher*			SecondTitleSwitcher = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UWidgetSwitcher*			SecondItemSwitcher = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UUC_Item_Icons*>		SecondItemIconList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	ULocalizingTextBlock*		Text_RewardDesc = nullptr;


	//Member Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UPanelWidget*>		Panel_BlueTeamRankList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UPanelWidget*>		Panel_BlueTeamList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UWidgetSwitcher*>	Swicher_BlueTeamMyDataList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UTextBlock*>			Text_BlueTeamNameList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UTextBlock*>			Text_BlueTeamGuildNameList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UTextBlock*>			Text_BlueTeamPointList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UTextBlock*>			Text_BlueTeamKillCountList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	TArray<UPanelWidget*>		Panel_GuildRaidHideDataList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	ULocalizingTextBlock*		Text_RankChangeName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UPanelWidget*				Panel_BlueTeamMyData = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UTextBlock*					Text_BlueTemMyRanking = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UUC_ResultStatistics*		BattleStatistics = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RealTime_Result)
	UUC_ResultEffect*			ResultEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*					TextBlock_Min = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*					TextBlock_Sec = nullptr;
private:
	int32 MyPlayerUK;
};
