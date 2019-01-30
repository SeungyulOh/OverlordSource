// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "SharedConstants/GlobalConstants.h"

#include "CustomStructures/SpawnStructures/SpawnerInfo.h"

#include "Engine/StaticMeshActor.h"


#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameState/Stage/Spawner/SpawnBox.h"
#include "UI/Dialogue/UP_Dialogue.h"

#include "GamePlayBluePrintFunction.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMemberMultiplierCategory : uint8
{
	VE_NONE					UMETA(DisplayName = "NONE"),
	VE_Tutorial				UMETA(DisplayName = "Tutorial"),
	VE_Normal 				UMETA(DisplayName = "Normal"),
	VE_Elite 				UMETA(DisplayName = "Elite"),
	VE_Boss					UMETA(DisplayName = "Boss"),
	VE_BossEP1				UMETA(DisplayName = "Boss_Ep1"),
	VE_Epic					UMETA(DisplayName = "Epic"),
	VE_Protect				UMETA(DisplayName = "Protect"),
	VE_Helper				UMETA(DisplayName = "Helper"),
	VE_HeroTemp				UMETA(DisplayName = "HeroTemp"),
	VE_RaidAkhan			UMETA(DisplayName = "Raid_Akhan"),
	VE_RaidGigant			UMETA(DisplayName = "Raid_Gigant"),
	VE_RaidMors				UMETA(DisplayName = "Raid_Mors"),
	VE_GRaidStandard		UMETA(DisplayName = "GRaidStandard"),
	VE_BossTanker			UMETA(DisplayName = "Boss_Tanker"),
	VE_BossMeleeDealer		UMETA(DisplayName = "Boss_MeleeDealer"),
	VE_BossRangDealer		UMETA(DisplayName = "Boss_RangeDealer"),
	VE_BossMagicDealer		UMETA(DisplayName = "Boss_MagicDealer"),
	VE_BossSupporter		UMETA(DisplayName = "Boss_Supporter"),
	VE_BossHero				UMETA(DisplayName = "Boss_Hero"),
	VE_EpicTanker			UMETA(DisplayName = "Epic_Tanker"),
	VE_EpicMeleedealer		UMETA(DisplayName = "Epic_MeleeDealer"),
	VE_EpicRangeDealer		UMETA(DisplayName = "Epic_RangeDealer"),
	VE_EpicMagicDealer		UMETA(DisplayName = "Epic_MagicDealer"),
	VE_EpicSupporter		UMETA(DisplayName = "Epic_Supporter"),
	VE_EpicHero				UMETA(DisplayName = "Epic_Hero"),
	VE_ThreeMember			UMETA(DisplayName = "ThreeMember"),
	VE_FiveMember			UMETA(DisplayName = "FiveMember"),
};
static TMap<int32, FCustomizeDialogue> defaultDialogMap;

UCLASS()
class PROJECTR_API UGamePlayBluePrintFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						BP_LoadData(FName InSpawnID);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						BP_StartMapPhase();
	//	PartyID_NONE -1, PartyID_PC_MY 0, PartyID_NPC_ENEMY 1, PartyID_NPC_HELP 2 //	GroupKey_SINGLE_ENEMY -1, GroupKey_SINGLE_NONE 0, GroupKey_SINGLE_MY 1
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						SpawnPlayer(FName InSpawnID);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						SpawnMonster(FName InSpawnID, FName InUnitID, int32 InLevel = 1, int32 InSightRange = 5000, EMemberMultiplierCategory InMemberMultiplier = EMemberMultiplierCategory::VE_NONE, float InStageHpMultiplier = 1.0f, float InStageAtkMultiplier = 1.0f, bool IsBattleObject = false);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						SpawnNpc(FName InSpawnID, FName InUnitID, int32 InLevel = 1, int32 InSightRange = 5000, EMemberMultiplierCategory InMemberMultiplier = EMemberMultiplierCategory::VE_NONE, float InStageHpMultiplier = 1.0f, float InStageAtkMultiplier = 1.0f, bool IsBattleObject = false);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						SpawnBoss(FName InSpawnID, FName InUnitID, int32 InLevel = 1, int32 InSightRange = 5000, EMemberMultiplierCategory InMemberMultiplier = EMemberMultiplierCategory::VE_NONE, float InStageHpMultiplier = 1.0f, float InStageAtkMultiplier = 1.0f, bool IsBattleObject = false);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						PlaySequence(bool isSkip);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						SetActived(FName InObjectID, bool inActived);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						ShowDialogue(TArray<int32> DialogueIndexes, TMap<int32, FCustomizeDialogue> OverrideMap);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void						Deferred_ShowDialogue();
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							SetDialog(bool InDialog);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static bool							IsShowDialog();
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							SetFirstGameStart(bool InFirstGameStart);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	bool						GetFirstGameStart();
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							NextWayPoint(FName InWayPointID);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							ClearWayPoint();
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							SetPause(bool InPause);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static bool							IsPause();
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							SpeechBubblePlay(FName InUnitID, FName InSpeechKey, float InSpeechShowTime, float InDelay);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							IndicatorPlay(FName KeyName, float InDuration= 0.0f, bool bNoAni= false, bool bBottomMsg = false);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							NpcNextWayPoint(FName InWayPointID);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							NpcClearWayPoint();
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static void							InitCameraTransform(FName InWayPointID);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	void 						SetElevator(bool isElevator, float InDuration);
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	bool						GetElevator();
	UFUNCTION(BlueprintCallable, Category = UGamePlayBluePrintFunction)
	static	float						GetDuration();
public:
	static	void						GameStartEvent();
	static	void						Die(AActor* InDiePlayer);
	static	void						SpawnMultiBoss(TArray<AActor*>& InBoss);
	static	void						SpawnSingleBoss(TArray<AActor*>& InBoss);
	static	void						PlayEventSequence(bool isEnableSkip = false);
	static	void						SpawnPC(AActor* InSpawnPosition);
	static	AActor*						SpawnNPC(AActor* InSpawnPosition, FName InUnitID, ECharacterCategoryTypeEnum InCategoryType, int32 InGroupKey, int32 InPartyID, int32 InLevel, int32 InSightRange, EMemberMultiplierCategory InMemberMultiplier, float InStageHpMultiplier, float InStageAtkMultiplier);
	static	bool						CheckValidSpawnPos(FVector& SpawnPos);
	static	bool						FindValidLocation(const FVector& SpawnPos, FCellIndex& AdjacentCell);
	static	void						StageClearSuccess();
	static	void						StageClearFail(EStageFailReason reason);
	static	UBaseStageInfo*				GetBaseStageInfo();

private:
	static bool							bInDialog;
	static bool							bIsFirstGameStart;
	static bool							bIsElevator;
	static float						Duration;

public:
	static TArray<int32>						SavedDialogueInfo;
	static TMap<int32, FCustomizeDialogue>     SavedInfoMap;
};
