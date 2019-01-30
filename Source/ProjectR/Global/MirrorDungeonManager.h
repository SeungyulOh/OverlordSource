// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Object.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "MirrorDungeonManager.generated.h"

// void MIRROR_STATUS_RQ();
// void MIRROR_PLAY_START_RQ(FString mirrorUD, TArray<FString> heroUDs);
// void MIRROR_PLAY_DONE_RQ(FString playUD, int32 clearGrade, TArray<int32> heroHPs_hp, TArray<int32> target_heroHPs_hp);
// void MIRROR_RESET_RQ(FString mirrorUD, bool useGem);
// void MIRROR_DEFENSE_DECK_SET_RQ(TArray<FString> defense, int32 defenseFormation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnMirrorDungeonStatusRp);

/**
*
*/
UCLASS()
class PROJECTR_API UMirrorDungeonManager: public UObject
{
	GENERATED_BODY()

public:
	void Initialize();

	void ResetMirrorDungeonManagerData();
public:
	FORCEINLINE void SetSelectedMirrorSlotData(FMIRROR InMirror) { SelectedMirrorSlotData = InMirror; }
	FORCEINLINE const FMIRROR& GetSelectedMirrorSlotData() { return SelectedMirrorSlotData; }
	
	FORCEINLINE void SetTargetInfo(FBATTLE_USER& MatchUser) { m_MatchList = MatchUser; }
	
	FORCEINLINE void SetMirrorInfoResult(FMIRROR_STATUS& MyUserResult, int32 PVPToken) { m_MyMirrorInfoResult = MyUserResult; m_PVPToken = PVPToken; }
	FORCEINLINE const FMIRROR_STATUS& GetMirrorInfoResult() { return m_MyMirrorInfoResult; }

	FORCEINLINE bool IsMyWin() { return m_MyWin; }
	FORCEINLINE void SetMyWin(bool my_win) { m_MyWin = my_win; }

	FORCEINLINE int32 GetMyMaximumTotalHP() { return m_nMyHeroesMaxHP; }
	FORCEINLINE int32 GetTargetMaximumTotalHP() { return m_nTargetHeroesMaxHP; }

	FORCEINLINE int32 GetMyTotalHP() { return m_nMyHeroesCurrentHP; }
	FORCEINLINE int32 GetTargetTotalHP() { return m_nTargetHeroesCurrentHP; }

	void SetMirrorStatus(FMIRROR_STATUS InMirrorStatus);
	
	void SetGameResultRewards(TArray<FREWARD> InRewards)	{ m_GameResultRewards = InRewards; }
	void GetGameResultRewards(TArray<FREWARD>& InRewards)	{ InRewards = m_GameResultRewards; }

	int32 FindRemainHP(FString InHeroUD);

	void StartEnemyAI();
	void StartHeroAI();
	void StopEnemyAI();
	void StopHeroAI();

	void AttachBattleRecordManager();
	
	UFUNCTION()
	void OnBattleRecord_UpdateHP();

	void UpdateMirrorGuage();


	void SetMirrorHeroPosition(bool my_hero, int32 pos_index, FVector pos, FRotator rot);
	bool GetMyHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos);
	bool GetMatchHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos);

	bool IsProgressMirrorSlot();

	UFUNCTION()
	void RequestPlayStart();
	void RequestPlayDone();
	
	void GetMyHeroHpList(TArray<int32>& InHpList);
	void GetMyHeroMaxHPList(TArray<int32>& InMaxHPList);
	void GetEnemyHpList(TArray<int32>& InHpList);

	void MirrorDoneRP();

	void ClearMirrorDungeonInfo();
	void SetCameraModeAfterGameStart(EArenaCameraMode cameraMode);
	void InitMirrorGuage();

	void ResetCameraMode();

	void DecideWinner();
	bool IsCombatEnd();
	int32 GetAliveMyHero();
	int32 GetAliveEnemy();

// 	void GetMyHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos);
// 	void GetMatchTargetHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos);
// 	void GetMatchHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos);
	bool GetMatchTargetInfo(FBATTLE_USER& target_info);

	void SetMirrorDefenceSetting(bool bSet) { bMirrorDeckSetting = bSet; }
	bool IsMirrorDefenceSetting() { return bMirrorDeckSetting; }
public:
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MirrorDungeonManager)
	FDelegate_OnMirrorDungeonStatusRp		OnMirrorDungeonStatusRp;


public:
	FMIRROR_STATUS							MirrorStatus;
	FMIRROR									SelectedMirrorSlotData;

	// 로비 및 플레이 정보
	FString						m_MirrorPlayUD;
	FBATTLE_USER				m_MatchList;
	bool						m_IsFirstDefence = false;
	bool						m_IsDefenceSetting = false;
	TArray<FString>				m_MirrorMyHeroUDs;
	int32						m_MyPower = 0;
	int32						m_MyPosotion = 0;
	int32						m_TargetPower = 0;
	UPROPERTY()
	TArray<AActor*>				mMyHeroList;
	UPROPERTY()
	TArray<AActor*>				mEnemyList;

private:

	// 스폰 위치 정보
	TArray<FTransform>			m_MyHeroSpawnPosList;
	TArray<FTransform>			m_MatchHeroSpawnPosList;

	// 플레이 정보
	int32						m_nMyHeroesMaxHP = 0;
	int32						m_nTargetHeroesMaxHP = 0;

	int32						m_nMyHeroesCurrentHP = 0;
	int32						m_nTargetHeroesCurrentHP = 0;

	// 결과 정보
	bool						m_MyWin = false;
	FMIRROR_STATUS				m_MyMirrorInfoResult;
	int32						m_PVPToken;
	TArray<FREWARD>				m_GameResultRewards;


	

	bool						bMirrorDeckSetting = false;
};
