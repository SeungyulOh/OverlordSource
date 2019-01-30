// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Tickable.h"
#include "SharedConstants/GlobalConstants.h"
#include "ContentGameMode/Modes/RaidGameMode.h"
#include "Global/EventManager.h"
#include "Global/TableManager.h"
#include "Network/ClientProtocols.h"
#include "Network/RTSProtocol/rts.struct_generated.h"
#include "Network/RTSProtocol/rts.s2c_generated.h"
#include "RealTimeModManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnDeckSelect, FDeckSelected, DeckSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnCSDeckSelect, FCSDeckSelected, DeckSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_PartyUpdatePublic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_PartyUpdatePlayer, int32, kid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnPartyPlayerExit, int32, InSlotNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnReceiveStepAndGo, int32, InTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnReceiveRemainTime, int32, Sequence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnLeaderSlot, int32, InSlotNumber, bool, InIsLeader);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnDeckReady, int32, InSlotNumber, bool, InReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnChangeSpirit, int32, TargetUK, int32, ChangeSpiritIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnReceiveMultiRaidStat, TArray<FString>, nick, TArray<int32>, damage);
/**
 * 
 */
UCLASS()
class PROJECTR_API URealTimeModManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	static bool IsRealTimeGameMode();
	static bool IsControlWithRTS();
	void Initialize(/*URGameInstance* InGameInstance*/);
	void ClearRealTimeModData();
	// Tick 사용하기 위해서 FTickableGameObject 상속 받았음, 상속 받기 위해 정의한 함수들
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	TStatId GetStatId() const override;
	bool IsTickableWhenPaused() const override;
	float GetGameTime() const;
	// Game Timer [ RTS모드에서 게임 시간을 동기화 하기 위해서 사용 ] [ 서버에서 보내준 시간 ]
	UFUNCTION(BlueprintCallable, Category = ContentsUnLockManager)
		void SYNC_TIME_REMAIN();
	void OnACK_BT_SYNC_TIME_REMAIN(float InTimeSec, bool AddNotify);

	void OnACK_CS_DECK_TIME_REMAIN(OVERLORD::RTS::S2C::ACK_CS_DECK_TIME_REMAINT* data);
public:
	UPROPERTY()
	float	MoveTickElapsedTime = 0.0f;
	UPROPERTY()
	float   RealModeClearTime = 0.0f;
	//////////////////////////////////////////////////////////////////////////
	// RTS Network
	void ClearData();
	void Connect(class URGameInstance* instance, const FString& tcpServerHost, const FString& tcpServerPort);
	void DisConnect();
	// RTS Data Struct
	//	bool Delete_RTS_Actor(uint16 InActorUK);
	void Update_RTS_Team(OVERLORD::RTS::STRUCT::TEAMT* InData);
	FRTS_TEAM* Get_RTS_Team(uint16 InTeamID);
	void Update_RTS_Player(OVERLORD::RTS::STRUCT::PLAYERT* InData);
	FRTS_PLAYER* Get_RTS_Player(int16 InKID);
	void CreateRTSActor();
	bool Get_RTS_Actor_UKLIstByKID(int16 InUserKID, TArray<int16>& HeroUKList);
	FRTS_TEAM* GetMyTeam();
	FString GetPlayerNameByKID(int16 InPlayerKID);

	UPROPERTY()
	TMap<int16, FRTS_TEAM>			TeamList;
	UPROPERTY()
	TMap<int16, FRTS_ACTOR>			ActorList;
// 	UPROPERTY()
// 	TArray<FDeckSelected>			mCacheDecks;

	bool GetIsControllable() const;
public:
	void REQ_BT_ACTOR_MOVE(AActor* PlayerActor, bool IsStop, float DeltaTime);
	void OnACK_BT_ATTACK_NOW(OVERLORD::RTS::S2C::ACK_BT_ATTACK_NOWT* data);
	void OnACK_BT_ATTACK_NORMAL(OVERLORD::RTS::S2C::ACK_BT_ATTACK_NORMALT* data);
	void OnACK_BT_CHANGE_CREW(OVERLORD::RTS::S2C::ACK_BT_CHANGE_CREWT* Packet);
	void OnACK_BT_DROP_CREW(OVERLORD::RTS::S2C::ACK_BT_DROP_CREWT* Packet);
	void OnACK_BT_CHANGE_SPIRIT(OVERLORD::RTS::S2C::ACK_BT_CHANGE_SPIRITT* Packet);
	

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnDeckSelect				OnReceiveDeckSelect;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_PartyUpdatePublic			OnUpdatePartyPublic;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_PartyUpdatePlayer			OnUpdatePartPlayer;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnPartyPlayerExit			OnPartyPlayerExit;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnLeaderSlot				OnLeaderSlot;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnDeckReady				OnDeckReady;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnCSDeckSelect			OnReceiveCSDeckSelect;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnDeckReady				OnReceiveCSReady;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnReceiveRemainTime		OnReceiveRemainTime;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnReceiveRemainTime		OnCSDeckSelectTime;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnReceiveStepAndGo		OnCSStepAndGo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
	FDelegate_OnChangeSpirit			OnChangeSpirit;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RealTimeModManager)
		FDelegate_OnReceiveMultiRaidStat OnReceiveStatCount;
	

	void EnterWorldRaid(FName StageKey);
	void EnterColosseum(FName StageKey);
	void EnterTowerRaid(FName StageKey);
	void SetGameMode(ARaidRGameMode* inGameMode);
	void OnACK_DISCONNECTED(uint8 reason);
	void OnACK_PLAY_SEND_RESULT(OVERLORD::RTS::S2C::ACK_PLAY_SEND_RESULTT* Packet);
	void OnACK_AUTH(OVERLORD::RTS::S2C::ACK_AUTHT* Packet);
	void OnACK_MATCHED(OVERLORD::RTS::S2C::ACK_MATCHEDT* Packet);
	void OnACK_MATCH_ROUND(OVERLORD::RTS::S2C::ACK_MATCH_ROUNDT* Packet);

	void OnACK_PARTY_UPDATE_PUBLIC(OVERLORD::RTS::S2C::ACK_PARTY_UPDATE_PUBLICT* Packet);
	void OnACK_DECK_SELECT(OVERLORD::RTS::S2C::ACK_DECK_SELECTT* Packet);
	void OnACK_PARTY_PLAYER(OVERLORD::RTS::S2C::ACK_PARTY_PLAYERT* Packet);
	void OnACK_PLAYER_EXIT(OVERLORD::RTS::S2C::ACK_PARTY_PLAYER_EXITT* Packet);
	void OnACK_LEADER_SLOT(OVERLORD::RTS::S2C::ACK_PARTY_LEADER_SLOTT* Packet);
	void OnACK_DECK_SELECT_READY(OVERLORD::RTS::S2C::ACK_DECK_SELECT_READYT* Packet);
	void OnACK_PARTY_GO(OVERLORD::RTS::S2C::ACK_PARTY_GOT* Packet);


	void Update_RTS_Actor(OVERLORD::RTS::STRUCT::ACTORT* InData);
	FRTS_ACTOR* Get_RTS_Actor(uint16 InActorUK);
	void OnACK_BT_ACTOR_UPDATE(OVERLORD::RTS::S2C::ACK_BT_ACTOR_UPDATET* Packet);
	void OnACK_BT_SYNC_ACTOR_UPDATE(OVERLORD::RTS::S2C::ACK_BT_SYNC_ACTOR_UPDATET* data);
	void OnMapLoadingComplete();
	void OnREQ_CINEMATIC_PLAY(OVERLORD::RTS::S2C::REQ_CINEMATIC_PLAYT* Packet);
	void OnACK_CINEMATIC_DONE(OVERLORD::RTS::S2C::ACK_CINEMATIC_DONET* Packet);
	void OnACK_PLAY_READY(OVERLORD::RTS::S2C::ACK_PLAY_READYT* Packet);
	void OnREQ_STEP_READY(OVERLORD::RTS::S2C::REQ_STEP_READYT* Packet);
	void OnACK_STEP_GO(OVERLORD::RTS::S2C::ACK_STEP_GOT* Packet);
	void OnACK_BT_ACTOR_CREATE(OVERLORD::RTS::S2C::ACK_BT_ACTOR_CREATET* Packet);
	void OnACK_DECK_CACHED_DONE(OVERLORD::RTS::S2C::ACK_DECK_CACHED_DONET* Packet);
	void OnACK_BT_ACTOR_SPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_SPAWNT* Packet);
	void OnACK_BT_ACTOR_RESPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_RESPAWNT* Packet);
	void OnACK_PLAY_START(OVERLORD::RTS::S2C::ACK_PLAY_STARTT* Packet);
	void OnACK_BT_ACTOR_DESPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_DESPAWNT* Packet);
	void OnACK_BT_ACTOR_DISAPPEAR(OVERLORD::RTS::S2C::ACK_BT_ACTOR_DISAPPEART* Packet);
	//	Coloseum
	void OnACK_CS_MATCHED(OVERLORD::RTS::S2C::ACK_CS_MATCHEDT* Packet);
	void OnACK_CS_DECK_SELECT(OVERLORD::RTS::S2C::ACK_CS_DECK_SELECTT* Packet);
	void OnACK_CS_DECK_SELECT_READY(OVERLORD::RTS::S2C::ACK_CS_DECK_SELECT_READYT* Packet);
	
	//
	void OnACK_MR_PLAYER_SCORE(OVERLORD::RTS::S2C::ACK_BT_SYNC_PLAYER_SCORET* Packet);

	UPROPERTY()
	ARaidRGameMode*					RaidGameMode = nullptr;
	UPROPERTY()
	TArray<FString>					HeroSelectedList;
	UPROPERTY()
	FName							PlayStageID;
	UPROPERTY()
	FString							mRoomKey;
	FRaidStageTableInfo*			RaidStageTableInfo;
//	FWorldBossStageInfo*			WorldRaidStageInfo;
	UPROPERTY()
	FString							JoinNumberString;
	UPROPERTY()
	bool							bIsControlable = false;
	UPROPERTY()
	FRTSInfo						RTSInfo;
	UPROPERTY()
	TMap<int16, FRTS_PLAYER>		RTSPlayerList;
	UPROPERTY()
	class URGameInstance*			GameInstance = nullptr;
	UPROPERTY()
	FString							mTCPTargetHost;
	UPROPERTY()
	FString							mTCPTargetPort;
// 	UPROPERTY()
// 	bool							bIsCachedDone = false;
	UPROPERTY()
	FREALTIME_BG_REWARD				RealTimeModReward;
	UPROPERTY()
	TMap<int16, FREALTIME_REWARD>	KillBossRewardItem;
	UPROPERTY()
	TArray<int16>					DeadBossProgress;
	OVERLORD::RTS::ENUM::RTSPlayResult	RealTimePlayResult;
	UPROPERTY()
	FText					DiceOwnerName;
	UPROPERTY()
	FText					DiceRewardName;
	UPROPERTY()
	FREALTIME_REWARD		DiceRewardData;
	UPROPERTY()
	int16					MyKillPlayerUK = 0;
	UPROPERTY()
	float					GameTimeSec = 0;

	//
	TArray<FString> MS_Score_NickList;
	TArray<int32> MS_Score_List;
private:
	UPROPERTY()
	FTimerHandle			TimeHandler_DeckSetting;
	UPROPERTY()
	FTimerHandle			TimeHandler_RemainGame;
	UPROPERTY()
	FTimerHandle			TimeHandler_RemainTimeNotify;
public:
	void OnACK_PLAY_TIMEOUT(OVERLORD::RTS::S2C::ACK_PLAY_TIMEOUTT* Packet);
	void OnExitGame();
	UFUNCTION()
	void GoToLobby();
	UFUNCTION()
	void ChangeCrew(ESkillOrigin CrewNo, FVector _Location, FVector _Direction);
	UFUNCTION()
	void DropCrew(ESkillOrigin CrewNo, FVector _Location, FVector _Direction);

	UFUNCTION()
	void Deferred_ShowIngameHUD();


	UFUNCTION()
	void ChangeSPIRIT(int32 nSpiritIndex);
	UFUNCTION()
	void OnSequenceEndColosseumStart(FName InSequenceKey);
	//temp
	void SetRTSResults(const TArray<FREWARD>& Reward);
	FORCEINLINE TArray<FREWARD>	GetRTSResults() { return m_ResultReward; }
	FORCEINLINE uint8 GetCinematicSequence() { return CinematicSequence; }

	void RTSKillMsg(FRTS_ACTOR* InKiller, FRTS_ACTOR* InDie);
	bool IsKillMsg(FRTS_ACTOR* InKiller, FRTS_ACTOR* InDie);

	ESkillOrigin CrewChangeProcess = ESkillOrigin::VE_Leader;
	UPROPERTY()
	int32		 SpiritChangeProcess = -1;
	private:
		TArray<FREWARD>		m_ResultReward;
		FAVATAR				m_ResultAvatar;
public:
	void OnACK_BT_DOOR_EVENT(OVERLORD::RTS::S2C::ACK_BT_DOOR_EVENTT* data);

private:
	int64								nCheckTimeSeconds = 1;
	uint8								CinematicSequence;

};