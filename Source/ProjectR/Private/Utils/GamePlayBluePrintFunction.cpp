// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "Kismet/GameplayStatics.h"
#include "Object/SpawnerTransform.h"
#include "Object/WallActor.h"
#include "GameState/Stage/Spawner/SpawnBox.h"
#include "GameState/Stage/BaseLevelScriptActor.h"
#include "GameState/Stage/Spawner/LevelActor.h"
#include "Character/Components/SpeechBubbleComponent.h"
#include "Character/Components/WaypointComponent.h"
#include "CustomActors/BaseSequenceActor.h"
#include "DirectionTrackManager.h"

#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "TutorialGameMode.h"

// Sets default values
bool UGamePlayBluePrintFunction::bInDialog				=	false;
bool UGamePlayBluePrintFunction::bIsFirstGameStart		=	false;
bool UGamePlayBluePrintFunction::bIsElevator			=	false;
float UGamePlayBluePrintFunction::Duration				=	5.0f;

TArray<int32> UGamePlayBluePrintFunction::SavedDialogueInfo;
TMap<int32, FCustomizeDialogue> UGamePlayBluePrintFunction::SavedInfoMap;

void UGamePlayBluePrintFunction::BP_LoadData(FName InTagID)
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine->GetWorld());
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GameWorld, ASpawnBox::StaticClass(), OutActors);
	for (AActor* actor : OutActors)
	{
		if (IsValid(actor) && actor->Tags.Contains(InTagID) == false)
		{
			actor->SetActorEnableCollision(false);
			actor->SetActorHiddenInGame(true);
			actor->SetActorTickEnabled(false);
		}
	}
	UGameplayStatics::GetAllActorsOfClass(GameWorld, AWallActor::StaticClass(), OutActors);
	for (AActor* actor : OutActors)
	{
		if (IsValid(actor) && actor->Tags.Contains(InTagID) == false)
		{
			actor->SetActorEnableCollision(false);
			actor->SetActorHiddenInGame(true);
			actor->SetActorTickEnabled(false);
		}
	}
 	UGameplayStatics::GetAllActorsOfClass(GameWorld, ASpawnerTransform::StaticClass(), OutActors);
 	for (AActor* actor : OutActors)
 	{
 		if (IsValid(actor) && actor->Tags.Contains(InTagID) == false)
		{
			actor->SetActorEnableCollision(false);
			actor->SetActorHiddenInGame(true);
			actor->SetActorTickEnabled(false);
		}
 	}
}

void UGamePlayBluePrintFunction::BP_StartMapPhase()
{
	return;
}

void UGamePlayBluePrintFunction::SpawnPlayer(FName InSpawnID)
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	AActor* SpawnActorPosition = UUtilFunctionLibrary::FindSpawnBox(InSpawnID);

 	if(SpawnActorPosition)
		SpawnPC(SpawnActorPosition);
}

void UGamePlayBluePrintFunction::SpawnMonster(FName InSpawnID, FName InUnitID, int32 InLevel /*= 1*/, int32 InSightRange /*= 5000*/, EMemberMultiplierCategory InMemberMultiplier /*= EMemberMultiplierCategory::VE_NONE*/, float InStageHpMultiplier /*= 1.0f*/, float InStageAtkMultiplier /*= 1.0f*/, bool IsBattleObject /*= false*/)
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);
	 
	//UIGamePlayMode*	GameMode = UUtilFunctionLibrary::GetGamePlayMode();
	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
	if (GameMode == nullptr)
	 	return;
	 
	if (GameMode->GameModeState == EGameModeStateEnum::VE_End || GameMode->GameModeState == EGameModeStateEnum::VE_Result)
	 	return;

	AActor* SpawnActorPosition = UUtilFunctionLibrary::FindSpawnBox(InSpawnID);

	if (SpawnActorPosition == nullptr)
		return;

	AActor* SpawnCharacter = nullptr;

	ECharacterCategoryTypeEnum	CatecoryType = ECharacterCategoryTypeEnum::VE_Npc;

	if (IsBattleObject)
		CatecoryType = ECharacterCategoryTypeEnum::VE_BattleObject;

	if(SpawnActorPosition)
		SpawnCharacter = SpawnNPC(SpawnActorPosition, InUnitID, CatecoryType, GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, InLevel, InSightRange, InMemberMultiplier, InStageHpMultiplier, InStageAtkMultiplier);

	ALevelActor* LevelActor = BaseStageInfo->GetLevelActor();

	if (LevelActor)
	{
		ALevelActorPhase* LevelActorPhase = LevelActor->GetLevelActorPhase();
		if (LevelActorPhase)
			LevelActorPhase->MonsterSpawnCount++;
		LevelActor->SetSpecificMonster(InSpawnID, SpawnCharacter);
	}
}

void UGamePlayBluePrintFunction::SpawnNpc(FName InSpawnID, FName InUnitID, int32 InLevel /*= 1*/, int32 InSightRange /*= 5000*/, EMemberMultiplierCategory InMemberMultiplier /*= EMemberMultiplierCategory::VE_NONE*/, float InStageHpMultiplier /*= 1.0f*/, float InStageAtkMultiplier /*= 1.0f*/, bool IsBattleObject /*= false*/)
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
	if (GameMode == nullptr)
		return;

	if (GameMode->GameModeState == EGameModeStateEnum::VE_End || GameMode->GameModeState == EGameModeStateEnum::VE_Result)
		return;

	AActor* SpawnActorPosition = UUtilFunctionLibrary::FindSpawnBox(InSpawnID);

	ECharacterCategoryTypeEnum	CatecoryType = ECharacterCategoryTypeEnum::VE_Npc;

	if (IsBattleObject)
		CatecoryType = ECharacterCategoryTypeEnum::VE_BattleObject;

	

	if (SpawnActorPosition)
		SpawnNPC(SpawnActorPosition, InUnitID, CatecoryType, GroupKey_SINGLE_MY, PartyID_NPC_HELP, InLevel, InSightRange, InMemberMultiplier, InStageHpMultiplier, InStageAtkMultiplier);
}

void UGamePlayBluePrintFunction::SpawnBoss(FName InSpawnID, FName InUnitID, int32 InLevel /*= 1*/, int32 InSightRange /*= 5000*/, EMemberMultiplierCategory InMemberMultiplier /*= EMemberMultiplierCategory::VE_NONE*/, float InStageHpMultiplier /*= 1.0f*/, float InStageAtkMultiplier /*= 1.0f*/, bool IsBattleObject /*= false*/)
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
	if (GameMode == nullptr)
		return;

	if (GameMode->GameModeState == EGameModeStateEnum::VE_End || GameMode->GameModeState == EGameModeStateEnum::VE_Result)
		return;

	AActor* SpawnActorPosition = UUtilFunctionLibrary::FindSpawnBox(InSpawnID);

	AActor* SpawnCharacter = nullptr;

	ECharacterCategoryTypeEnum	CatecoryType = ECharacterCategoryTypeEnum::VE_Npc;

	if (IsBattleObject)
		CatecoryType = ECharacterCategoryTypeEnum::VE_BattleObject;

	FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(InUnitID);
	if (TableInfo)
		InLevel = TableInfo->Level;

	if (SpawnActorPosition)
		SpawnCharacter = SpawnNPC(SpawnActorPosition, InUnitID, CatecoryType, GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, InLevel, InSightRange, InMemberMultiplier, InStageHpMultiplier, InStageAtkMultiplier);

	ALevelActor* LevelActor = BaseStageInfo->GetLevelActor();

	if (LevelActor)
		LevelActor->SetSpecificMonster(InSpawnID, SpawnCharacter);
}

void UGamePlayBluePrintFunction::PlaySequence(bool isSkip)
{
	

	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_BossSpawnSequence);
}

void UGamePlayBluePrintFunction::SetActived(FName InObjectID, bool inActived)
{
	URGameInstance*	GameInstance		=	RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld			=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	AActor*			Object				=	UUtilFunctionLibrary::FindWall(InObjectID);

	if (inActived == false)
	{
		if (Object)
		{
			Object->SetActorHiddenInGame(true);
			Object->SetActorEnableCollision(false);
		}
	}
	else
	{
		if (Object)
		{
			Object->SetActorHiddenInGame(false);
			Object->SetActorEnableCollision(true);
		}
	}
}

void UGamePlayBluePrintFunction::ShowDialogue(TArray<int32> DialogueIndexes, TMap<int32, FCustomizeDialogue> OverrideMap)
{
	URGameInstance*	GameInstance	=	RGAMEINSTANCE(GEngine);
	UBaseStageInfo* BaseStageInfo	=	GetBaseStageInfo();
	UWorld*			GameWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);
	checkf(BaseStageInfo != nullptr, TEXT("[%s]BaseStageInfo is Empty"), __FUNCTION__);

	if (GameInstance->IsAutoPlayMode())
		return;
	
	FTOWER_STATUS TowerStatus = GameInstance->CampaignManager->TowerStatus;
	if(!GameInstance->OptionManager->IsStoryMatineeView())
	{
		if (TowerStatus.clearGrades.IsValidIndex(BaseStageInfo->StageIndex) &&
			TowerStatus.clearGrades[BaseStageInfo->StageIndex] != 0)
		{
			ALevelActor* LvActor = BaseStageInfo->GetLevelActor();
			if (LvActor == nullptr/*LvActor->CurPhase == 0*/)
				UUIFunctionLibrary::ReadyToStartUI_Campaign(BaseStageInfo->StageID);
			else
			{
				BaseStageInfo->OnHideDialog();
				LvActor->SetPhaseCheck(true);
				LvActor->ConditionCheck(EPhaseClearEnumType::VE_Dialogue);
			}

			return;
		}
	}

	SavedDialogueInfo = DialogueIndexes;
	SavedInfoMap = OverrideMap;
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Story_Dialogue);
	bInDialog = true;

}

void UGamePlayBluePrintFunction::Deferred_ShowDialogue()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo* BaseStageInfo = GetBaseStageInfo();
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);
	checkf(BaseStageInfo != nullptr, TEXT("[%s]BaseStageInfo is Empty"), __FUNCTION__);

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (IsValid(pGameMode))
	{
		

		if (!pGameMode->GetClass()->IsChildOf<ATutorialGameMode>())
		{
			BaseStageInfo->PauseTimer();
			BaseStageInfo->BindHideDialogueEvent();
		}
	}
	
	TArray<FDialogueInfo> Dialogues;
	for (size_t i = 0; i < SavedDialogueInfo.Num(); ++i)
	{
		FDialogueInfo* info = UTableManager::GetInstancePtr()->GetDialogueTableRow(FName(*FString::FromInt(SavedDialogueInfo[i])));
		if (info)
			Dialogues.Emplace(*info);
	}


	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UUserWidget* Widget = UUtilFunctionLibrary::CreateUserWidget(RGameInstance->GetWorld(), WIDGETID_DIALOGUEUI);
	if (Widget)
	{
		UUP_Dialogue* DialogueWidget = Cast<UUP_Dialogue>(Widget);
		if (!DialogueWidget->IsInViewport())
			DialogueWidget->AddToViewport(DIALOGUE_ZORDER);

		DialogueWidget->DialogueIndexes = SavedDialogueInfo;
		DialogueWidget->Init_AfterCreation(Dialogues, SavedInfoMap);

	}
}

void UGamePlayBluePrintFunction::SetDialog(bool InDialog)
{
	UGamePlayBluePrintFunction::bInDialog = InDialog;
}

bool UGamePlayBluePrintFunction::IsShowDialog()
{
	return UGamePlayBluePrintFunction::bInDialog;
}

void UGamePlayBluePrintFunction::SetFirstGameStart(bool InFirstGameStart)
{
	UGamePlayBluePrintFunction::bIsFirstGameStart = InFirstGameStart;
}

bool UGamePlayBluePrintFunction::GetFirstGameStart()
{
	return UGamePlayBluePrintFunction::bIsFirstGameStart;
}

void UGamePlayBluePrintFunction::NextWayPoint(FName InWayPointID)
{
	if (!InWayPointID.IsValid() || InWayPointID.IsNone())
		return;

	URGameInstance*			GameInstance	=	RGAMEINSTANCE(GEngine);
	UWorld*					GameWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	AActor*					WayPointActor	=	UUtilFunctionLibrary::FindWaypoint(InWayPointID);
	checkf(GameInstance->AutoPlayManager != nullptr, TEXT("[%s]AutoPlayManager is Empty"), __FUNCTION__);

	if (WayPointActor)
	{
 		FSpawnerTransformInfo	WayPoint;

		WayPoint.ID = WayPointActor->GetUniqueID();
		WayPoint.Position = WayPointActor->GetActorLocation();
		WayPoint.Rotation = WayPointActor->GetActorRotation();

		GameInstance->AutoPlayManager->SetNextWayPointTransform(WayPoint);
	}
}

void UGamePlayBluePrintFunction::ClearWayPoint()
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UWorld*					GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameInstance->AutoPlayManager != nullptr, TEXT("[%s]AutoPlayManager is Empty"), __FUNCTION__);

	GameInstance->AutoPlayManager->ClearWayPoint();
}

void UGamePlayBluePrintFunction::SetPause(bool InPause)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);

	UGameplayStatics::SetGamePaused(GameInstance, InPause);
}

bool UGamePlayBluePrintFunction::IsPause()
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);

	return UGameplayStatics::IsGamePaused(GameInstance);
}

void UGamePlayBluePrintFunction::SpeechBubblePlay(FName InUnitID, FName InSpeechKey, float InSpeechShowTime, float InDelay)
{
	UBaseStageInfo* BaseStageInfo = GetBaseStageInfo();

	TArray<AActor*> TargetActors = BaseStageInfo->GetHero();
	TargetActors.Append(BaseStageInfo->GetMonster());
	 

	if (TargetActors.GetAllocatedSize() <= 0)
		return;

	for (AActor* actor : TargetActors)
	{
		if (actor == nullptr)
			continue;
		IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(actor);

		if (EntityBaseProperty == nullptr)
			continue;

		if (EntityBaseProperty->GetCharacterID() == InUnitID)
		{
			UEntityRenderComponent* RenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(actor);
			if (IsValid(RenderComp))
			{
				FTransform HeadUpTransform = RenderComp->GetHeadUpTransform();
				USpeechBubbleComponent* SpeechBubbleComponent = UUtilFunctionLibrary::AttachCharacterSpeechBubbleWidget(actor, FName("WBP_SpeechBubble"), HeadUpTransform);
				if (IsValid(SpeechBubbleComponent))
				{
					SpeechBubbleComponent->SetSpeechBubbleShowTime(InSpeechShowTime);
					SpeechBubbleComponent->AddSpeechBubble(InSpeechKey, InDelay);
				}
			}
			return;
		}
	}
}

void UGamePlayBluePrintFunction::IndicatorPlay(FName KeyName, float InDuration/*= 0.0f*/, bool bNoAni/*= false*/, bool bBottomMsg /*= false*/)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);

	FText DisplayText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, KeyName);

	UUtilFunctionLibrary::ShowIndicator(DisplayText, InDuration, bNoAni, bBottomMsg);
}

void UGamePlayBluePrintFunction::NpcNextWayPoint(FName InWayPointID)
{
	if (!InWayPointID.IsValid() || InWayPointID.IsNone())
		return;

	URGameInstance*			GameInstance	=	RGAMEINSTANCE(GEngine);
	UBaseStageInfo*			BaseStageInfo	=	GetBaseStageInfo();
	UWorld*					GameWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	AActor*					WayPointActor = UUtilFunctionLibrary::FindWaypoint(InWayPointID);
	checkf(GameInstance->AutoPlayManager != nullptr, TEXT("[%s]AutoPlayManager is Empty"), __FUNCTION__);

	NpcClearWayPoint();

	if (WayPointActor)
	{
		for (AActor* Npc : BaseStageInfo->ArrHeroInfo)
		{
			UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Npc);
			if (EntityAIComponent)
			{
				UWaypointComponent* WaypointComponent = EntityAIComponent->GetWaypointComponent();
				if (WaypointComponent == nullptr)
				{
					EntityAIComponent->AttachWaypointComponent();
					WaypointComponent = EntityAIComponent->GetWaypointComponent();
				}
				WaypointComponent->AddWaypoint(WayPointActor->GetActorLocation(), 0.1f);
				EntityAIComponent->AIRestart();
			}
		}
	}
}

void UGamePlayBluePrintFunction::NpcClearWayPoint()
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo*			BaseStageInfo = GetBaseStageInfo();

	for (AActor* Npc : BaseStageInfo->ArrHeroInfo)
	{
		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Npc);
		UWaypointComponent* WaypointComponent = EntityAIComponent->GetWaypointComponent();

		EntityAIComponent->StopMove();

		if (WaypointComponent == nullptr)
		{
			EntityAIComponent->AttachWaypointComponent();
			WaypointComponent = EntityAIComponent->GetWaypointComponent();
		}
		WaypointComponent->ClearWaypoint();
	}
}

void UGamePlayBluePrintFunction::InitCameraTransform(FName InObjectID)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo*			BaseStageInfo = GetBaseStageInfo();
	UWorld*					GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);	
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
	if (GameMode == nullptr)
		return;

	if (GameMode->GameModeState == EGameModeStateEnum::VE_End || GameMode->GameModeState == EGameModeStateEnum::VE_Result)
		return;

	
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	checkf(pGameMode != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);
	AActor*			Object = UUtilFunctionLibrary::FindSpawnBox(InObjectID);
	checkf(Object != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	pGameMode->InitCameraTransform(Object->GetTransform().GetLocation());
}

void UGamePlayBluePrintFunction::GameStartEvent()
{
	URGameInstance*			GameInstance	=	RGAMEINSTANCE(GEngine);
	UBaseStageInfo*			BaseStageInfo	=	GetBaseStageInfo();
	UWorld*					GameWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	if (UGamePlayBluePrintFunction::bIsFirstGameStart == true)
		return;

	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
	if (GameMode == nullptr)
		return;

	if (GameMode->GameModeState == EGameModeStateEnum::VE_End || GameMode->GameModeState == EGameModeStateEnum::VE_Result)
		return;

	if (UUtilFunctionLibrary::GetGamePlayModeEnum() == EGamePlayModeTypeEnum::VE_Campaign)
	{
		ALevelActor* LevelActor = BaseStageInfo->GetLevelActor();

		if (LevelActor)
		{
			if (LevelActor->CurPhase == 0)
				LevelActor->ConditionCheck(EPhaseClearEnumType::VE_Dialogue);
			//LevelActor->GameStart();
		}
	}
	
	if (GameInstance->IsAutoPlayMode())
		UUIFunctionLibrary::ShowAutoPlayPopup(EBattleRepeatPopupState::VE_GamePlay);
}

void UGamePlayBluePrintFunction::Die(AActor* InDiePlayer)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo*			BaseStageInfo = GetBaseStageInfo();
	UWorld*					GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
	if (GameMode == nullptr)
		return;

	if (GameMode->GameModeState == EGameModeStateEnum::VE_End || GameMode->GameModeState == EGameModeStateEnum::VE_Result)
		return;

	if (UUtilFunctionLibrary::GetGamePlayModeEnum() != EGamePlayModeTypeEnum::VE_Campaign)
		return;
	
	ALevelActor*	LevelActor = BaseStageInfo->GetLevelActor();
	if (LevelActor)
	{
		if (LevelActor->isNpcHeroDie(InDiePlayer) == true)
		{
			UGamePlayBluePrintFunction::StageClearFail(EStageFailReason::VE_PROTECT_NPC_DIE_FAIL);
			return;
		}

		TArray<FHeroSlotInfo> SlotInfoArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
		for (size_t i = 0; i < SlotInfoArray.Num(); ++i)
		{
			if (SlotInfoArray[i].SpawnedTargetActor == InDiePlayer)
			{
				ALevelActorPhase* PhaseInfo = LevelActor->GetLevelActorPhase();
				if (PhaseInfo)
				{
					if (PhaseInfo->ClearCondition == EPhaseClearEnumType::VE_CharacterAllSurvival)
					{
						UGamePlayBluePrintFunction::StageClearFail(EStageFailReason::VE_ALL_SURVIVAL_FAIL);
						return;
					}
				}
			}
		}
	}

	ABasePlayerController* BasePlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);
	if (BasePlayerCtrl)
	{
		if (BasePlayerCtrl->IsSelectedCharacter(InDiePlayer))
		{
			UGamePlayBluePrintFunction::StageClearFail(EStageFailReason::VE_LEADER_DIE_FAIL);
			return;
		}
	}

	BaseStageInfo->DeletePlayer(InDiePlayer);
	BaseStageInfo->DeleteHero(InDiePlayer);
	BaseStageInfo->DeleteBoss(InDiePlayer);
	BaseStageInfo->DeleteMonster(InDiePlayer);
}

void UGamePlayBluePrintFunction::SpawnMultiBoss(TArray<AActor*>& InBoss)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance->EventManager != nullptr, TEXT("[%s]RGAMEINSTANCE(GEngine)->EventManager is Empty "), __FUNCTION__);

	UWorld*					GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_Stage_FailEvent)
		return;

	AActor* BossCharacter = nullptr;

	if (InBoss.IsValidIndex(0))
		BossCharacter = InBoss[0];

	if (BossCharacter == nullptr)
		return;

	GameInstance->EventManager->OnMultiBossSpawn.Broadcast(InBoss);

	UEntityAIComponent* BossActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(BossCharacter);

	if (BossActor_EntityAIComponent == nullptr)
		return;

	URBaseIngameHUD* OneGameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());
	if (OneGameHUD)
		OneGameHUD->HPBarSwitcher->SetActiveWidgetIndex(3);

	ABasePlayerController* BasePlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);
	if (BasePlayerCtrl)
		BossActor_EntityAIComponent->SetForcePauseProcess(BasePlayerCtrl->StageBossSlowTime, BasePlayerCtrl->StageBossSlowRate, BasePlayerCtrl->StageBossPostprocessTime, false);
}

void UGamePlayBluePrintFunction::SpawnSingleBoss(TArray<AActor*>& InBoss)
{
	URGameInstance*			GameInstance	=	RGAMEINSTANCE(GEngine);
	UWorld*					GameWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(GameInstance->EventManager != nullptr, TEXT("[%s]RGAMEINSTANCE(GEngine)->EventManager is Empty "), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_Stage_FailEvent)
		return;

	AActor* BossCharacter = InBoss[0];

	if (IsValid(BossCharacter) == false)
		return;

	UBattleEntityComponent* BossActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BossCharacter);

	GameInstance->EventManager->OnBossSpawn.Broadcast(BossCharacter);

	UEntityAIComponent* BossActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(BossCharacter);

	if (BossActor_EntityAIComponent == nullptr)
		return;
	URBaseIngameHUD* OneGameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());
	if (OneGameHUD)
	{
		if (!OneGameHUD->HPBarSwitcher->IsVisible())
			OneGameHUD->HPBarSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		OneGameHUD->HPBarSwitcher->SetActiveWidgetIndex(0);
	}
		

	ABasePlayerController* BasePlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);
	if (BasePlayerCtrl)
		BossActor_EntityAIComponent->SetForcePauseProcess(BasePlayerCtrl->StageBossSlowTime, BasePlayerCtrl->StageBossSlowRate, BasePlayerCtrl->StageBossPostprocessTime, false);
}

void UGamePlayBluePrintFunction::PlayEventSequence(bool isEnableSkip)
{
	URGameInstance*	GameInstance	=	RGAMEINSTANCE(GEngine);
	UBaseStageInfo*	BaseStageInfo	=	GetBaseStageInfo();
	UWorld*			GameWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(GameInstance->SequenceManager != nullptr, TEXT("[%s]RGAMEINSTANCE(GEngine)->SequenceManager is Empty "), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("UWorld is Empty"), __FUNCTION__);

	TArray<AActor*> Boss = BaseStageInfo->GetBoss();

	if (Boss.GetAllocatedSize() == 0)
		return;

	BaseStageInfo->BindPlayEventSequence(Boss);

	TArray< AActor* > IgonreActors;

	if (Boss.GetAllocatedSize() > 0)
		IgonreActors = Boss;

// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		pGamePlayMode->SetAllObjectVisible(false, IgonreActors);
// 		pGamePlayMode->SetAllObjectPause(MAX_FLOAT, 0.0f, Boss[0]);
// 		pGamePlayMode->SetAllObjectHPBarVisible(false);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.SetAllObjectVisible(false, IgonreActors);
		pGameMode->ObjectHelper.SetAllObjectPause(MAX_FLOAT, 0.0f, Boss[0]);
		pGameMode->ObjectHelper.SetAllObjectHPBarVisible(false);
	}
	UUIFunctionLibrary::OnShowInGameHUD(GameWorld, false);
	UUIFunctionLibrary::ActivateJoystick(GameWorld, false);

	BaseStageInfo->PauseTimer();

	UUtilFunctionLibrary::BossName(Boss[0]);
}

void UGamePlayBluePrintFunction::SpawnPC(AActor* InSpawnPosition)
{
	URGameInstance*			GameInstance	=	RGAMEINSTANCE(GEngine);
	UBaseStageInfo*			BaseStageInfo	=	UGamePlayBluePrintFunction::GetBaseStageInfo();
	UWorld*					GameWorld		=	GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("UWorld is Empty"), __FUNCTION__);

	FTransform	TargetTransform = InSpawnPosition->GetTransform();
	FQuat	ActorQuatnion = InSpawnPosition->GetActorRotation().Quaternion();
	TargetTransform.SetRotation(ActorQuatnion);

	UUtilFunctionLibrary::FirstSpawnPos(GameWorld, TargetTransform.GetLocation(), TargetTransform.GetRotation().Rotator());

	UUIFunctionLibrary::AttachHUDPage(GameWorld);
	UUIFunctionLibrary::OnShowInGameHUD(GameWorld, true);

	URBaseIngameHUD* InGameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());
	if (InGameHUD)
	{
		AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameWorld));
		if (StagePC)
			StagePC->ActorSettingOnTheCamera(StagePC->GetLeaderCharacter(), InGameHUD->CharacterFocusCameraTime, InGameHUD->CharacterFocusCameraExp);
	}

}

AActor* UGamePlayBluePrintFunction::SpawnNPC(AActor* InSpawnPosition, FName InUnitID, ECharacterCategoryTypeEnum InCategoryType, int32 InGroupKey, int32 InPartyID, int32 InLevel, int32 InSightRange, EMemberMultiplierCategory InMemberMultiplier, float InStageHpMultiplier, float InStageAtkMultiplier)
{
	URGameInstance*			GameInstance	=	RGAMEINSTANCE(GEngine);
	UBaseStageInfo*			BaseStageInfo	= UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_Stage_FailEvent)
		return nullptr;

	checkf(GameInstance->BattleRecordManager != nullptr, TEXT("RGAMEINSTANCE(GEngine)->BattleRecordManager is Empty "));

	FTransform	TargetTransform = InSpawnPosition->GetTransform();
	FVector		RealLocation = TargetTransform.GetLocation();

	TargetTransform.SetLocation(RealLocation);
	CheckValidSpawnPos(RealLocation);

	FQuat	ActorQuatnion = InSpawnPosition->GetActorRotation().Quaternion();
	TargetTransform.SetRotation(ActorQuatnion);

	AActor*		SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacter(TargetTransform, InUnitID, InCategoryType, KID_NONE, HERO_UD_NONE, InGroupKey, InPartyID, PLAYER_UK_NONE, InLevel);

	if (SpawnedCharacter == nullptr)
		return nullptr;

	SpawnedCharacter->SetActorEnableCollision(true);

	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
	if (BattleComp == nullptr)
		return nullptr;
	UCharacterBattleInfo*	BattleInfo = BattleComp->GetBattleInfo();
	if (BattleInfo == nullptr)
		return nullptr;

	BattleInfo->CharacterProperty.SightRange = InSightRange;

	if (UUIFunctionLibrary::IsBossNpc(InUnitID) == true)
	{
		BattleComp->SetBoss(true);
		BaseStageInfo->SetBoss(SpawnedCharacter);
	}
	else
	{
		BaseStageInfo->SetNormalNpc(SpawnedCharacter, InGroupKey);
	}

	GameInstance->EventManager->OnSpawnActorFromCharacterSpawner.Broadcast(SpawnedCharacter);

	if (InPartyID == PartyID_NPC_ENEMY)
	{
		float MemberHPMultiplier = 1.0f;
		float MemberPAttackMultiplier = 1.0f;
		float MemberMAttackMultiplier = 1.0f;

		if (InMemberMultiplier != EMemberMultiplierCategory::VE_NONE)
		{
			FGrowthHeroMultiplier*	MemberMultiplierInfo = nullptr;
			FText					DisplayName = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EMemberMultiplierCategory"), (int32)InMemberMultiplier);
			MemberMultiplierInfo = UTableManager::GetInstancePtr()->GetHeroGrowthMultiplierRow(FName(*(DisplayName.ToString())));

			if (MemberMultiplierInfo)
			{
				MemberHPMultiplier = MemberMultiplierInfo->MaxHP;
				MemberPAttackMultiplier = MemberMultiplierInfo->PhysicalAttackPower;
				MemberMAttackMultiplier = MemberMultiplierInfo->MagicAttackPower;
			}
		}

		BattleInfo->CharacterProperty.MaxHP = (int32)((float)BattleInfo->CharacterProperty.MaxHP * InStageHpMultiplier * MemberHPMultiplier);
		BattleInfo->CharacterProperty.PhysicalAttackPower = (int32)((float)BattleInfo->CharacterProperty.PhysicalAttackPower * InStageAtkMultiplier * MemberPAttackMultiplier);
		BattleInfo->CharacterProperty.MagicAttackPower = (int32)((float)BattleInfo->CharacterProperty.MagicAttackPower * InStageAtkMultiplier * MemberMAttackMultiplier);

		BattleComp->InitBattleProperty();
	}

	GameInstance->BattleRecordManager->SetHeroBattleInfoDelegate(SpawnedCharacter);
	return SpawnedCharacter;
}

bool UGamePlayBluePrintFunction::CheckValidSpawnPos(FVector& SpawnPos)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UWorld*					GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(GameInstance->GridMapManager != nullptr, TEXT("RGAMEINSTANCE(GEngine)->GridMapManager is Empty "));
	checkf(GameWorld != nullptr, TEXT("GameWorld is Empty "));

	if (UMapFunctionLibrary::CanSpawnPostion(GameInstance, SpawnPos) == false)
	{
		FCellIndex AdjacentCell;
		FindValidLocation(SpawnPos, AdjacentCell);

		auto WorldPos = GameInstance->GridMapManager->GetCellWorldPos(AdjacentCell);
		if (WorldPos == FVector::ZeroVector)
			return false;

		SpawnPos = WorldPos;
	}

	return true;
}

bool UGamePlayBluePrintFunction::FindValidLocation(const FVector& SpawnPos, FCellIndex& AdjacentCell)
{
	if (UMapFunctionLibrary::GetAdjacentCellNearestWithTargetPositionOnNavyMesh(AdjacentCell, SpawnPos, 1, 5) == false)
		return false;

	return true;
}

void UGamePlayBluePrintFunction::StageClearSuccess()
{
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_VictoryPosition);
}

void UGamePlayBluePrintFunction::StageClearFail(EStageFailReason reason)
{
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_FailEvent);
	RGAMEINSTANCE(GEngine)->CampaignFailReson = reason;
}

UBaseStageInfo* UGamePlayBluePrintFunction::GetBaseStageInfo()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);

	ensureMsgf(GameInstance->BaseStageInfo != nullptr, TEXT("[ RGameInstance Is Empty ]"));
	checkf(GameInstance->BaseStageInfo != nullptr, TEXT("[ RGameInstance Is Empty ]"));

	return GameInstance->BaseStageInfo;
}

void UGamePlayBluePrintFunction::SetElevator(bool isElevator, float InDuration)
{
	UGamePlayBluePrintFunction::bIsElevator = isElevator;
	UGamePlayBluePrintFunction::Duration = InDuration;
}

bool UGamePlayBluePrintFunction::GetElevator()
{
	return UGamePlayBluePrintFunction::bIsElevator;
}

float UGamePlayBluePrintFunction::GetDuration()
{
	return UGamePlayBluePrintFunction::Duration;
}