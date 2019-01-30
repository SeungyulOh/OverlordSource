// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "BaseScenePhase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseScenePhase : public UObject
{
	GENERATED_BODY()
	
public:
	//UFUNCTION(BlueprintCallable, Category = "ScenePhase|Input", meta = (WorldContext = "WorldContext", AdvancedDisplay = "WrapperClass"))
	EScenePhaseType		Type;

	
	//void Excute();
	//bool IsSynchronization
	//LogComment
	//CheckTimerHandler
	//CoroutineTime

	//Character Stack
	//Scene Stack
};


//=====================================================================================================
//Base
//=====================================================================================================

USTRUCT(BlueprintType)
struct PROJECTR_API FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BaseSceneInfo)
	EScenePhaseType		Type;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FMakeSceneInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	FName					MapName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	int32					SceneLevel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	bool					IsStreaming;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	TArray<FName>			CharacterIDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	TArray<int32>			CharacterLevel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	TArray<int32>			CharacterGrade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	FName					NextMapName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MakeSceneInfo)
	bool					IsTestLoading;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FStartSceneInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StartSceneInfo)
	TArray<struct FVector>	StartLocationCharacterArray;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StartSceneInfo)
	FName					StageName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StartSceneInfo)
	bool					IsAllEnemySpawn;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StartSceneInfo)
	bool					IsStartUI;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StartSceneInfo)
	bool					IsHideUI;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDelaySceneInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DelaySceneInfo)
	float					DelayTime;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCommandSceneInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	ECommandSceneType		PauseGame;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	ECommandSceneType		HideDecal;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	ECommandSceneType		VisibleSlideArrow;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	ECommandSceneType		VisibleHPBar;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	ECommandSceneType		CustomGame;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	ECommandSceneType		BlackScreen;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	ECommandSceneType		UpdateAI;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	bool					IsFullRecover;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	bool					IsClearObject;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CommandSceneInfo)
	bool					IsStopSound;
};


//=====================================================================================================
//Init
//=====================================================================================================

USTRUCT(BlueprintType)
struct PROJECTR_API FInitCharacterCustomInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	int32					SlotIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	float					DamagePercent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	ECommandSceneType		ClearCustom;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	int32					TargetIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	bool					IsSelectedCharacter;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	bool					IsForceRot;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	bool					IsNullTarget;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	int32					ForceBuff;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	bool					IsBuffSet;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	bool					IsClearAnim;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	FName					TagName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCharacterCustomInfo)
	ECommandSceneType		CustomState;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitSpawnerInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	TArray<int32>			SpawnerIDs;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	bool					IsCustom;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	ECommandSceneType		Custom;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	ECommandSceneType		Visible;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	ECommandSceneType		CustomState;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	float					AddScale;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	int32					AddHP;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	bool					IsClearAnim;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	bool					IsNoneHpbar;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnerInfo)
	bool					IsOnlyInit;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitSpawnSlotInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnSlotInfo)
	int32					SlotID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitSpawnSlotInfo)
	FVector					Location;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitIconInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitIconInfo)
	EHudIcon				IconType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitIconInfo)
	ECommandSceneType		Visible;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitIconInfo)
	bool					IsClick;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitIconInfo)
	ECommandSceneType		BlockClick;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitIconInfo)
	ECommandSceneType		GuideAni;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitIconInfo)
	ECommandSceneType		BlockIcon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitIconInfo)
	ECommandSceneType		TempAttackIcon;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitEffectInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitEffectInfo)
	FName					EffectID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitEffectInfo)
	ECommandSceneType		Visible;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitEffectInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitEffectInfo)
	FVector					Scale;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitEffectInfo)
	bool					IsLoop;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitNotiIndicatorInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitNotiIndicatorInfo)
	FName					DialogueID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitNotiIndicatorInfo)
	float					DelayTime;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitNotiDialogueInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InittNotiDialogurInfo)
	FName					DialogueID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InittNotiDialogurInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InittNotiDialogurInfo)
	int32					SlotIndex;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitForceSpawnInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitForceSpawnInfo)
	FName					ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitForceSpawnInfo)
	int32					Index;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitForceSpawnInfo)
	int32					Level;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitForceSpawnInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitForceSpawnInfo)
	float					Scale;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitForceSpawnInfo)
	float					Rot;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitForceSpawnInfo)
	bool					IsCustomActor;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitWallSpawnInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitWallSpawnInfo)
	int32					Index;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitWallSpawnInfo)
	ECommandSceneType		Visible;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitWallSpawnInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitWallSpawnInfo)
	FVector					Scale;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitWallSpawnInfo)
	FRotator				Rotator;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitCameraArmInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCameraArmInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCameraArmInfo)
	FRotator				Rotator;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCameraArmInfo)
	float					ArmLength;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCameraArmInfo)
	ECommandSceneType		RotationCamera;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitTriggerInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitTriggerInfo)
	int32					GroupIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitTriggerInfo)
	bool					IsTrigger;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitViewImageInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitViewImageInfo)
	FName					TableKeyTexture2D;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitViewImageInfo)
	ECommandSceneType		Visible;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitCameraShakeInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCameraShakeInfo)
	TSubclassOf<class UCameraShake >	CameraShakeEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitCameraShakeInfo)
	float								ShakeIntensity;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInitJudgeInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitJudgeInfo)
	EHudIcon				IconType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitJudgeInfo)
	EJudgeType				MaxJudge;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitJudgeInfo)
	bool					IsActive;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitJudgeInfo)
	float					Time;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InitJudgeInfo)
	int32					SpawnerID;
};

//=====================================================================================================
//Play
//=====================================================================================================

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayMatineeInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	FString					MatineeName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	FString					MatineeCamName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	float					CheckPlayTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	bool					IsSkipButton;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	bool					IsVisibleActor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	bool					IsPauseActor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	bool					IsSelectActorCamera;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	bool					IsLoop;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMatineeInfo)
	bool					IsHideResult;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlaySequenceInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	FName					SequenceID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	ECommandSceneType		VisibleActor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	ECommandSceneType		ActiveHUD;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	bool					IsLoop;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	bool					IsNonFocusSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	bool					IsTimeSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	bool					IsOffAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	bool					IsSkipButton;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySequence)
	bool					IsPauseBGM;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayMovieInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMovieInfo)
	FName					MovieID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMovieInfo)
	bool					MovieType;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayCameraMoveInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayCameraMoveInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayCameraMoveInfo)
	FRotator				Rotator;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayCameraMoveInfo)
	float					MoveTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayCameraMoveInfo)
	float					DelayTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayCameraMoveInfo)
	float					ZoomRate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayCameraMoveInfo)
	float					ZoomValue;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	//bool					IsResult;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayMoveInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	int32					SlotIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	int32					SpawnerID;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	FName					CharacterID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	TArray<FVector>			Locations;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	EAnimStateEnum			Ani;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	float					RotY;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	bool					IsResult;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	bool					IsForceRot;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMoveInfo)
	bool					IsTempClean;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayMovePlayerInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMovePlayerInfo)
	int32					SlotIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayMovePlayerInfo)
	FVector					Locations;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayAnimationInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	int32					SlotIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	int32					SpawnerID;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	ECharacterCommandEnum	AniCommand;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	EBattleDamageResultEnum DamageResultCommand;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	ESkillAniTypeEnum		SkillAniType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	float					ClearAnimationTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	float					AniRateTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	TAssetPtr< UAnimationAsset > Animation;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	bool					IsAniDie2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	bool					IsLoop;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayAnimationInfo)
	bool					IsResult;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayHitAnimationInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayHitAnimationInfo)
	int32					SpawnerID;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayHitAnimationInfo)
	EAnimStateEnum			Ani;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayHitAnimationInfo)
	float					HitTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayHitAnimationInfo)
	float					DelayTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayHitAnimationInfo)
	bool					IsDeath;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayHitAnimationInfo)
	bool					IsDisappear;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayHitAnimationInfo)
	bool					IsResult;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayBattleInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayBattleInfo)
	int32					SpawnerID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayBattleInfo)
	int32					TargetIndex;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayBattleInfo)
	ECommandSceneType		SetBossHPBar;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayBattleInfo)
	bool					IsClearCustom = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayBattleInfo)
	bool					IsUseEnemySkill = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayBattleInfo)
	bool					IsResult = false;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlaySkillInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySkillInfo)
	ECharacterCategoryTypeEnum		CharacterType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySkillInfo)
	int32					SlotIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySkillInfo)
	int32					SpawnerID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySkillInfo)
	int32					SkillID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlaySkillInfo)
	FName					SkillName;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayTargetHPInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTargetHPInfo)
	int32					SpawnerID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTargetHPInfo)
	float					CompareHPPercent;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayJudgeResultInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayJudgeResultInfo)
	EJudgeType				CheckJudge;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayTimeMotionInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTimeMotionInfo)
	float					GlobalTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTimeMotionInfo)
	bool					IsGameTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTimeMotionInfo)
	bool					IsRecovery;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTimeMotionInfo)
	float					RecoveryTickTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTimeMotionInfo)
	float					RecoveryValue;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = PlayTimeMotionInfo)
	float					RecoveryAddValue;
};

//=====================================================================================================
//Input
//=====================================================================================================

USTRUCT(BlueprintType)
struct PROJECTR_API FInputDialogueInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputDialogueInfo)
	FName					DialogueID;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInputWorldLocationInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputWorldLocationInfo)
	int32					SlotIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputWorldLocationInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputWorldLocationInfo)
	FName					TableKeyParticle;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInputIconInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputIconInfo)
	EHudIcon				IconType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputIconInfo)
	FName					NotiDialogueName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputIconInfo)
	bool					IsHideScreen;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputIconInfo)
	bool					IsSlideAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputIconInfo)
	bool					IsCoolTimeZero;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputIconInfo)
	float					ExceptionAttackDelayTime;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInputSkillLocationInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	int32					SkillIconIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	bool					IsGuideAni;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	FVector2D				ScreenLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	float					MaxTime;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	float					TickTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	float					DelayTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSkillLocationInfo)
	FName					RollbackDialogueID;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInputSpawnInfo : public FBaseSceneInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSpawnInfo)
	int32					SlotIndex;

	/*UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSpawnInfo)
	int32					SpawnerID;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSpawnInfo)
	FName					CharacterID;*/

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSpawnInfo)
	FVector					Location;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = InputSpawnInfo)
	bool					IsSpawnEvent;
};
