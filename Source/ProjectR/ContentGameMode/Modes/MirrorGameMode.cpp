// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "MirrorGameMode.h"
#include "ContentGameMode/Controllers/MirrorPlayerController.h"
#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/InGameHUD/UP_InGameHUD_Mirror.h"
#include "Character/BattleBaseCharacter.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"
#include "LeaderControlHelper.h"



AMirrorGameMode::AMirrorGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = AMirrorPlayerController::StaticClass();
}

void AMirrorGameMode::InitGameMode()
{
	Super::InitGameMode();
	GameModeState = EGameModeStateEnum::VE_Init;
	GameContextState = EStageContextEnum::VE_None;

	GameInstance->BattleRecordManager->ResetBattleRecordData();
	UObjectFunctionLibrary::SetMyTeamKey(GroupKey_SINGLE_MY);
	UObjectFunctionLibrary::SetMyPartyID(PartyID_PC_MY);

	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
}

void AMirrorGameMode::PrepareGameMode()
{
	Super::PrepareGameMode();
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->bPlayAlone = false;
	GameModeState = EGameModeStateEnum::VE_Prepare;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (!pGameMode)
		return;
	
	GameInstance->PlayStateInfo->ReadyToPlay();

	if (GameInstance->GetCurrentStateType() == EGameStateEnum::VE_Play)
		USoundManager::GetInstancePtr()->PlayBattleBGMusic(BaseStageInfo->MapTableKey);

	pGameMode->ObjectHelper.SetBlackOut(false);

	StageTime = BaseStageInfo->GetPlayTime();
	BaseStageInfo->SetTimer(StageTime);
	BaseStageInfo->PauseTimer();

	/*Cache Resource*/
	ObjectHelper.AssetsQueue.Empty();
	int32 my_hero_count = GameInstance->MirrorDungeonManager->m_MirrorMyHeroUDs.Num();
	for (int n = 0; n < my_hero_count; ++n)
	{
		FHERO* Hero = GameInstance->RInventory->GetHeroWithUD(GameInstance->MirrorDungeonManager->m_MirrorMyHeroUDs[n]);
		if (Hero == nullptr)
			continue;

		pGameMode->ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, Hero->heroId);
	}
	FBATTLE_USER MatchTarget = GameInstance->MirrorDungeonManager->m_MatchList;
	for (FEQUIPED_HERO& equipedhero : MatchTarget.heros)
	{
		pGameMode->ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, equipedhero.heroId);
	}
	ObjectHelper.SetAssetResource_FromTables(ObjectHelper.AssetsQueue);

	UUP_LoadingScene* LoadingScene = Cast<UUP_LoadingScene>(RGAMEINSTANCE(this)->LoadingSceneManager->Widget);
	if (LoadingScene->IsValidLowLevel())
	{
		int32 iTemp = ObjectHelper.AssetsQueue.Num() / iPartition;
		LoadingScene->UnitPercent = 100.f / iTemp;
	}

	TArray<FSoftObjectPath> TargetAssets;
	for (size_t i = 0; i < iPartition; ++i)
	{
		if (ObjectHelper.AssetsQueue.IsValidIndex(0))
		{
			FSoftObjectPath target = ObjectHelper.AssetsQueue.Pop();
			TargetAssets.AddUnique(target);
		}
	}
	if (TargetAssets.Num())
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &AMirrorGameMode::OnCompleteResourcePreLoading));
	
}

void AMirrorGameMode::ReadyGameMode()
{
	Super::ReadyGameMode();
	GameModeState = EGameModeStateEnum::VE_Ready;
	
	UUIFunctionLibrary::AttachHUDPage(GetWorld());
	UUIFunctionLibrary::OnShowInGameHUD(GetWorld(), false);

	UMirrorDungeonManager* MirrorManager = GameInstance->MirrorDungeonManager;
	if (IsValid(MirrorManager))
	{
		MirrorManager->InitMirrorGuage();
	}

	GameInstance->EventManager->OnGameMode_ReadyToStart.RemoveDynamic(this, &AMirrorGameMode::OnReadyToStart);
	GameInstance->EventManager->OnGameMode_ReadyToStart.AddDynamic(this, &AMirrorGameMode::OnReadyToStart);

	FGameModeSequenceTableInfo* GameModeSequenceInfo = UTableManager::GetInstancePtr()->GetGameModeSequenceRow(GameModeSequenceKey);
	if (GameModeSequenceInfo)
	{
		if (false == GameModeSequenceInfo->StartSequence.IsNone())
		{
			ABaseSequenceActor* SequenceActor = GameInstance->SequenceManager->SpawnSequence(GameModeSequenceInfo->StartSequence);
			if (IsValid(SequenceActor))
			{
				if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &AMirrorGameMode::OnSequenceEndMirrorStart))
				{
					SequenceActor->OnSequenceEndEvent.AddDynamic(this, &AMirrorGameMode::OnSequenceEndMirrorStart);
				}
			}
		}
	}

	if (IsValid(GameInstance->RWidgetManager))
		GameInstance->RWidgetManager->ChangeInputModeUIOnly(false);

	UUserWidget* HUDWidget = UUIFunctionLibrary::GetInGameHUD();
	if (IsValid(HUDWidget))
	{
		UUP_InGameHUD_Mirror* InGameHUD_Arena = Cast<UUP_InGameHUD_Mirror>(HUDWidget);
		if (InGameHUD_Arena)
			InGameHUD_Arena->Init();
	}
}

void AMirrorGameMode::StartGameMode()
{
	Super::StartGameMode();
	
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
	UUIFunctionLibrary::OnShowInGameHUD(GetWorld(), true);

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	GameModeState = EGameModeStateEnum::VE_Start;
	GameContextState = EStageContextEnum::VE_Progress;

	if (BaseStageInfo)
		BaseStageInfo->SetTimer(StageTime);

	GameInstance->EventManager->OnGameMode_ReadyToStart.RemoveDynamic(this, &AMirrorGameMode::OnReadyToStart);
	GameInstance->MirrorDungeonManager->SetCameraModeAfterGameStart(EArenaCameraMode::VE_Sequence_1);
}

void AMirrorGameMode::EndGameMode()
{
	Super::EndGameMode();
	GameModeState = EGameModeStateEnum::VE_End;
	
	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (!pGameMode)
		return;
	
	UUIFunctionLibrary::AttachHUDPage(GetWorld(), false);
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);

	GameInstance->PlayStateInfo->SetClearTime(BaseStageInfo->GetElapsedGameTime());
	GameInstance->PlayStateInfo->SetRemainingTime(BaseStageInfo->GetRemainingGameTime());
	BaseStageInfo->PauseTimer();

	if (GameContextState == EStageContextEnum::VE_Fail)
	{
		TArray< TWeakObjectPtr< AActor > > ObjList;
		pGameMode->ObjectHelper.GetObjectList(ObjList);

		for (auto& Element : ObjList)
		{
			if (!Element.IsValid())
				continue;
			IEntityBaseProperty* Target_EntityBaseProperty = Cast< IEntityBaseProperty >(Element.Get());
			if (Target_EntityBaseProperty == nullptr)
				continue;
			UBattleEntityComponent* Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element.Get());
			if (!IsValid(Target_BattleEntityComponent))
				continue;
			if (Target_BattleEntityComponent->IsDead())
				continue;

			bool EnemyTarget = URenewal_BattleFunctionLibrary::IsEnemy(Target_EntityBaseProperty->GetPlayerUK(), Target_EntityBaseProperty->GetGroupKey(), PLAYER_UK_NONE, UObjectFunctionLibrary::GetMyTeamKey());

			// if this is my character, force to dead..
			if (false == EnemyTarget)
			{
				Target_BattleEntityComponent->SetHP(0);
				Target_BattleEntityComponent->ForceDead(Element.Get());
				UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Element.Get());
				if (EntityRenderComponent)
					EntityRenderComponent->ShowDeadDisapearEvent();
			}
			else
			{
#ifdef RULE_TESTCHANGE
				UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Element.Get());
				if (EntityRenderComponent)
					EntityRenderComponent->HpBarWidgetDestroy();
#else
				auto HPWidget = UCharacterHelpFunctionLibrary::GetHpBarWidgetComponent(Element.Get());
				if (HPWidget)
					HPWidget->DeleteWidgetDecorator();
#endif
			}
		}
	}
	else if (GameContextState == EStageContextEnum::VE_Success)
	{
		TArray< TWeakObjectPtr< AActor > > ObjList;
		pGameMode->ObjectHelper.GetObjectList(ObjList);
		for (auto& Element : ObjList)
		{
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element.Get());
			if (BattleEntityComponent && BattleEntityComponent->IsDead() == false)
			{
				auto HPWidget = UCharacterHelpFunctionLibrary::GetHpBarWidgetComponent(Element.Get());
				if (HPWidget)
					HPWidget->DeleteWidgetDecorator();
			}
		}
	}

	GameInstance->GridMapManager->LeaveFromPlayLevel();

	WinSequenceEnd = false;
	ReceiveMirrorPlayDoneRp = false;

	if (IsValid(GameInstance->MirrorDungeonManager))
	{
		GameInstance->MirrorDungeonManager->ResetCameraMode();
		GameInstance->MirrorDungeonManager->RequestPlayDone();
	}

	GameInstance->TimerManager->SetTimer(EndGameModeDelayHandler, this, &AMirrorGameMode::EndGameModeDelay, 2.0f, false);
}

void AMirrorGameMode::ResultGameMode()
{
	Super::ResultGameMode();
	GameModeState = EGameModeStateEnum::VE_Result;
	USoundManager::GetInstancePtr()->FadeOutBGM();

	if (IsValid(GameInstance->EventManager))
		GameInstance->EventManager->OnGameMode_ReadyToResult.RemoveDynamic(this, &AMirrorGameMode::OnReadyToResult);
}

void AMirrorGameMode::GamePlayModeEnd()
{
	Super::GamePlayModeEnd();
	
	if (IsValid(GameInstance->PlayModeManager))
	{
		GameInstance->PlayModeManager->PlayModeMirrorDungeonEnd();
	}
}

EStageContextEnum AMirrorGameMode::GetStageContext()
{
	UMirrorDungeonManager* MirrorManager = GameInstance->MirrorDungeonManager;
	if (!IsValid(MirrorManager))
		return EStageContextEnum::VE_Fail;

	//if Result is already Update then Not Check Win Deck
	if (GameContextState == EStageContextEnum::VE_Fail)
	{
		MirrorManager->SetMyWin(false);
		return GameContextState;
	}
	else if (GameContextState == EStageContextEnum::VE_Success)
	{
		MirrorManager->SetMyWin(true);
		return GameContextState;
	}

	if (bIsTimeOut)
	{
		GameContextState = EStageContextEnum::VE_Fail;
		MirrorManager->DecideWinner();
	}
	else
	{
		if (MirrorManager->IsCombatEnd())
		{
			if (MirrorManager->GetAliveMyHero() == 0)
			{
				GameContextState = EStageContextEnum::VE_Fail;
				MirrorManager->SetMyWin(false);
				return GameContextState;
			}
			else if (MirrorManager->GetAliveMyHero() > MirrorManager->GetAliveEnemy()) {
				GameContextState = EStageContextEnum::VE_Success;
				MirrorManager->SetMyWin(true);
				return GameContextState;
			}
			else
			{
				GameContextState = EStageContextEnum::VE_Fail;
				MirrorManager->SetMyWin(false);
				return GameContextState;
			}
		}
	}

	return GameContextState;
}

void AMirrorGameMode::OnReadyToStart()
{
	NextGameModeState= EGameModeStateEnum::VE_Start;
	
	if (false == ShowUI_PVP_VS)
	{
		ShowUI_PVP_VS = true;
		UUIFunctionLibrary::ReadyToStartUI_PVP();
	}
	else if (false == ShowUI_PVP_Start)
	{
		ShowUI_PVP_Start = true;
		GameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_PVP_ReadyToStart);
		UUIFunctionLibrary::ReadyToStartUI();
	}
	else
	{
		GameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_ReadyToStart);
// 		Super::OnReadyToStart();
	}
}

void AMirrorGameMode::OnReadyToResult()
{
	NextGameModeState = EGameModeStateEnum::VE_Result;
}

void AMirrorGameMode::EndGameModeDelay()
{
	Super::EndGameMode();
	WinSequenceEnd = true;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (!pGameMode)
		return;	
	
	GameInstance->EventManager->OnGameMode_ReadyToResult.RemoveDynamic(this, &AMirrorGameMode::OnReadyToResult);
	GameInstance->EventManager->OnGameMode_ReadyToResult.AddDynamic(this, &AMirrorGameMode::OnReadyToResult);

	UMirrorDungeonManager* MirrorManager = GameInstance->MirrorDungeonManager;
	if (!IsValid(MirrorManager))
		return;

	pGameMode->ObjectHelper.SetAllObjectHPBarVisible(false);
	pGameMode->ObjectHelper.SetAllAIStop();
	GameInstance->SequenceManager->AllPause();
	UUtilFunctionLibrary::ViewTargetBaseCamera(GameInstance->GetWorld());



	if (MirrorManager->IsMyWin())
	{
		FGameModeSequenceTableInfo* GameModeSequenceInfo = UTableManager::GetInstancePtr()->GetGameModeSequenceRow(GameModeSequenceKey);
		if (GameModeSequenceInfo)
		{
			if (false == GameModeSequenceInfo->WinSequence.IsNone())
			{
				ABaseSequenceActor* SequenceActor = GameInstance->SequenceManager->SpawnSequence(GameModeSequenceInfo->WinSequence);
				if (IsValid(SequenceActor))
				{
					WinSequenceEnd = false;
					VictoryPositionActor = UUtilFunctionLibrary::SetActorVictoryPosition(GetWorld());

					if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &AMirrorGameMode::OnSequenceEndMirrorWin))
					{
						SequenceActor->OnSequenceEndEvent.AddDynamic(this, &AMirrorGameMode::OnSequenceEndMirrorWin);
					}
				}
			}
		}
	}


	CheckGameModeEndToResult();
}

void AMirrorGameMode::OnCompleteResourcePreLoading()
{
	TArray<FSoftObjectPath> TargetAssets;
	for (size_t i = 0; i < iPartition; ++i)
	{
		if (ObjectHelper.AssetsQueue.IsValidIndex(0))
		{
			FSoftObjectPath target = ObjectHelper.AssetsQueue.Pop();
			TargetAssets.AddUnique(target);
		}
	}
	if (TargetAssets.Num())
	{
		UUP_LoadingScene* LoadingScene = Cast<UUP_LoadingScene>(RGAMEINSTANCE(this)->LoadingSceneManager->Widget);
		if (LoadingScene->IsValidLowLevel())
		{
			LoadingScene->CurrentPercent += LoadingScene->UnitPercent;
			FMath::Clamp<int32>(LoadingScene->CurrentPercent, 0, 100);

			float percent = LoadingScene->CurrentPercent / 100.f;
			LoadingScene->SetProgressBarPercent(percent);
		}
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &AMirrorGameMode::OnCompleteResourcePreLoading));
	}
	else
	{
		Super::OnCompleteResourcePreLoading();
		if (GetWorld())
		{
			SpawnMirrorMatchHeroes();
			GameInstance->MirrorDungeonManager->StopHeroAI();
			GameInstance->MirrorDungeonManager->StopEnemyAI();
		}
		UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
		NextGameModeState = EGameModeStateEnum::VE_Ready;
	}

	
}

void AMirrorGameMode::OnSequenceEndMirrorStart(FName InSequenceKey)
{
	checkf(GameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	GameInstance->EventManager->OnGameMode_ReadyToStart.Broadcast();

	UUtilFunctionLibrary::ViewTargetBaseCamera(GameInstance->GetWorld());
}

void AMirrorGameMode::OnSequenceEndMirrorWin(FName InSequenceKey)
{
	WinSequenceEnd = true;
	CheckGameModeEndToResult();

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (PCM && VictoryPositionActor)
	{
		PCM->SetViewTarget(VictoryPositionActor);
		AActor* CamActor = Cast<AActor>(PCM->ViewTarget.Target);
		UE_LOG(LogClass, Log, TEXT("%s"), *CamActor->GetName());
	}
}

void AMirrorGameMode::SpawnMirrorMatchHeroes()
{
	UBattleRecordManager* BattleRecordMgr = GameInstance->BattleRecordManager;
	GameInstance->MirrorDungeonManager->AttachBattleRecordManager();
	AActor* SpawnedCharacter = nullptr;
	GameInstance->MirrorDungeonManager->mMyHeroList.Empty();
	// MyHero Spawn
	FTransform spawn_pos;
	int32 my_hero_count = GameInstance->MirrorDungeonManager->m_MirrorMyHeroUDs.Num();
	for (int n = 0; n < my_hero_count; ++n)
	{
		GameInstance->MirrorDungeonManager->GetMyHeroSpawnPosition(n, spawn_pos);
		FHERO* Hero = GameInstance->RInventory->GetHeroWithUD(GameInstance->MirrorDungeonManager->m_MirrorMyHeroUDs[n]);
		if (Hero == nullptr)
			continue;
		int32 nRemainHP = GameInstance->MirrorDungeonManager->FindRemainHP(GameInstance->MirrorDungeonManager->m_MirrorMyHeroUDs[n]);
		if (nRemainHP == 0)// dead actor
			continue;

		auto CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(Hero->heroId);

		SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacterForArena(spawn_pos, Hero->heroId, ECharacterCategoryTypeEnum::VE_Hero,
			GameInstance->UserInfo->kId, Hero->heroUD, GroupKey_SINGLE_MY, PartyID_PC_MY, PLAYER_UK_NONE,
			Hero->level, CharacterInfo->Grade, n);

		if (nRemainHP != -1)	// -1 == first battle
		{
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
			if (BattleComp)
				BattleComp->SetHP(nRemainHP);
		}
		BattleRecordMgr->SetHeroBattleInfoDelegate(SpawnedCharacter);
		GameInstance->MirrorDungeonManager->mMyHeroList.Emplace(SpawnedCharacter);
	}
	MyTeamKID = GameInstance->UserInfo->kId;
	BattleRecordMgr->SetUserNickName(MyTeamKID, GameInstance->UserInfo->Avatar.nick, GameInstance->UserInfo->Avatar.nick);

	// 피아식별을 위해서 리더만 세팅한다.(IsEnemy 에서 리더사용)
	if (0 < GameInstance->MirrorDungeonManager->mMyHeroList.Num())
	{
		TArray<FHeroSlotInfo>& rSelectedCharacterArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
		int32 nValidIndex = -1;
		for (int32 i = 0; i < rSelectedCharacterArray.Num(); ++i)
		{
			if (rSelectedCharacterArray.IsValidIndex(i) && rSelectedCharacterArray[i].CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Hero)
			{
				nValidIndex = i;
				break;
			}
		}

		if (0 <= nValidIndex)
			GameInstance->PlayStateInfo->SetSpawnedCharacter(nValidIndex, GameInstance->MirrorDungeonManager->mMyHeroList[0]);
	}
	// MatchTargetHero Spawn
	GameInstance->MirrorDungeonManager->mEnemyList.Empty();
	FBATTLE_USER MatchTarget = GameInstance->MirrorDungeonManager->m_MatchList;
	FEQUIPED_HERO	HeroInfo;
	FMIRROR SelectedSlot = GameInstance->MirrorDungeonManager->GetSelectedMirrorSlotData();

	int32 target_hero_count = MatchTarget.decks.defense.Num();
	for (int n = 0; n < target_hero_count; ++n)
	{
		if (MatchTarget.decks.defense.IsValidIndex(n))
		{
			if (GameInstance->OtherHeroManager->GetOtherHeroData(MatchTarget.kId, MatchTarget.decks.defense[n], HeroInfo) == false)
				continue;
			int32 nRemainHP = -1;
			if (SelectedSlot.hpList.IsValidIndex(n))
			{
				nRemainHP = SelectedSlot.hpList[n];
				if (nRemainHP == 0)
					continue;
			}
			GameInstance->MirrorDungeonManager->GetMatchHeroSpawnPosition(MAX_HERO_POS_COUNT - 1 - n, spawn_pos);

			auto CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInfo.heroId);

			SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacterForArena(spawn_pos, HeroInfo.heroId, ECharacterCategoryTypeEnum::VE_Hero,
				MatchTarget.kId, HeroInfo.heroUD, GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, PLAYER_UK_NONE,
				HeroInfo.level, CharacterInfo->Grade, n + COMMANDSLOT_MAX);

			if (0 < nRemainHP)
			{
				UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
				BattleComp->SetHP(nRemainHP);
			}
			BattleRecordMgr->SetHeroBattleInfoDelegate(SpawnedCharacter);
			GameInstance->MirrorDungeonManager->mEnemyList.Emplace(SpawnedCharacter);
		}
	}
	EnemyTeamKID = MatchTarget.kId;
	BattleRecordMgr->SetUserNickName(EnemyTeamKID, MatchTarget.nick, MatchTarget.guild);
}

void AMirrorGameMode::OnPlayModeMirrorDoneRpDelegate()
{
	ReceiveMirrorPlayDoneRp = true;
	CheckGameModeEndToResult();
}

void AMirrorGameMode::CheckGameModeEndToResult()
{
	if (WinSequenceEnd && ReceiveMirrorPlayDoneRp)
	{
		GameInstance->EventManager->OnGameMode_ReadyToResult.Broadcast();
	}
}
