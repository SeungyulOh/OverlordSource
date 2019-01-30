// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GameFramework/Actor.h"
#include "SharedConstants/GlobalConstants.h"
#include "BaseScenePhase.h"
#include "ScenePhaseMaker.generated.h"

class UBaseScenePhase;
struct FBaseSceneInfo;
/**
 * 
 */
UCLASS()
class PROJECTR_API AScenePhaseMaker : public AActor
{
	GENERATED_BODY()
	
public:
	void Destroyed() override;

public:
//=====================================================================================================
//Control Scene
//=====================================================================================================
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Scene")
	void MakeScene(FName InMapName, int32 InSceneLevel, bool InIsStreaming, 
		TArray<FName> InCharacterIDs, TArray<int32> InCharacterLevel, TArray<int32> InCharacterGrade,
		bool InIsTestLoading);

	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Scene")
	void MakeScene2(const FMakeSceneInfo& Info);

	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Scene")
	void StartScene(TArray<FVector> InStartLocationCharacterArray, FName InStageName, 
		bool InIsAllEnemySpawn, bool InIsStartUI, bool InIsHideUI);

	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Scene")
	void EndScene(bool InIsResult, bool InIsLobby);

	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Scene")
	void DelayScene(float InTime);

	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Scene")
	void CommandScene(ECommandSceneType InPauseGame, ECommandSceneType InHideDecal,
		ECommandSceneType InVisibleSlideArrow, ECommandSceneType InVisibleHPBar,
		ECommandSceneType InCustomGame, ECommandSceneType InBlackScreen, 
		ECommandSceneType InUpdateAI,
		bool InIsFullRecover, bool InIsClearObject, bool InIsStopSound);

//=====================================================================================================
//Init
//=====================================================================================================
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitPlayerCustom(int32 InSlotIndex, float InDamagePercent, ECommandSceneType InClearCustom,
		bool InIsSelectedCharacter, bool InIsForceRot, bool InIsNullTarget,
		bool InIsBuffSet, bool InIsClearAnim, FName InTagName, ECommandSceneType InCustomState,
		int32 InTargetIndex = -1);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitSpawner(TArray<int32> InSpawnerIDs, bool InIsCustom, 
		ECommandSceneType InCustom, ECommandSceneType InVisible, ECommandSceneType InCustomState, 
		float InAddScale, int32 InAddHP, bool InIsClearAnim, bool InIsNoneHpbar, bool InIsOnlyInit);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitSpawnSlot(int32 InSpawnID, FVector InLocation);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitIcon(EHudIcon InIconType, ECommandSceneType InVisible, 
		bool InIsClick, ECommandSceneType InBlockIcon,
		ECommandSceneType InBlockHeroIcon, ECommandSceneType InGuideAni, ECommandSceneType InTempAttackIcon);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitEffect(FName InEffectID, ECommandSceneType InVisible, 
		FVector InLocation, FVector InScale, bool InIsLoop);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitNotiIndicator(FName InDialogueName, float InDelayTime);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitNotiDialogue(FName InDialogueName, FVector InLocation, int32 InSlotIndex);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitForceSpawn(FName InID, int32 InIndex, int32 InLevel, FVector InLocation,
		float InScale, float InRot, bool IsCustom);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitWallSpawn(int32 InIndex, ECommandSceneType InVisible, FVector InLocation, 
		FVector InScale, FRotator InRotator);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitCameraArm(FVector InLocation, FRotator InRotator, float InArmLength, ECommandSceneType InRotationCamera);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitTrigger(int32 InGroupIndex, bool InIsTrigger);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitViewImage(FName InTableKeyTexture2D, ECommandSceneType InVisible);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitCameraShake(TSubclassOf<class UCameraShake > InCameraShakeEffect, float InShakeIntensity);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Init")
	void AddInitJudge(EHudIcon InIconType, EJudgeType InMaxJudge, bool InIsActive, float InTime, int32 InSpawnerID);

//=====================================================================================================
//Play
//=====================================================================================================
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayMatinee(FString InMatineeName, FString InMatineeCamName, float InCheckPlayTime,
		bool InIsSkipButton, bool InIsVisibleActor, bool InIsPauseActor = true, 
		bool InIsSelectActorCamera = false, bool InIsLoop = false, bool InIsHideResult = false);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlaySequence(FName InSequenceName, FVector InLocation, ECommandSceneType InVisibleActor,
		ECommandSceneType InActiveHUD, bool InIsLoop, bool InIsNonFocusSequence,
		bool InIsTimeSequence, bool InIsOffAction, bool InIsSkipButton, bool InIsPauseBGM);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayMovie(FName InMovieName);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayCameraMove(FVector InLocation, FRotator InRotator, float InMoveTime, float InGoalDelayTime);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayMove(int32 InSlotIndex, int32 InSpawnerID, FName InCharacterID, TArray<FVector> InLocations,
		EAnimStateEnum InAni, bool InIsResult, bool InIsForceRot, bool InIsTempClean, float InRotY = -1);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayAnimation(TAssetPtr< UAnimationAsset > InAnimation, int32 InSlotIndex, int32 InSpawnerID,
		ECharacterCommandEnum InAniCommand, EBattleDamageResultEnum InDamageResultCommand,
		ESkillAniTypeEnum InSkillAniType, float InClearAnimationTime = 0.5f, float InAniRateTime = 1.0f,
		bool InIsAniDie2 = false, bool InIsLoop = true, bool InIsResult = false);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayHitAnimation(int32 InSpawnerID, EAnimStateEnum InAni, float InHitTime, float InDelayTime,
		bool InIsDeath, bool InIsDisappear, bool InIsResult);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayBattle(int32 InSpawnerID, int32 InTargetIndex, ECommandSceneType InSetBossHPBar, 
		bool InIsClearCustom, bool InIsUseEnemySkill, bool InIsResult);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlaySkill(ECharacterCategoryTypeEnum InCharacterType, int32 InSlotIndex, int32 InSpawnerID, 
		int32 InSkillID, FName InSkillName);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayTargetHP(int32 InSpawnerID, float InCompareHPPercent);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayJudgeResult(EJudgeType InCheckJudge);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Play")
	void AddPlayTimeMotion(float InGlobalTime, bool InIsGameTime, bool InIsRecovery,
		float InRecoveryTickTime, float InRecoveryValue, float InRecoveryAddValue);

//=====================================================================================================
//Input
//=====================================================================================================
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Input", meta = (HidePin = "Target", DefaultToSelf = "self"))
	void AddInputDialogue(FName InDialogueName);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Input")
	void AddInputWorldLocation(int32 InSlotIndex, FVector InLocation, FName	InTableKeyParticle);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Input")
	void AddInputIcon(EHudIcon InIconType, FName InNotiDialogueName, 
		bool InIsHideScreen, bool InIsSlideAction, bool InIsCoolTimeZero, float InExceptionAttackDelayTime);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Input")
	void AddInputSkillLocation(int32 InSkillIconIndex, FVector InLocation,
		bool InIsGuideAni, FVector2D InScreenLocation, float InMaxTime, float InTickTime, float InDelayTime,
		FName InRollbackDialogueID);
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Input")
	void AddInputSpawnLocation(int32 InSlotIndex, FVector InLocation);

//=====================================================================================================
//etc
//=====================================================================================================
	//temp
	UFUNCTION(BlueprintCallable, Category = "ScenePhase|Scene")
	void ForceCache();

	//void ForceSetting();

	void				UpdateSceneIndex();
	FORCEINLINE void	RollbackSceneIndex()	{ --CurrentSceneIndex; }
	FORCEINLINE void	ForceSkipTutorial()		{ CurrentSceneIndex = SceneSize - 1; }
	FBaseSceneInfo*		GetCurrentScene();
	FORCEINLINE int32	GetCurrentSceneIndex()	{ return CurrentSceneIndex; }
	FORCEINLINE bool	GetIsEndScene()			{ return (SceneSize == CurrentSceneIndex + 1); }

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ScenePhaseMaker)
	bool					IsDebug = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = ScenePhaseMaker)
	int32					SceneSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NewTutorialManager)
	bool					IsDebugHud = false;

private:
	void Empty();

private:
	TArray<TSharedPtr<FBaseSceneInfo>>	ScenePhaseArray;
	//TArray<FBaseSceneInfo*> ScenePhaseArray;
	//TArray<TSharedPtr<FBaseSceneInfo>>	SceneRollbackArray;

	int32 CurrentSceneIndex = -1;
};
