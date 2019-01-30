// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"

#include "UI/UIDefs.h"

#include "Network/PacketDataStructures.h"

#include "GameFramework/Actor.h"

#include "ArenaManager.generated.h"

UCLASS()
class PROJECTR_API UArenaManager : public UObject
{
	GENERATED_BODY()

public:
 	void Initialize();
 	void ResetArenaManagerData();

 	bool GetMatchTargetInfo(FBATTLE_USER& target_info);

 	void SetRankerList(TArray<FARENA_RANK_USER>& user_list);

	FORCEINLINE void				SetArenaInfo(FARENA_STATUS& MyUser) {m_MyArenaInfo = MyUser;}
	FORCEINLINE void				SetArenaInfoResult(FARENA_STATUS& MyUserResult, int32 PVPToken) {m_MyArenaInfoResult = MyUserResult; m_PVPToken = PVPToken;}
	FORCEINLINE void				SetTargetInfo(FBATTLE_USER& MatchUser) { m_MatchList = MatchUser; bMirrorDeckSetting = false; }
	FORCEINLINE void				SetDefenceSetting(bool isDenfence = false) { m_IsDefenceSetting = isDenfence; }
	FORCEINLINE void				SetMyWin(bool my_win) { m_MyWin = my_win; }
	FORCEINLINE void				SetReward(FAVATAR& InAvatarResult, TArray<FREWARD>& InRewardsItem) {m_AvatarResult = InAvatarResult; m_RewardsItem = InRewardsItem;}

	FORCEINLINE FARENA_STATUS&		GetArenaInfo() { return m_MyArenaInfo; }
	FORCEINLINE FARENA_STATUS		GetArenaInfoResult() { return m_MyArenaInfoResult; }
	FORCEINLINE int32				GetArenaInfoPVPToken() { return m_PVPToken; }

	FORCEINLINE bool				isDefenceSetting() { return m_IsDefenceSetting; }
	FORCEINLINE bool				IsMyWin() { return m_MyWin; }
	bool							IsDefenceReward() { return m_MyArenaInfo.defenseCountMax <= m_MyArenaInfo.defenseCount; }

	FORCEINLINE FAVATAR&			GetResultAvatar() { return m_AvatarResult; }
	FORCEINLINE TArray<FREWARD>		GetResultReward() { return m_RewardsItem; }

	void			ArenaLobbyUpdate();
	void			ArenaDeckSeetingUpdate();

	void SetCameraModeAfterGameStart(EArenaCameraMode cameraMode);

	UFUNCTION()
 	void RequestPlayStart();


 	void RequestPlayDone();
 	void RequestRankigList();
 	void RequestStatus();
	void RequestTargetInfo();
	void RequestSeasonDone();
	void RequestDefenceReward();
	void RequestDefenceReset();
// 
 	void SetArenaHeroPosition(bool my_hero, int32 pos_index, FVector pos, FRotator rot);
 	bool GetMyHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos);
 	bool GetMatchHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos);

	void AttachBattleRecordManager();
	UFUNCTION()
	void OnBattleRecord_UpdateHP();

	void InitArenaGuage();
	void UpdateArenaGuage();

 	FORCEINLINE void SetArenaPlayUD(const FString& play_us) { m_ArenaPlayUD = play_us; }

	FORCEINLINE int32 GetMyMaximumTotalHP()		{ return m_nMyHeroesMaxHP; }
	FORCEINLINE int32 GetMyCurrentTotalHP()		{ return m_nMyHeroesCurrentHP; }
	FORCEINLINE int32 GetTargetMaximumTotalHP() { return m_nTargetHeroesMaxHP; }
	FORCEINLINE int32 GetTargetCurrentTotalHP() { return m_nTargetHeroesCurrentHP; }

 	void DecideWinner();
	void ResetCameraMode();
	void ResetCamera();
	bool IsCombatEnd();
	int32 GetAliveMyHero();
	int32 GetAliveEnemy();
	
	void StartEnemyAI();
	void StartHeroAI();
	void StopEnemyAI();
	void StopHeroAI();
	void ClearArenaInfo();
	void ArenaDoneRP();

	void SetArenaHeroPosition();


	// 아레나 로비 및 플레이 정보
	UPROPERTY()
	FString						m_ArenaPlayUD;
	UPROPERTY()
	FARENA_STATUS				m_MyArenaInfo;
	UPROPERTY()
	FBATTLE_USER				m_MatchList;
	UPROPERTY()
	TArray<FARENA_RANK_USER>	m_RankList;
	UPROPERTY()
	TArray<FARENA_HISTORY>		m_History;
	UPROPERTY()
	bool						m_IsFirstDefence = false;
	UPROPERTY()
	bool						m_IsDefenceSetting = false;
	UPROPERTY()
	TArray<FString>				m_ArenaMyHeroUDs;
	UPROPERTY()
	int32						m_MyPower = 0;
	UPROPERTY()
	int32						m_TargetPower = 0;
	UPROPERTY()
	TArray<AActor*>				mMyHeroList;
	UPROPERTY()
	TArray<AActor*>				mEnemyList;

	DECLARE_DELEGATE(FACK_ARENA_STATUS)
	FACK_ARENA_STATUS onACK_ARENA_STATUS;

private:

	// 아레나 스폰 위치 정보
 	UPROPERTY()
 	TArray<FTransform>			m_MyHeroSpawnPosList;
 	UPROPERTY()
 	TArray<FTransform>			m_MatchHeroSpawnPosList;

	// 아레나 플레이 정보
	UPROPERTY()
	int32						m_nMyHeroesMaxHP = 0;
	UPROPERTY()
	int32						m_nTargetHeroesMaxHP = 0;
	UPROPERTY()
	int32						m_nMyHeroesCurrentHP = 0;
	UPROPERTY()
	int32						m_nTargetHeroesCurrentHP = 0;

	// 아레나 결과 정보
 	UPROPERTY()
 	bool						m_MyWin = false;
	UPROPERTY()
	FARENA_STATUS				m_MyArenaInfoResult;
	UPROPERTY()
	int32						m_PVPToken;

	UPROPERTY()
	FAVATAR						m_AvatarResult;
	UPROPERTY()
	TArray<FREWARD>				m_RewardsItem;


public:
	bool	m_bIsAlreadyPlayArena = false;
	bool	bMirrorDeckSetting = false;
};
