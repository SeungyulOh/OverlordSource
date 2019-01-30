// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Tickable.h"
#include "Engine/GameInstance.h"

#include "UI/UIDefs.h"
#include "SharedConstants/GlobalConstants.h"
#include "GameState/IGameState.h"
#include "Global/EditorManager.h"
#include "Object/BaseCameraActor.h"
#include "CustomStructures/GameEnvironmentVar.h"
#include "SharedConstants/BattleLogicConstants.h"
#include "RGameInstance.generated.h"

/**
 * 
 */


UCLASS()
class PROJECTR_API URGameInstance : public UGameInstance	//, public FTickableGameObject
{
	GENERATED_BODY()

public:
	URGameInstance(const FObjectInitializer& ObjectInitializer);

	static class URGameInstance* GetRGameInstance(class UObject* OutterOwner);

	void Init() override;
	void Shutdown() override;

	void SetupForFirstExecute();
	void EtcSetting();
	// Init before login
	UFUNCTION(BlueprintCallable, Category = RGameInstance)
	void InitBeforeLaunch();

	// Init after loading prepare game scene..
	UFUNCTION(BlueprintCallable, Category = RGameInstance)
	void InitAfterLaunch();

 	UFUNCTION(BlueprintImplementableEvent, Category = RGameInstance)
 	void OnInitComplete();

	/*
	Game states
	*/
	void						InitGameStates();
	void						SetState(EGameStateEnum State, bool bForce = false);
	void						CheckNextState();

	/*
	Load Level
	*/
	void CurrentStateBeginState();

	/*
	EditorManager
	*/
	UFUNCTION(BlueprintCallable, Category = RGameInstance)
	void		CreateEditorManager(EEditorEnum editorType);
	UFUNCTION(BlueprintCallable, Category = RGameInstance)
	void		DestoryEditorManager();
	UFUNCTION(BlueprintCallable, Category = RGameInstance)
	void		EditorRefresh();
	
	/*
	Tower Lobby
	shortcut functions
	*/
	class ATowerLobbyGameMode* GetTowerLobbyGameMode();
	//class AIndoorLobbyStateActor* GetIndoorStateActor();
	//class AOutdoorLobbyStateActor* GetOutdoorStateActor();

	FORCEINLINE bool GetIsVictoryState()				{ return IsVictoryState; }
	FORCEINLINE void SetIsVictoryState(bool InState)	{ IsVictoryState = InState; }
	FORCEINLINE bool GetIsResetSkill() { return IsResetSkill; }
	FORCEINLINE void SetIsResetSkill(bool InValue) { IsResetSkill = InValue; }

	/*
	Time and Date
	*/
	UPROPERTY()
	FDateTime								CurrentDateTime;
	UPROPERTY()
	FDateTime								CurrentUTCDateTime;
	UPROPERTY()
	FTimespan								UTC_DiffTimespan;
	UPROPERTY()
	FDateTime								ServerDateTime;

	ERaidDayType							GetRaidDayType();
public:
	//////////////////////////////////////////////////////////////////////////
	// GameInfo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RGameInstance)
	class URInventory*							RInventory = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RGameInstance)
	//UItemInventory*							RItemInventory = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RGameInstance)
	class UPlayStateInfo*							PlayStateInfo = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RGameInstance)
	class URewardInfo*							RewardInfo = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RGameInstance)
	class UBaseStageInfo*							BaseStageInfo = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RGameInstance)
	class UUserInfo*								UserInfo = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RGameInstance)
	class UGuildInfo*								GuildInfo = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UPartyManager*							PartyManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UOtherHeroManager*						OtherHeroManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UForceTutorialManager*					ForceTutorialManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UResourceCacheManager*					ResourceCacheManager = nullptr;

	//////////////////////////////////////////////////////////////////////////
	// Contents
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UArenaManager*							ArenaManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UColosseumManager*						ColosseumManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UMirrorDungeonManager*				MirrorDungeonManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class URealTimeModManager*					RealTimeModManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UCampaignManager*						CampaignManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UHeroRunekDungeonManager*				HeroRuneDungeonManager = nullptr;

// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
// 	class URaidManager*							RaidManager = nullptr;
	
	//////////////////////////////////////////////////////////////////////////
	// Content System
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UBattleRecordManager*					BattleRecordManager = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UPlayModeManager*						PlayModeManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UAchievementManager*					AchievementManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UMailManager*							MailManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UNotifyManager*							NotifyManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UQuestManager*							QuestManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UContentsUnLockManager*					ContentsUnLockManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UShopManager*							ShopManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UAutoPlayManager*						AutoPlayManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UNotifyEventList*						NotifyEventList = nullptr;

//#if SHOW_TOOLTIP == 1	// 임시로 작동 중지 시킴
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
// 	class UTooltipEventList*						TooltipEventList = nullptr;
//#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UAttendanceManager*						AttendanceManager = nullptr;


	//////////////////////////////////////////////////////////////////////////
	// System
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UEditorManager*							EditorManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UEventManager*							EventManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class ULoadingSceneManager*					LoadingSceneManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UObjectPoolManager*						ObjectPoolManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UGameOptionManager*						OptionManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UGameDataManager*						GameDataManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UPacketErrorManager*					PacketErrorManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class USequenceManager*						SequenceManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UServerInfoUpdateManager*				ServerInfoUpdateManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UInputManager*							InputManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UNetSkillManager*						NetSkillManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UChatManager*							ChatManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UGridMapManager*						GridMapManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UBillingManager*						BillingManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UUIMediaFrameWorkManager*				UIMediaFrameWorkManager = nullptr;


	//////////////////////////////////////////////////////////////////////////
	// Network
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UHttpRequestClient*						HttpClient;
	
	//////////////////////////////////////////////////////////////////////////
	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class URWidgetManager*						RWidgetManager = nullptr;

	//////////////////////////////////////////////////////////////////////////
	// Util
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UConsoleCheatManager*					ConsoleCheatManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class URPointLightManager*					PointLightManager = nullptr;

	//////////////////////////////////////////////////////////////////////////
	// abnormal table info manager	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UAbnormalStateGroupTable_V2*			AbnormalStateGroupTable_V2 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class UMissionManager*						MissionManager;

	//////////////////////////////////////////////////////////////////////////
	// UserInfoSetting(.ini)
	UPROPERTY()
	class URGameUserSetting* RGameUserSetting = nullptr;


	UPROPERTY()
	class URaidManager*							RaidManager = nullptr;

	UPROPERTY()
	class URSaveGame*							SaveGame = nullptr;
	//temp

public:



	UPROPERTY()
	TMap< EGameStateEnum, UIGameState* >	GameStates;
	EGameStateEnum							NextState;


	/*
	Game help object ref..
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	class ABaseCameraActor*						CameraActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RGameInstance)
	FGameEnvironmentVar						GameEnvironmentVar;

	UPROPERTY()
	FBattleLogicConstants					BattleLogicConstants;

	UPROPERTY()
	class UCharacterBattleInfo*					BattleInfo = nullptr;

	bool IsViewAwaken = false;
	// temp
private:
	//temp
	bool									IsAlreadyInitBeforeLaunch = false;
	bool									IsAlreadyInitAfterLaunch = false;
	bool									IsVictoryState = false;
	bool									IsResetSkill = false;


public:

	void		Logout();
	URSaveGame* GetSaveGame();

	/*
	Game States
	*/
	UFUNCTION(BlueprintCallable, Category = RGameInstance)
	void	ChangeState(EGameStateEnum Next, bool bForce = false);

	EGameStateEnum	GetCurrentStateType();
	FORCEINLINE UIGameState*	GetCurrentState() { return CurrentState; }
	bool IsAutoPlayMode();
	void SetAutoPlayCount(int32 InCount);

	//Parameters for the result UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	bool									IsSuccess = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	bool									IsTimeOut = false;
	UPROPERTY()
	EStageFailReason CampaignFailReson = EStageFailReason::VE_NONE;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	EGameStateEnum							CurState = EGameStateEnum::VE_None;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	EGamePlayModeTypeEnum							CurrentGameMode = EGamePlayModeTypeEnum::VE_None;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	UIGameState*							CurrentState = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	bool									IsEditorMode = false;

	/*UClass Reference Capture BP*/
	UPROPERTY()
	UClass* CaptureBlueprintActor;
	UPROPERTY()
	UClass* CaptureBlueprint;
// 	UPROPERTY()
// 	FTimerHandle TimerHandler;
// 	UPROPERTY()
// 	FTimerHandle ProfileHandler;
};