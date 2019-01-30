// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

//#include "BaseScenePhase.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "CustomStructures/GameEnvironmentVar.h"

#include "ScenePhaseMaker.h"



void AScenePhaseMaker::Destroyed()
{
	Empty();

	Super::Destroyed();
}


void AScenePhaseMaker::UpdateSceneIndex()
{
	++CurrentSceneIndex;
	if (SceneSize <= CurrentSceneIndex)
	{
		CurrentSceneIndex = SceneSize - 1;
	}
}

FBaseSceneInfo* AScenePhaseMaker::GetCurrentScene()
{
	if (ScenePhaseArray.IsValidIndex(CurrentSceneIndex))
	{
		return ScenePhaseArray[CurrentSceneIndex].Get();
	}

	return nullptr;
}

//=====================================================================================================
//Control Scene
//=====================================================================================================

void AScenePhaseMaker::MakeScene(FName InMapName, int32 InSceneLevel, bool InIsStreaming, 
	TArray<FName> InCharacterIDs, TArray<int32> InCharacterLevel, TArray<int32> InCharacterGrade, bool InIsTestLoading)
{
	TSharedPtr<FMakeSceneInfo> Info = MakeShareable(new FMakeSceneInfo());
	Info->Type = EScenePhaseType::VE_MakeScene;
	Info->MapName = InMapName;
	Info->SceneLevel = InSceneLevel;
	Info->IsStreaming = InIsStreaming;
	Info->CharacterIDs = InCharacterIDs;
	Info->CharacterLevel = InCharacterLevel;
	Info->CharacterGrade = InCharacterGrade;
	Info->IsTestLoading = InIsTestLoading;
	ScenePhaseArray.Emplace(Info);
	
	if (InIsStreaming)
	{
		/*URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		bool IsMovieType = (RGameInstance && RGameInstance->GameEnvironmentVar.InitNewTutorialStateType);

		if (IsMovieType == false)
		{
			AddPlayMovie(FName(""));
		}*/

		/*TSharedPtr<FBaseSceneInfo> NextInfo = MakeShareable(new FBaseSceneInfo());
		NextInfo->Type = EScenePhaseType::VE_WaitCharacter;
		ScenePhaseArray.Emplace(NextInfo);*/
	}
	
}

void AScenePhaseMaker::MakeScene2(const FMakeSceneInfo& Info)
{
	TSharedPtr<FMakeSceneInfo> Info2 = MakeShareable(new FMakeSceneInfo());
	TSharedRef<FMakeSceneInfo> Info3(new FMakeSceneInfo());
	Info2->Type = EScenePhaseType::VE_MakeScene;
	ScenePhaseArray.Emplace(Info2);
}

void AScenePhaseMaker::StartScene(TArray<FVector> InStartLocationCharacterArray, FName InStageName, 
	bool InIsAllEnemySpawn, bool InIsStartUI, bool InIsHideUI)
{
	TSharedPtr<FStartSceneInfo> Info = MakeShareable(new FStartSceneInfo());
	Info->Type = EScenePhaseType::VE_StartScene;
	Info->StartLocationCharacterArray = InStartLocationCharacterArray;
	Info->StageName = InStageName;
	Info->IsAllEnemySpawn = InIsAllEnemySpawn;
	Info->IsStartUI = InIsStartUI;
	Info->IsHideUI = InIsHideUI;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::EndScene(bool InIsResult, bool InIsLobby)
{
	TSharedPtr<FBaseSceneInfo> Info = MakeShareable(new FBaseSceneInfo());
	Info->Type = EScenePhaseType::VE_EndScene;
	//Info->IsLobby = InIsLobby;
	ScenePhaseArray.Emplace(Info);

	//if(InIsResult)
	SceneSize = ScenePhaseArray.Num();
}

void AScenePhaseMaker::DelayScene(float InTime)
{
	TSharedPtr<FDelaySceneInfo> Info = MakeShareable(new FDelaySceneInfo());
	Info->Type = EScenePhaseType::VE_DelayScene;
	
	Info->DelayTime = InTime;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::CommandScene(ECommandSceneType InPauseGame, ECommandSceneType InHideDecal,
	ECommandSceneType InVisibleSlideArrow, ECommandSceneType InVisibleHPBar, 
	ECommandSceneType InCustomGame, ECommandSceneType InBlackScreen, 
	ECommandSceneType InUpdateAI,
	bool InIsFullRecover, bool InIsClearObject, bool InIsStopSound)
{
	TSharedPtr<FCommandSceneInfo> Info = MakeShareable(new FCommandSceneInfo());
	Info->Type = EScenePhaseType::VE_CommandScene;
	
	Info->PauseGame = InPauseGame;
	Info->HideDecal = InHideDecal;
	Info->VisibleSlideArrow = InVisibleSlideArrow;
	Info->VisibleHPBar = InVisibleHPBar;
	Info->CustomGame = InCustomGame;
	Info->BlackScreen = InBlackScreen;
	Info->UpdateAI = InUpdateAI;
	Info->IsFullRecover = InIsFullRecover;
	Info->IsClearObject = InIsClearObject;
	Info->IsStopSound = InIsStopSound;
	ScenePhaseArray.Emplace(Info);
}

//=====================================================================================================
//Init
//=====================================================================================================

void AScenePhaseMaker::AddInitPlayerCustom(int32 InSlotIndex, float InDamagePercent, 
	ECommandSceneType InClearCustom,
	bool InIsSelectedCharacter, bool InIsForceRot, bool InIsNullTarget,
	bool InIsBuffSet, bool InIsClearAnim, FName InTagName, ECommandSceneType InCustomState,
	int32 InTargetIndex /*= -1*/)
{
	TSharedPtr<FInitCharacterCustomInfo> Info = MakeShareable(new FInitCharacterCustomInfo());
	Info->Type = EScenePhaseType::VE_InitCharacterCustom;
	
	Info->SlotIndex = InSlotIndex;
	Info->DamagePercent = InDamagePercent;
	Info->ClearCustom = InClearCustom;
	Info->IsSelectedCharacter = InIsSelectedCharacter;
	Info->IsForceRot = InIsForceRot;
	Info->IsNullTarget = InIsNullTarget;
	//Info->ForceBuff = InForceBuff;
	Info->IsBuffSet = InIsBuffSet;
	Info->IsClearAnim = InIsClearAnim;
	Info->TagName = InTagName;
	Info->CustomState = InCustomState;
	Info->TargetIndex = InTargetIndex;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitSpawner(TArray<int32> InSpawnerIDs, bool InIsCustom, 
	ECommandSceneType InCustom, ECommandSceneType InVisible, ECommandSceneType InCustomState, 
	float InAddScale, int32 InAddHP, bool InIsClearAnim, bool InIsNoneHpbar, bool InIsOnlyInit)
{
	TSharedPtr<FInitSpawnerInfo> Info = MakeShareable(new FInitSpawnerInfo());
	Info->Type = EScenePhaseType::VE_InitSpawner;
	
	Info->SpawnerIDs = InSpawnerIDs;
	Info->IsCustom = InIsCustom;
	Info->Custom = InCustom;
	Info->Visible = InVisible;
	Info->CustomState = InCustomState;
	Info->AddScale = InAddScale;
	Info->AddHP = InAddHP;
	Info->IsClearAnim = InIsClearAnim;
	Info->IsNoneHpbar = InIsNoneHpbar;
	Info->IsOnlyInit = InIsOnlyInit;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitSpawnSlot(int32 InSpawnID, FVector InLocation)
{
	TSharedPtr<FInitSpawnSlotInfo> Info = MakeShareable(new FInitSpawnSlotInfo());
	Info->Type = EScenePhaseType::VE_InitSpawnSlot;
	
	Info->SlotID = InSpawnID;
	Info->Location = InLocation;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitIcon(EHudIcon InIconType, ECommandSceneType InVisible, 
	bool InIsClick, ECommandSceneType InBlockIcon,
	ECommandSceneType InBlockHeroIcon, ECommandSceneType InGuideAni, ECommandSceneType InTempAttackIcon)
{
	TSharedPtr<FInitIconInfo> Info = MakeShareable(new FInitIconInfo());
	Info->Type = EScenePhaseType::VE_InitIcon;
	
	Info->IconType = InIconType;
	Info->Visible = InVisible;
	Info->IsClick = InIsClick;
	Info->BlockClick = InBlockHeroIcon;
	Info->GuideAni = InGuideAni;
	Info->BlockIcon = InBlockIcon;
	Info->TempAttackIcon = InTempAttackIcon;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitEffect(FName InEffectID, ECommandSceneType InVisible, 
	FVector InLocation, FVector InScale, bool InIsLoop)
{
	TSharedPtr<FInitEffectInfo> Info = MakeShareable(new FInitEffectInfo());
	Info->Type = EScenePhaseType::VE_InitEffect;
	
	Info->EffectID = InEffectID;
	Info->Visible = InVisible;
	Info->Location = InLocation;
	Info->Scale = InScale;
	Info->IsLoop = InIsLoop;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitNotiIndicator(FName InDialogueName, float InDelayTime)
{
	TSharedPtr<FInitNotiIndicatorInfo> Info = MakeShareable(new FInitNotiIndicatorInfo());
	Info->Type = EScenePhaseType::VE_InitNotiIndicator;
	
	Info->DialogueID = InDialogueName;
	Info->DelayTime = InDelayTime;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitNotiDialogue(FName InDialogueName, FVector InLocation, int32 InSlotIndex)
{
	TSharedPtr<FInitNotiDialogueInfo> Info = MakeShareable(new FInitNotiDialogueInfo());
	Info->Type = EScenePhaseType::VE_InitNotiDialogue;
	
	Info->DialogueID = InDialogueName;
	Info->Location = InLocation;
	Info->SlotIndex = InSlotIndex;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitForceSpawn(FName InID, int32 InIndex, int32 InLevel, FVector InLocation,
	float InScale, float InRot, bool IsCustom)
{
	TSharedPtr<FInitForceSpawnInfo> Info = MakeShareable(new FInitForceSpawnInfo());
	Info->Type = EScenePhaseType::VE_InitForceSpawn;
	
	Info->ID = InID;
	Info->Index = InIndex;
	Info->Level = InLevel;
	Info->Location = InLocation;
	Info->Scale = InScale;
	Info->Rot = InRot;
	Info->IsCustomActor = IsCustom;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitWallSpawn(int32 InIndex, ECommandSceneType InVisible, FVector InLocation,
	FVector InScale, FRotator InRotator)
{
	TSharedPtr<FInitWallSpawnInfo> Info = MakeShareable(new FInitWallSpawnInfo());
	Info->Type = EScenePhaseType::VE_InitWallSpawn;

	Info->Index = InIndex;
	Info->Visible = InVisible;
	Info->Location = InLocation;
	Info->Scale = InScale;
	Info->Rotator = InRotator;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitCameraArm(FVector InLocation, FRotator InRotator, float InArmLength, ECommandSceneType InRotationCamera)
{
	TSharedPtr<FInitCameraArmInfo> Info = MakeShareable(new FInitCameraArmInfo());
	Info->Type = EScenePhaseType::VE_InitCameraArm;
	
	Info->Location = InLocation;
	Info->Rotator = InRotator;
	Info->ArmLength = InArmLength;
	Info->RotationCamera = InRotationCamera;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitTrigger(int32 InGroupIndex, bool InIsTrigger)
{
	TSharedPtr<FInitTriggerInfo> Info = MakeShareable(new FInitTriggerInfo());
	Info->Type = EScenePhaseType::VE_InitTrigger;
	
	Info->GroupIndex = InGroupIndex;
	Info->IsTrigger = InIsTrigger;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitViewImage(FName InTableKeyTexture2D, ECommandSceneType InVisible)
{
	TSharedPtr<FInitViewImageInfo> Info = MakeShareable(new FInitViewImageInfo());
	Info->Type = EScenePhaseType::VE_InitViewImage;
	
	Info->TableKeyTexture2D = InTableKeyTexture2D;
	Info->Visible = InVisible;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitCameraShake(
	TSubclassOf<class UCameraShake > InCameraShakeEffect, float InShakeIntensity)
{
	TSharedPtr<FInitCameraShakeInfo> Info = MakeShareable(new FInitCameraShakeInfo());
	Info->Type = EScenePhaseType::VE_InitCameraShake;
	
	Info->CameraShakeEffect = InCameraShakeEffect;
	Info->ShakeIntensity = InShakeIntensity;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInitJudge(EHudIcon InIconType, EJudgeType InMaxJudge, bool InIsActive, float InTime, int32 InSpawnerID)
{
	TSharedPtr<FInitJudgeInfo> Info = MakeShareable(new FInitJudgeInfo());
	Info->Type = EScenePhaseType::VE_InitJudge;
	Info->IconType = InIconType;
	Info->MaxJudge = InMaxJudge;
	Info->IsActive = InIsActive;
	Info->Time = InTime;
	Info->SpawnerID = InSpawnerID;
	//Info->
	ScenePhaseArray.Emplace(Info);
}

//=====================================================================================================
//Play
//=====================================================================================================
void AScenePhaseMaker::AddPlayMatinee(FString InMatineeName, FString InMatineeCamName, float InCheckPlayTime,
	bool InIsSkipButton, bool InIsVisibleActor, bool InIsPauseActor /*= true*/, 
	bool InIsSelectActorCamera /*= false*/, bool InIsLoop /*= false*/, bool InIsHideResult /*= false*/)
{
	TSharedPtr<FPlayMatineeInfo> Info = MakeShareable(new FPlayMatineeInfo());
	Info->Type = EScenePhaseType::VE_PlayMatinee;
	
	Info->MatineeName = InMatineeName;
	Info->MatineeCamName = InMatineeCamName;
	Info->CheckPlayTime = InCheckPlayTime;
	Info->IsSkipButton = InIsSkipButton;
	Info->IsVisibleActor = InIsVisibleActor;
	Info->IsPauseActor = InIsPauseActor;
	Info->IsSelectActorCamera = InIsSelectActorCamera;
	Info->IsLoop = InIsLoop;
	Info->IsHideResult = InIsHideResult;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlaySequence(FName InSequenceName, FVector InLocation, ECommandSceneType InVisibleActor,
	ECommandSceneType InActiveHUD, bool InIsLoop, bool InIsNonFocusSequence,
	bool InIsTimeSequence, bool InIsOffAction, bool InIsSkipButton, bool InIsPauseBGM)
{
	TSharedPtr<FPlaySequenceInfo> Info = MakeShareable(new FPlaySequenceInfo());
	Info->Type = EScenePhaseType::VE_PlaySequence;
	
	Info->SequenceID = InSequenceName;
	Info->Location = InLocation;
	Info->VisibleActor = InVisibleActor;
	Info->ActiveHUD = InActiveHUD;
	Info->IsLoop = InIsLoop;
	Info->IsNonFocusSequence = InIsNonFocusSequence;
	Info->IsTimeSequence = InIsTimeSequence;
	Info->IsOffAction = InIsOffAction;
	Info->IsSkipButton = InIsSkipButton;
	Info->IsPauseBGM = InIsPauseBGM;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayMovie(FName InMovieName)
{
	TSharedPtr<FPlayMovieInfo> Info = MakeShareable(new FPlayMovieInfo());
	//FPlayMovieInfo* Info = new FPlayMovieInfo;
	Info->Type = EScenePhaseType::VE_PlayMovie;
	
	Info->MovieID = InMovieName;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayCameraMove(FVector InLocation, FRotator InRotator, float InMoveTime, float InGoalDelayTime)
{
	TSharedPtr<FPlayCameraMoveInfo> Info = MakeShareable(new FPlayCameraMoveInfo());
	Info->Type = EScenePhaseType::VE_PlayCameraMove;
	
	Info->Location = InLocation;
	Info->Rotator = InRotator;
	Info->MoveTime = InMoveTime;
	Info->DelayTime = InGoalDelayTime;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayMove(int32 InSlotIndex, int32 InSpawnerID, FName InCharacterID, TArray<FVector> InLocations, 
	EAnimStateEnum InAni, bool InIsResult, bool InIsForceRot, bool InIsTempClean, float InRotY /*= -1*/)
{
	TSharedPtr<FPlayMoveInfo> Info = MakeShareable(new FPlayMoveInfo());
	Info->Type = EScenePhaseType::VE_PlayMove;
	
	Info->SlotIndex = InSlotIndex;
	Info->SpawnerID = InSpawnerID;
	Info->CharacterID = InCharacterID;
	Info->Locations = InLocations;
	Info->Ani = InAni;
	Info->IsResult = InIsResult;
	Info->IsForceRot = InIsForceRot;
	Info->IsTempClean = InIsTempClean;
	Info->RotY = InRotY;

	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayAnimation(TAssetPtr< UAnimationAsset > InAnimation, int32 InSlotIndex, int32 InSpawnerID,
	ECharacterCommandEnum InAniCommand, EBattleDamageResultEnum InDamageResultCommand,
	ESkillAniTypeEnum InSkillAniType, float InClearAnimationTime /*= 0.5f*/, float InAniRateTime /*= 1.0f*/,
	bool InIsAniDie2 /*= false*/, bool InIsLoop /*= true*/, bool InIsResult /*= false*/)
{
	TSharedPtr<FPlayAnimationInfo> Info = MakeShareable(new FPlayAnimationInfo());
	Info->Type			= EScenePhaseType::VE_PlayAnimation;
	//Info->Animation	= InAnimation;
	Info->SlotIndex = InSlotIndex;
	Info->SpawnerID		= InSpawnerID;
	Info->AniCommand	= InAniCommand;
	Info->DamageResultCommand = InDamageResultCommand;
	Info->SkillAniType = InSkillAniType;
	Info->ClearAnimationTime = InClearAnimationTime;
	Info->AniRateTime	= InAniRateTime;
	Info->IsAniDie2		= InIsAniDie2;
	Info->IsLoop		= InIsLoop;
	Info->IsResult		= InIsResult;
	//CharacterID, Animation
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayHitAnimation(int32 InSpawnerID, EAnimStateEnum InAni, float InHitTime,
	float InDelayTime, bool InIsDeath, bool InIsDisappear, bool InIsResult)
{
	TSharedPtr<FPlayHitAnimationInfo> Info = MakeShareable(new FPlayHitAnimationInfo());
	Info->Type = EScenePhaseType::VE_PlayHitAnimation;
	
	Info->SpawnerID = InSpawnerID;
	Info->Ani = InAni;
	Info->HitTime = InHitTime;
	Info->DelayTime = InDelayTime;
	Info->IsDeath = InIsDeath;
	Info->IsDisappear = InIsDisappear;
	Info->IsResult = InIsResult;
	
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayBattle(int32 InSpawnerID, int32 InTargetIndex, ECommandSceneType InSetBossHPBar,
	bool InIsClearCustom, bool InIsUseEnemySkill, bool InIsResult)
{
	TSharedPtr<FPlayBattleInfo> Info = MakeShareable(new FPlayBattleInfo());
	Info->Type		= EScenePhaseType::VE_PlayBattle;
	
	Info->SpawnerID = InSpawnerID;
	Info->TargetIndex = InTargetIndex;
	Info->SetBossHPBar = InSetBossHPBar;
	Info->IsClearCustom = InIsClearCustom;
	Info->IsUseEnemySkill = InIsUseEnemySkill;
	Info->IsResult = InIsResult;
	
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlaySkill(ECharacterCategoryTypeEnum InCharacterType, int32 InSlotIndex, int32 InSpawnerID,
	int32 InSkillID, FName InSkillName)
{
	TSharedPtr<FPlaySkillInfo> Info = MakeShareable(new FPlaySkillInfo());
	Info->Type = EScenePhaseType::VE_PlaySkill;
	
	Info->CharacterType = InCharacterType;
	Info->SlotIndex = InSlotIndex;
	Info->SpawnerID = InSpawnerID;
	Info->SkillID = InSkillID;
	Info->SkillName = InSkillName;

	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayTargetHP(int32 InSpawnerID, float InCompareHPPercent)
{
	TSharedPtr<FPlayTargetHPInfo> Info = MakeShareable(new FPlayTargetHPInfo());
	Info->Type = EScenePhaseType::VE_PlayTargetHP;
	
	Info->SpawnerID = InSpawnerID;
	Info->CompareHPPercent = InCompareHPPercent;

	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayJudgeResult(EJudgeType InCheckJudge)
{
	TSharedPtr<FPlayJudgeResultInfo> Info = MakeShareable(new FPlayJudgeResultInfo());
	Info->Type = EScenePhaseType::VE_PlayJudgeResult;
	Info->CheckJudge = InCheckJudge;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddPlayTimeMotion(float InGlobalTime, bool InIsGameTime, bool InIsRecovery,
	float InRecoveryTickTime, float InRecoveryValue, float InRecoveryAddValue)
{
	TSharedPtr<FPlayTimeMotionInfo> Info = MakeShareable(new FPlayTimeMotionInfo());
	Info->Type = EScenePhaseType::VE_PlayTimeMotion;
	Info->GlobalTime = InGlobalTime;
	Info->IsGameTime = InIsGameTime;
	Info->IsRecovery = InIsRecovery;
	Info->RecoveryTickTime = InRecoveryTickTime;
	Info->RecoveryValue = InRecoveryValue;
	Info->RecoveryAddValue = InRecoveryAddValue;

	ScenePhaseArray.Emplace(Info);
}

//=====================================================================================================
//Input
//=====================================================================================================
void AScenePhaseMaker::AddInputDialogue(FName InDialogueName)
{
	TSharedPtr<FInputDialogueInfo> Info = MakeShareable(new FInputDialogueInfo());
	Info->Type = EScenePhaseType::VE_InputDialogue;
	Info->DialogueID = InDialogueName;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInputIcon(EHudIcon InIconType, FName InNotiDialogueName, 
	bool InIsHideScreen, bool InIsSlideAction, bool InIsCoolTimeZero, float InExceptionAttackDelayTime)
{
	TSharedPtr<FInputIconInfo> Info = MakeShareable(new FInputIconInfo());
	Info->Type			= EScenePhaseType::VE_InputIcon;
	Info->IconType		= InIconType;
	Info->NotiDialogueName = InNotiDialogueName;
	Info->IsHideScreen	= InIsHideScreen;
	Info->IsSlideAction = InIsSlideAction;
	Info->IsCoolTimeZero = InIsCoolTimeZero;
	Info->ExceptionAttackDelayTime = InExceptionAttackDelayTime;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInputWorldLocation(int32 InSlotIndex, FVector InLocation, FName InTableKeyParticle)
{
	TSharedPtr<FInputWorldLocationInfo> Info = MakeShareable(new FInputWorldLocationInfo());
	Info->Type = EScenePhaseType::VE_InputLocation;
	Info->SlotIndex = InSlotIndex;
	Info->Location = InLocation;
	Info->TableKeyParticle = InTableKeyParticle;
	//CharacterID, Location
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInputSkillLocation(int32 InSkillIconIndex, FVector InLocation, 
	bool InIsGuideAni, FVector2D InScreenLocation, float InMaxTime, float InTickTime, float InDelayTime,
	FName InRollbackDialogueID)
{
	TSharedPtr<FInputSkillLocationInfo> Info = MakeShareable(new FInputSkillLocationInfo());
	Info->Type = EScenePhaseType::VE_InputSkillLocation;
	Info->SkillIconIndex = InSkillIconIndex;
	Info->Location = InLocation;
	Info->IsGuideAni = InIsGuideAni;
	Info->ScreenLocation = InScreenLocation;
	Info->MaxTime = InMaxTime;
	Info->TickTime = InTickTime;
	Info->DelayTime = InDelayTime;
	Info->RollbackDialogueID = InRollbackDialogueID;
	//CharacterID, Location
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::AddInputSpawnLocation(int32 InSlotIndex, FVector InLocation)
{
	TSharedPtr<FInputSpawnInfo> Info = MakeShareable(new FInputSpawnInfo());
	Info->Type = EScenePhaseType::VE_InputSpawn;
	Info->SlotIndex = InSlotIndex;
	Info->Location = InLocation;
	//CharacterID, Location
	ScenePhaseArray.Emplace(Info);
}

//=====================================================================================================
//etc
//=====================================================================================================
void AScenePhaseMaker::ForceCache()
{
	TSharedPtr<FBaseSceneInfo> Info = MakeShareable(new FBaseSceneInfo());
	Info->Type = EScenePhaseType::VE_ForceCache;
	ScenePhaseArray.Emplace(Info);
}

void AScenePhaseMaker::Empty()
{
	/*for (auto Element : ScenePhaseArray)
	{
		//delete Element.Get();
		int32 a = 1;
	}*/
	ScenePhaseArray.Empty();
}