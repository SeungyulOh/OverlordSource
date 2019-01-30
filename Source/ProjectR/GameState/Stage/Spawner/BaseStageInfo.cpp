
#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "BaseStageInfo.h"





#include "UtilFunctionIntegrated.h"
#include "CustomActors/BaseSequenceActor.h"



#include "GlobalIntegrated.h"
#include "ContentGameMode/Modes/TestGameMode.h"

#include "ContentGameMode/Controllers/StagePlayerController.h"

#include "Table/CharacterTableInfo.h"
#include "Table/MapTableInfo.h"

#include "DirectionTrackManager.h"

UBaseStageInfo::UBaseStageInfo(const FObjectInitializer& ObjectInitializer)
{
	
}

UBaseStageInfo::~UBaseStageInfo()
{

}

void UBaseStageInfo::Init()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	SetPlayTime();
	ArrMonsterInfo.Reset();
	ArrHeroInfo.Reset();
	ArrPlayerInfo.Reset();
	ArrBossInfo.Reset();
	GameInstance->AutoPlayManager->Initialize();
}

void UBaseStageInfo::Reset()
{
	URGameInstance*		GameInstance		=	RGAMEINSTANCE(this);
	UWorld*				OwnerWorld			=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	bFirstStageTry = false;
	MapTableKey = FName(TEXT(""));
	FloorInfoId = FName(TEXT(""));
	StageID = FName(TEXT(""));
	StageIndex = 0;
	ClearRewardTableKey = FName(TEXT(""));
	PlayLevel = 1;
	PlayUD = TEXT("");
	ArrBossInfo.Empty();
	ArrMonsterInfo.Empty();
	ArrHeroInfo.Empty();
	ArrPlayerInfo.Empty();
	GamePlayTime = 0.0f;
	ClearCondition = EPhaseClearEnumType::VE_MAX;
	bPlayAlone = false;

	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	if (GetGameTimeHandler().IsValid())
		OwnerWorld->GetTimerManager().ClearTimer(TimeHandler_GameTime);

	ArrMonsterInfo.Reset();
	ArrHeroInfo.Reset();
	ArrPlayerInfo.Reset();
	ArrBossInfo.Reset();

	UGamePlayBluePrintFunction::SetDialog(false);
	UGamePlayBluePrintFunction::SetFirstGameStart(false);

}


void UBaseStageInfo::SetNormalNpc(AActor* InActor, int32 InGroupKey)
{
	if (InGroupKey == GroupKey_SINGLE_ENEMY)
		SetMonster(InActor);
	else if (InGroupKey == GroupKey_SINGLE_MY)
		SetHero(InActor);
}

void UBaseStageInfo::SetMonster(AActor* InMonster) 
{
	ArrMonsterInfo.Emplace(InMonster);
}

void UBaseStageInfo::SetHero(AActor* InHero)
{
	ArrHeroInfo.Emplace(InHero);
	if (LevelActor)
		LevelActor->SetNpcHero(InHero);
}

void UBaseStageInfo::SetBoss(AActor* InBoss)
{
	ArrBossInfo.Emplace(InBoss);
}

void UBaseStageInfo::SetPlayer(AActor* InPlayer)
{
	ArrPlayerInfo.Emplace(InPlayer);
}

void UBaseStageInfo::DeleteMonster(AActor* InMonster)
{
	if (LevelActor == nullptr)
		return;

	ALevelActorPhase* LevelActorPhase = LevelActor->GetLevelActorPhase();

	int32 Index				=	ArrMonsterInfo.Find(InMonster);
	int32 SpecificIndex		=	LevelActor->SpecificMonster.Find(InMonster);

	if (Index != INDEX_NONE)
	{
		ArrMonsterInfo.RemoveAt(Index);
		if (LevelActorPhase)
			LevelActorPhase->MonsterDieCount++;
	}

	if (SpecificIndex != INDEX_NONE)
		LevelActor->SpecificMonster.RemoveAt(SpecificIndex);

	LevelActor->ConditionCheck(EPhaseClearEnumType::VE_AllKill);
	LevelActor->ConditionCheck(EPhaseClearEnumType::VE_Resisting);
	LevelActor->ConditionCheck(EPhaseClearEnumType::VE_MonsterCountCheck);
	LevelActor->ConditionCheck(EPhaseClearEnumType::VE_SpecificMonsterCountCheck);

	if (LevelActorPhase)
	{
		EPhaseClearEnumType Condition = LevelActorPhase->assistantCondition;
		if(Condition != EPhaseClearEnumType::VE_BossKill)
			LevelActor->ConditionCheck(EPhaseClearEnumType::VE_CharacterAllSurvival);
	}

}

void UBaseStageInfo::DeleteHero(AActor* InHero)
{
	int32 Index = ArrHeroInfo.Find(InHero);

	if (Index != INDEX_NONE)
		ArrHeroInfo.RemoveAt(Index);
}

void UBaseStageInfo::DeleteBoss(AActor* InBoss)
{
	if (LevelActor == nullptr)
		return;

	ALevelActorPhase* LevelActorPhase = LevelActor->GetLevelActorPhase();

	int32 Index = ArrBossInfo.Find(InBoss);
	int32 SpecificIndex = LevelActor->SpecificMonster.Find(InBoss);

	if (Index != INDEX_NONE)
		ArrBossInfo.RemoveAt(Index);
	if (SpecificIndex != INDEX_NONE)
		LevelActor->SpecificMonster.RemoveAt(SpecificIndex);

	LevelActor->ConditionCheck(EPhaseClearEnumType::VE_SpecificMonsterCountCheck);
	if (LevelActorPhase)
	{
		EPhaseClearEnumType Condition = LevelActorPhase->assistantCondition;
		if (Condition != EPhaseClearEnumType::VE_BossKill)
			LevelActor->ConditionCheck(EPhaseClearEnumType::VE_CharacterAllSurvival);
	}
}

void UBaseStageInfo::DeletePlayer(AActor* InPlayer)
{
	if (LevelActor == nullptr)
		return;

	ALevelActorPhase* LevelActorPhase = LevelActor->GetLevelActorPhase();
	int32 Index = ArrPlayerInfo.Find(InPlayer);

	if (Index != INDEX_NONE)
	{
		//ArrPlayerInfo[Index]->Destroy();
		//Character should be deleted after playing dead animation. 
		//Dont worry about deleting character actor here. 
		//this actor will be deleted after done playing dead animation.
		ArrPlayerInfo.RemoveAt(Index);
		if (LevelActorPhase)
		{
			EPhaseClearEnumType Condition = LevelActorPhase->assistantCondition;
			if (Condition != EPhaseClearEnumType::VE_BossKill)
				LevelActor->ConditionCheck(EPhaseClearEnumType::VE_CharacterAllSurvival);
		}
	}
}

FName UBaseStageInfo::GetSelectedLevel()
{
	FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapTableKey);

	checkf(MapTableInfo != nullptr, TEXT("MapTableInfo is Empty "));

	return MapTableInfo->P_Map;
}

TArray< FName >	UBaseStageInfo::GetSubLevels()
{
	TArray< FName > tSubLevels;

	FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapTableKey);

	checkf(MapTableInfo != nullptr, TEXT("MapTableInfo is Empty "));
	//tSubLevels.Emplace(MapTableInfo->Sub_Level_Art);
	//tSubLevels.Emplace(MapTableInfo->Sub_Level_Design);
	
	if(!MapTableInfo->Sub_Level_ID.IsNone())
		tSubLevels.Emplace(MapTableInfo->Sub_Level_ID);

	return tSubLevels;
}


FName UBaseStageInfo::GetSubLevel()
{
	FName	 tSubLevel;

	if (MapTableKey.IsNone() == false)
	{
		FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapTableKey);

		checkf(MapTableInfo != nullptr, TEXT("MapTableInfo is Empty "));
		tSubLevel = MapTableInfo->Sub_Level_Design;
	}

	return tSubLevel;
}

EGamePlayModeTypeEnum	UBaseStageInfo::GetGameMode()
{
	if (MapTableKey != EXCEPTION_ZERO && MapTableKey != EXCEPTION_NONE)
	{
		FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapTableKey);

		checkf(MapTableInfo != nullptr, TEXT("MapTableInfo is Empty "));
		return MapTableInfo->GameMode;
	}

	return EGamePlayModeTypeEnum::VE_None;
}

float UBaseStageInfo::GetPlayTime()
{
	return GamePlayTime;
}

void UBaseStageInfo::SetSelectedMapInfo(FName InMapID, int32 InLevel)
{
	MapTableKey = InMapID;
	PlayLevel = InLevel;
}

void UBaseStageInfo::SetPlayTime()
{
// 	AProjectRGameMode*		RGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (Cast<ATestGameMode>(RGamePlayMode))
// 		return;
	ATestGameMode* TestMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));

	if (TestMode == nullptr)
	{
		FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapTableKey);

		checkf(MapTableInfo != nullptr, TEXT("MapTableInfo is Empty "));

		GamePlayTime = MapTableInfo->PlayTime;
	}	
}

// Timer
void UBaseStageInfo::SetTimer(float TotalGameSeconds)
{
	URGameInstance*		GameInstance		=	RGAMEINSTANCE(this);
	UWorld*				OwnerWorld			= GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	if(TimeHandler_GameTime.IsValid())
		OwnerWorld->GetTimerManager().ClearTimer(TimeHandler_GameTime);
	OwnerWorld->GetTimerManager().SetTimer(TimeHandler_GameTime, this, &UBaseStageInfo::TimeOut, TotalGameSeconds, false);

	bIsTimeOut = false;
}

void UBaseStageInfo::PauseTimer()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UWorld*				OwnerWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	OwnerWorld->GetTimerManager().PauseTimer(TimeHandler_GameTime);
}

void UBaseStageInfo::UnPauseTimer()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UWorld*				OwnerWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	OwnerWorld->GetTimerManager().UnPauseTimer(TimeHandler_GameTime);
}

bool UBaseStageInfo::isPauseTimer()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UWorld*				OwnerWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	return OwnerWorld->GetTimerManager().IsTimerPaused(TimeHandler_GameTime);
}

float UBaseStageInfo::GetElapsedGameTime()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UWorld*				OwnerWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	float time;
	if (OwnerWorld->GetTimerManager().TimerExists(TimeHandler_GameTime))
	{
		time = OwnerWorld->GetTimerManager().GetTimerElapsed(TimeHandler_GameTime);
		if (time < 0.0f)
			time = 0.0f;
	}
	else
	{
		time = GamePlayTime;
	}

	return time;
}

float UBaseStageInfo::GetRemainingGameTime()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UWorld*				OwnerWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	float time = OwnerWorld->GetTimerManager().GetTimerRemaining(TimeHandler_GameTime);

	if (time < 0.0f)
		time = 0.0f;

	return time;
}

void UBaseStageInfo::TimeOut()
{
	URGameInstance*		GameInstance		=	RGAMEINSTANCE(this);
// 	AProjectRGameMode*		RGamePlayMode		=	UUtilFunctionLibrary::GetGamePlayMode();
	UWorld*				OwnerWorld			=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameInstance->ColosseumManager != nullptr, TEXT("GameInstance->ColosseumManager is Empty "));
// 	checkf(RGamePlayMode != nullptr, TEXT("RGamePlayMode is Empty "));
	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));

	OwnerWorld->GetTimerManager().ClearTimer(TimeHandler_GameTime);

	bIsTimeOut = true;

// 	if (RGamePlayMode->GamePlayModeType == EGamePlayModeTypeEnum::VE_BattleGround || RGamePlayMode->GamePlayModeType == EGamePlayModeTypeEnum::VE_GuildRaid)
// 		return;

	UGamePlayBluePrintFunction::StageClearFail(EStageFailReason::VE_TIMEOUT_FAIL);
}

bool UBaseStageInfo::isMultiBoss()
{
	if (ArrBossInfo.Num() > 1)
		return true;

	return false;
}

void UBaseStageInfo::BindHideDialogueEvent()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance->EventManager != nullptr, TEXT("GameInstance->EventManager is Empty "));

	GameInstance->EventManager->OnHideDialogueEvent.Clear();
	GameInstance->EventManager->OnHideDialogueEvent.AddDynamic(this, &UBaseStageInfo::OnHideDialog);
}

void UBaseStageInfo::OnHideDialog()
{
	URGameInstance*		GameInstance		=	RGAMEINSTANCE(this);
	UWorld*				OwnerWorld			=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
// 	UIGamePlayMode*		GamePlayMode		=	UUtilFunctionLibrary::GetGamePlayMode();
	AProjectRGameMode*	pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	
	checkf(GameInstance->EventManager != nullptr, TEXT("GameInstance->EventManager is Empty "));
// 	checkf(GameInstance->GameObjectMgr != nullptr, TEXT("GameInstance->GameObjectMgr is Empty "));
	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));
// 	checkf(GamePlayMode != nullptr, TEXT("GamePlayMode is Empty "));
	checkf(pGameMode != nullptr, TEXT("pGameMove is Empty"));

	if (GameInstance->EventManager->OnHideDialogueEvent.IsAlreadyBound(this, &UBaseStageInfo::OnHideDialog))
		GameInstance->EventManager->OnHideDialogueEvent.RemoveDynamic(this, &UBaseStageInfo::OnHideDialog);

	UGamePlayBluePrintFunction::SetDialog(false);
	
	if(LevelActor && LevelActor->CurPhase == 0)
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Start, true);

	
	UnPauseTimer();

	if(LevelActor)
		LevelActor->ConditionCheck(EPhaseClearEnumType::VE_Dialogue);
}

void UBaseStageInfo::BindPlayEventSequence(TArray<AActor*>& InBoss)
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	checkf(GameInstance->SequenceManager != nullptr, TEXT("GameInstance->SequenceManager is Empty "));

	FName CharID = UCharacterHelpFunctionLibrary::GetCharacterID(InBoss[0]);

	if (CharID.IsValid() == false)
		return;

	FCharacterTableInfo* CharacterTable = UTableManager::GetInstancePtr()->GetCharacterNpcRow(CharID);

	checkf(CharacterTable != nullptr, TEXT("CharacterTable is Empty "));

	if (CharacterTable->SpawnSequence.IsNone())
		return;

	ABaseSequenceActor* SequenceActor = GameInstance->SequenceManager->PlaySequenceAtTargetActorLocation(CharacterTable->SpawnSequence, ESequenceActorType::VE_General, InBoss[0]);
	if (false == IsValid(SequenceActor))
		return;

	if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &UBaseStageInfo::OnSequenceEndSpawnActor))
		SequenceActor->OnSequenceEndEvent.AddDynamic(this, &UBaseStageInfo::OnSequenceEndSpawnActor);
}

void UBaseStageInfo::OnSequenceEndSpawnActor(FName InSequence)
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
// 	UIGamePlayMode*		RGamePlayMode	=	UUtilFunctionLibrary::GetGamePlayMode();
	UWorld*				OwnerWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	AProjectRGameMode*	pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	
// 	checkf(GameInstance->GameObjectMgr != nullptr, TEXT("GameInstance->GameObjectMgr is Empty "));
	checkf(GameInstance->SequenceManager != nullptr, TEXT("GameInstance->SequenceManager is Empty "));
// 	checkf(RGamePlayMode != nullptr, TEXT("RGamePlayMode is Empty "));
	checkf(OwnerWorld != nullptr, TEXT("OwnerWorld is Empty "));
	checkf(pGameMode != nullptr, TEXT("pGameMode is Empty"));
	TArray<AActor*> Boss = GetBoss();

	if (Boss.GetAllocatedSize() == 0)
		return;

	UUtilFunctionLibrary::ViewTargetBaseCamera(OwnerWorld);

	/*Always make the camera look at LeaderCharacter in campaign mode */
	AStagePlayerController* StagePC = Cast<AStagePlayerController>(UGameplayStatics::GetPlayerController(OwnerWorld, 0));
	if(StagePC && StagePC->GetSelectedCharacter())
		StagePC->ActorSettingOnTheCamera(StagePC->GetSelectedCharacter());
		

// 	RGamePlayMode->SetAllObjectVisible(true);
// 	RGamePlayMode->SetAllObjectBreak();
// 	RGamePlayMode->SetAllObjectHPBarVisible(true);

	pGameMode->ObjectHelper.SetAllObjectVisible(true);
	pGameMode->ObjectHelper.SetAllObjectBreak();
	pGameMode->ObjectHelper.SetAllObjectHPBarVisible(true);

	UUIFunctionLibrary::OnShowInGameHUD(OwnerWorld, true);
	UUIFunctionLibrary::InvalidateJoystick(OwnerWorld);

	UnPauseTimer();

	AttachBossEffect();

// 	UBaseCampMode* RBaseCampMode = Cast<UBaseCampMode>(RGamePlayMode);

	bool bExe = false;

	GameInstance->SequenceManager->CleanUpSequence(InSequence);

	UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_BossSpawnSequence);
}

void UBaseStageInfo::AttachBossEffect()
{
	TArray<AActor*> Boss = GetBoss();

	if (Boss.GetAllocatedSize() == 0)
		return;

	AActor* BossCharacter = Boss[0];
	if (BossCharacter == nullptr)
		return;

	auto BossActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BossCharacter);
	if (BossActor_BattleEntityComponent == nullptr)
		return;

	FName effectName = TEXT("BossEffect");
	UParticleSystem* Particle = UUtilFunctionLibrary::GetParticleSystemFromTable(effectName);
	if (IsValid(Particle))
	{
		auto BossActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(BossCharacter);
		if (BossActor_EntityRenderComponent)
		{
			auto Effectcomp = BossActor_EntityRenderComponent->AttachDieDestroyEffect(Particle, SOCKET_ROOT);
			if (IsValid(Effectcomp))
			{
				float MeshScaleFactor = 1.0f;
				int32 fUnitRadius = BossActor_BattleEntityComponent->GetBattleInfo()->GetCharacterProperty<int32>(ECharacterPropertyTypeEnum::VE_UnitRadius);
				float fFinalRadius = (fUnitRadius / RADIUSSIZE_DEFAULT) * MeshScaleFactor;
				Effectcomp->SetRelativeScale3D(FVector(fFinalRadius, fFinalRadius, 1.0f));
			}
		}
	}
}

void UBaseStageInfo::SetLevelActorBP()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(this);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	FMapTableInfo*  MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapTableKey);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);
	checkf(MapTableInfo != nullptr, TEXT("MapTableInfo is Empty "));

	UClass*		LevelActorClass = MapTableInfo->LevelActor.LoadSynchronous();
	if (LevelActorClass)
	{
		LevelActor = GameWorld->SpawnActor<ALevelActor>(LevelActorClass);
		checkf(LevelActor != nullptr, TEXT("Level Is Empty(%s)"), *(MapTableInfo->LevelActor.GetAssetName()));
	}
}
