// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "Table/LobbyCharacterTableInfo.h"
#include "EventManager.generated.h"


//class URGameInstance;
class ABasePlayerController;

//	Event delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_NoParam);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamActor, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamInt32, int32, Int);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamUInt16, uint16, UInt16);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamUInt32, uint32, UInt32);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamInt32AndBool, int32, Int, bool, bValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamActorAndfloat, AActor*, Actor, float, fValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamFloat, float, InFloat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamBool, bool, IsValid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamFString, FString, StringKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamFNameValue, FName, StrName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamFTextValue, FText, TextValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamActorAndFTextValue, AActor*, Actor, FText, TextValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamOneTouchAction, ABasePlayerController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamWidget, EWidgetBluePrintEnum, openWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamFStringAndBool, FString, StringKey, bool, bValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamVector2D, FVector2D, Position1, FVector2D, Position2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamAvatarSkill, UUC_AvatarSkill*, InSelectSkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamFStringAndFString, FString, nick, FString, message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamFStringNInt, FString, stringParam, int32, intParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDelegate_OnOpenGradePop, uint8, Grade, int32, kid, FString, Nick);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_SelectingOnDeckPage, URScrollItem*, InScrollItem, bool, bTrue);



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnCrewDead, int32, SlotIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnDelayNextStage, EGameModeStateEnum, InDelayedNextModeState, float, InDelayTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnTwoParamActor, AActor*, SourceActor, AActor*, DestActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnSkillJoystick, bool, IsVisible, int32, SkillIconIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnSlotAction, int32, HeroSlotIndex, AActor*, SelectedCharacter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDelegate_OnSkillIconClick, int32, CommonSlotIndex, FString, SkillID, ESkillCategoryTypeEnum, SkillCategoryType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDelegate_PartyMemberEvent, int32, InPartyIndex, int32, InSlotIndex, bool, InValidData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamActorArray, TArray<AActor*>, ActorList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamFStringArray, TArray<FString>, InHeroUDList);
//	Dialogue delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueDelegate_OnOneParamFName, FName, Name );

// cast skill
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDelegate_OnTrySkillCasting, AActor*, Caster, FName, SkillID, AActor*, Target, FVector, Location);

// Input
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnInputAttackValue, EInputCommandTypeEnum, InputCommand);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDelegate_OnInputSkillName, EInputCommandTypeEnum, InputCommand, int32, SkillIndex, FName, SkillName, int32, CrewIdx);

// Raid
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_RaidNotify, FName, Key);

// Inventroy selected item
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_InventorySelectItemIconInfo, FITEM_ICON_INFO, ItemIconInfo);

//	Campaign Quest
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnRScrollItemData, URScrollItemData*, ScrollItemData);

//	Media Framework
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_MediaFrameWork, FString, Url);


//	Battle
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnBuffEnter, AActor*, InActor, int32, InSlotType);


//	Boss Shield
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_ShieldNotify,float,PowerPercent,bool,InIsTimeOut);


//	Hero Shop
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_HeroShopRefresh, FHERO_SHOP_PANEL, InShopPanel);


//	Episode
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_CampaignEpisodeClearPopup, FName, EpisodeID, int32, EpisodeIdx);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnSkillUse, ESkillCategoryTypeEnum, InSkillCategoryType);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnSpawnCharacter, int32, HeroSlotIndex, AActor*, SpawnedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnSelectCharacter, int32, HeroSlotIndex, AActor*, SelectedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDelegate_OnTwoPointInput_Battle, FVector2D, ScreenPosition1, FVector2D, ScreenPosition2, EInputEvent, InputType);

// hero
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnHeroInvenUpdate, FString, HeroHashKey);
//Item Unlocked
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnItemSlotAdd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDelegate_OnEquipItem, FString, InHeroKey, EItemEquipPositionEnum, InEquipPosition, FString, InItemInventoryKey, FString, OldItem);

//	Quest
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_SetDailyQuestStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnQuestRewardRp, const TArray<FREWARD>&, rewards);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_GrindRewardRp, const TArray<FREWARD>&, rewards);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_RuneItemSetItemUD, FString&, HeroHashKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnCrewChange, ECharacterRoleEnum, eType, int32, nCrewOrder);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnActorGetDamge, AActor*, actor, float, damage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnHttpRp_GetRaidStageAll);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_GetRaidStagePlay, FString, RaidStageID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_HeroEnchant, TArray<bool>&, Result, int32, InCurrentPoint);

UCLASS()
class PROJECTR_API UEventManager : public UObject
{
	GENERATED_BODY()

public:
	void		Initialize(void);
	void		ClearEvent();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamActor		OnBossSpawn;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamActor		OnSpawnSequence;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamActorArray	OnMultiBossSpawn;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnTwoParamActor		OnDeadActor;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamActor		OnSpawnActorFromCharacterSpawner;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnBossGuageHide;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_ShieldNotify			OnBossShieldNotify;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnLevelPhaseChange;

	//	Game play state event
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnGameMode_ReadyToStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnGameMode_ReadyToResult;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnGameMode_Ready;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnMiddleBarRefersh;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamInt32		OnTowerBossRefersh;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnCrewDead			OnCrewDead;

	//	Matinee Event
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnNewHeroSequence_AnimationStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnNewHeroSequence_AnimationEnd;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnSequence_CreateWidget;

	//	UserInfo Event
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnUpdateUserInfo;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnUpdateUserRecord;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnUpdateAvatarIcon;

	//	Raid
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_RaidNotify			OnRaidNotify;
	


	//	UI Campaign ResultEvent
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnCampaignResultEvent;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnLobbyEnterUIEvent;

	//	UI Quest Guide
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnQuestGuideRewardRp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamInt32		OnUpdateCampaignUIPage;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnUpdateTowerBossUIPage;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamFNameValue	OnIndicator;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRefreshAutoSkillIcon;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamInt32		OnHUD_SetOwner;


	//	UI Battle
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnUI_ReadyToStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamFString		OnPhaseEvent;


	//	UI Guild
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRequestGuildInfo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRefreshGuildInfoUI;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRequestGuildMember;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamBool		OnRefreshGuildMemberUI;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRefreshGuildAttendanceUI;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRefreshGuildDonationUI;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnReceiveGuestGuildInfo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_OnOpenGradePop	OnOpenGradeChangePop;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_OnTwoParamFStringNInt AfterChangeGrade;

	//	Guild Raid
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRefreshGuildRaidInfo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRefreshGuildRaidRealServerInfo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamInt32		OnSelectDifficulty;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnRefreshGuildRaidHistory;
	//UI Topbar
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
// 	FDelegate_OnOneParamBool		OutMenuAnimPlay;

	//UI Lobby
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				TowerStatusResponse;
	//receive data about chat
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_OnOneParamBool		SetChatMinimalize;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_OnOneParamFString Receive_CurrentChannel;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_NoParam Receive_Message;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_NoParam Receive_UserInOut;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_OnTwoParamFStringAndFString UpdateLastMessage;
	UPROPERTY()
		FDelegate_NoParam GetNoMissionSignal;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnSkillJoystick		OnSkillJoystick;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnShowDashBoard;

	//	Battle
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnBuffEnter			OnBuffEnter;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamBool		OnSynergyEnter;

	//	Spawner
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnSetUpSpawner;

	//	UI ImageFlow Move Button Touch
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnClick_AnotherImage;

	//	UI QuickToggleTouch
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_OnOneParamInt32		OnClick_QuickToggle;

	//	UI HeroShop
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnHeroShopListRp;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_HeroShopRefresh		OnHeroShopRefreshRp;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnHeroShopBuyRp;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam			OnSuccess_HeroSummon;

	// UI Login
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnUpdatePage;

	// Packet RP Notify
// 	FDelegate_NoParam				OnRTS_CS_DECK_SCENE_LOADED_RP;
// 	FDelegate_NoParam				OnRTS_CS_DECK_SET_DONE_RP;
// 	FDelegate_NoParam				OnRTS_PLAY_READY_DONE_RP;
// 	FDelegate_NoParam				OnRTS_BT_ACTOR_SPAWN_RP;
// 	FDelegate_NoParam				OnRTS_PLAY_START_RP;
// 	FDelegate_NoParam				OnRTS_PLAY_TIMEOUT_RP;
// 	FDelegate_NoParam				OnRTS_PLAY_SEND_RESULT_RP;
//	FDelegate_OnOneParamUInt16		OnSpawn_RTS_Actor;
//	FDelegate_OnOneParamUInt16		OnDisappear_RTS_Actor;
//	FDelegate_OnOneParamInt32		OnDisappear_RTS_Player;

	// RealTime
// 	FDelegate_OnOneParamFTextValue	OnRealTimeModNotify;
// 	FDelegate_OnOneParamBool		OnModRoomDamageMeter;
	
	// Target hpbar
	FDelegate_OnOneParamActor		OnTargetChange;

	UPROPERTY()
	FDelegate_NoParam	OnHideDialogueEvent;
	UPROPERTY()
	FDelegate_NoParam	OnEndMediaEvent;

	UPROPERTY()
	FDelegate_OnInputSkillName			OnInputSkillNameDelegate;

	//	HeroManagement
	UPROPERTY()
	FDelegate_OnTwoParamInt32AndBool				OnClickItem_ItemEquipPosition_HeroManagement;
 	UPROPERTY()
	FDelegate_NoParam								OnSkillUpgradeSuccess;
	UPROPERTY()
	FDelegate_NoParam								OnClick_AutoEquipItem_HeroManagement;
	UPROPERTY()
	FDelegate_OnOneParamBool						OnClick_SortingFIlter;
	UPROPERTY()
	FDelegate_OnOneParamBool						OnClick_CheckFIlter;
	UPROPERTY()
	FDelegate_NoParam				OnClick_SkillManagement;
	UPROPERTY()
	FDelegate_OnOneParamInt32		OnClick_SkillDetail_HeroManagement;

	//	 MediaFrameWork - MoviePlayer
	UPROPERTY()
	FDelegate_MediaFrameWork		OnMediaFrameWorkMediaPlay;
	UPROPERTY()
	FDelegate_MediaFrameWork		OnMediaFrameWorkMediaStop;
	UPROPERTY()
	FDelegate_MediaFrameWork		OnMediaFrameWorkMediaClose;
// 	UPROPERTY()
// 	FDelegate_MediaFrameWork		OnMediaFrameWorkRequestToPlay;
	UPROPERTY()
	FDelegate_NoParam				OnBeforeFadeIn_Delegate;


	//	Special Effects
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnShowGachaUMG;

	//	AutoPlay
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnAutoPlayCoolTime;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
	FDelegate_NoParam				OnClickAutoPlayOptionOK;

	// toggle camera icon
	FDelegate_OnOneParamInt32		OnClickToggleCameraIcon;

	//	Avatar skill up
	FDelegate_OnOneParamAvatarSkill		OnClickSkillIcon;
	FDelegate_OnOneParamAvatarSkill		OnRefreshSkillInfo;
	FDelegate_NoParam					OnRequestReceive;

	// 리더가 움직이고 난 후에 크루ai 활성화 하기 위한 delegate
	FDelegate_NoParam			OnLeaderMove;

	// 평타중첩 적중횟수 갱신 이벤트
	FDelegate_OnOneParamActor	OnUpdateAttackCount;

	//	스폰 블루 프린트 전용
//	FDelegate_OnOneParamActor	OnSpawnBoxActorDead;
//	FDelegate_OnActorGetDamge	OnSpawnBoxActorGetDamage;
	//	스폰 블루 프린트 전용


	// combine스킬중 hpbar 안보이게
	FDelegate_OnOneParamBool	OnHpBarBoxShow;

	// HeroLevelUp
	FDelegate_NoParam			OnHeroLevelUp;
	// HeroGrind
	FDelegate_OnQuestRewardRp	OnHeroGrind;
	FDelegate_GrindRewardRp		AfterHeroGrind;
	// HeroGrind
	FDelegate_NoParam			OnHeroLock;
	// HeroEnchant
	FDelegate_HeroEnchant		OnHeroEnchant;
	FDelegate_NoParam			OnHeroStatApply;
	// HeroUpgrade
	FDelegate_NoParam			OnHeroUpgrade;
};