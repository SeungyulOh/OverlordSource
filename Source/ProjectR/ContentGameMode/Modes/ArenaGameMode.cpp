// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "ArenaGameMode.h"
// #include "Global/RGameInstance.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
// #include "Utils/ObjectFunctionLibrary.h"
// #include "Utils/UIFunctionLibrary.h"
// #include "Utils/GamePlayBluePrintFunction.h"
#include "ContentGameMode/Controllers/ArenaPlayerController.h"

#include "UI/UP_LoadingScene.h"



AArenaRGameMode::AArenaRGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = AArenaPlayerController::StaticClass();
}

void AArenaRGameMode::InitGameMode()
{
	Super::InitGameMode();

	GameModeState = EGameModeStateEnum::VE_Init;
	GameContextState = EStageContextEnum::VE_None;
	ShowUI_PVP_VS = false;

	ObjectHelper.RemoveObjectAll();

	GameInstance->BattleRecordManager->ResetBattleRecordData();

	UObjectFunctionLibrary::SetMyTeamKey(GroupKey_SINGLE_MY);
	UObjectFunctionLibrary::SetMyPartyID(PartyID_PC_MY);

	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
}

void AArenaRGameMode::PrepareGameMode()
{
	Super::PrepareGameMode();

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->bPlayAlone = false;
	GameModeState = EGameModeStateEnum::VE_Prepare;
	 
	GameInstance->PlayStateInfo->ReadyToPlay();
	
	if (GameInstance->GetCurrentStateType() == EGameStateEnum::VE_Play)
		USoundManager::GetInstancePtr()->PlayBattleBGMusic(BaseStageInfo->MapTableKey);
	

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (!pGameMode)
		return;
	ObjectHelper.SetBlackOut(false);

	StageTime = BaseStageInfo->GetPlayTime();
	BaseStageInfo->SetTimer(StageTime);
	BaseStageInfo->PauseTimer();

	/*Cache Resource*/
	ObjectHelper.AssetsQueue.Empty();
	int32 my_hero_count = GameInstance->ArenaManager->m_ArenaMyHeroUDs.Num();
	for (int n = 0; n < my_hero_count; ++n)
	{
		FHERO* Hero = GameInstance->RInventory->GetHeroWithUD(GameInstance->ArenaManager->m_ArenaMyHeroUDs[n]);
		if (Hero == nullptr)
			continue;

		ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, Hero->heroId);
	}

	FBATTLE_USER MatchTarget = GameInstance->ArenaManager->m_MatchList;

	int32 target_hero_count = MatchTarget.decks.defense.Num();
	
	for (int32 n = 0; n < target_hero_count; ++n)
	{
		if(MatchTarget.decks.defense.IsValidIndex(n) && !MatchTarget.decks.defense[n].IsEmpty())
		{
			FEQUIPED_HERO* FoundHero = MatchTarget.heros.FindByPredicate([&](const FEQUIPED_HERO& InHero) { 
				return MatchTarget.decks.defense[n] == InHero.heroUD; });

			ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, FoundHero->heroId);
		}
	}
	ObjectHelper.SetAssetResource_FromTables(ObjectHelper.AssetsQueue);

	GameInstance->ArenaManager->SetArenaHeroPosition();

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
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &AArenaRGameMode::OnCompleteResourcePreLoading));

}

void AArenaRGameMode::ReadyGameMode()
{
	Super::ReadyGameMode();

	GameModeState = EGameModeStateEnum::VE_Ready;

	UUIFunctionLibrary::AttachHUDPage(GetWorld());
	UUIFunctionLibrary::OnShowInGameHUD(GetWorld(), false);

	//if (IsValid(GameInstance))
	{
		UArenaManager* ArenaMgr = GameInstance->ArenaManager;
		if (IsValid(ArenaMgr))
			ArenaMgr->InitArenaGuage();

		GameInstance->EventManager->OnGameMode_ReadyToStart.RemoveDynamic(this, &AArenaRGameMode::OnReadyToStart);
		GameInstance->EventManager->OnGameMode_ReadyToStart.AddDynamic(this, &AArenaRGameMode::OnReadyToStart);

		FGameModeSequenceTableInfo* GameModeSequenceInfo = UTableManager::GetInstancePtr()->GetGameModeSequenceRow(GameModeSequenceKey);
		if (GameModeSequenceInfo)
		{
			if (false == GameModeSequenceInfo->StartSequence.IsNone())
			{
				ABaseSequenceActor* SequenceActor = GameInstance->SequenceManager->SpawnSequence(GameModeSequenceInfo->StartSequence);
				if (IsValid(SequenceActor))
				{
					if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &AArenaRGameMode::OnSequenceEndArenaStart))
						SequenceActor->OnSequenceEndEvent.AddDynamic(this, &AArenaRGameMode::OnSequenceEndArenaStart);
				}
			}
		}
	}

	if (IsValid(GameInstance->RWidgetManager))
		GameInstance->RWidgetManager->ChangeInputModeUIOnly(false);

	UUserWidget* HUDWidget = UUIFunctionLibrary::GetInGameHUD();
	if (IsValid(HUDWidget))
	{
		UUP_InGameHUD_Arena* InGameHUD_Arena = Cast<UUP_InGameHUD_Arena>(HUDWidget);
		if (InGameHUD_Arena)
			InGameHUD_Arena->Init();
	}
}

void AArenaRGameMode::StartGameMode()
{
	Super::StartGameMode();

	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
	UUIFunctionLibrary::OnShowInGameHUD(GetWorld(), true);

	GameModeState = EGameModeStateEnum::VE_Start;
	GameContextState = EStageContextEnum::VE_Progress;

	//if (IsValid(GameInstance))
	{
		GameInstance->EventManager->OnGameMode_ReadyToStart.RemoveDynamic(this, &AArenaRGameMode::OnReadyToStart);
		GameInstance->ArenaManager->SetCameraModeAfterGameStart(EArenaCameraMode::VE_Sequence_1);
	}

	GameInstance->TimerManager->SetTimer(StartGameModeDelayHandler, this, &AArenaRGameMode::TimeUnPause, 2.8f, false);
}

void AArenaRGameMode::EndGameMode()
{
	Super::EndGameMode();
	GameModeState = EGameModeStateEnum::VE_End;

	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	UUIFunctionLibrary::AttachHUDPage(GetWorld(), false);
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);

	GameInstance->PlayStateInfo->SetClearTime(BaseStageInfo->GetElapsedGameTime());
	GameInstance->PlayStateInfo->SetRemainingTime(BaseStageInfo->GetRemainingGameTime());
	BaseStageInfo->PauseTimer();
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (!pGameMode)
		return;
	
	pGameMode->ObjectHelper.SetAllAIStop();

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
	ReceiveArenaPlayDoneRp = false;

	if (IsValid(GameInstance->ArenaManager))
	{
		GameInstance->ArenaManager->ResetCamera();
		GameInstance->ArenaManager->RequestPlayDone();
	}
	GameInstance->ArenaManager->StopEnemyAI();
	GameInstance->ArenaManager->StopHeroAI();

	GameInstance->TimerManager->SetTimer(EndGameModeDelayHandler, this, &AArenaRGameMode::EndGameModeDelay, 2.0f, false);


}

void AArenaRGameMode::ResultGameMode()
{
	Super::ResultGameMode();
	GameModeState = EGameModeStateEnum::VE_Result;
	USoundManager::GetInstancePtr()->FadeOutBGM();
	GameInstance->EventManager->OnGameMode_ReadyToResult.RemoveDynamic(this, &AArenaRGameMode::OnReadyToResult);
}

void AArenaRGameMode::GamePlayModeEnd()
{
	Super::GamePlayModeEnd();
	//	ArenaHeroesPropertyList.Empty();
	if (IsValid(GameInstance->PlayModeManager))
	{
		GameInstance->PlayModeManager->PlayModeArenaEnd();
	}
}

EStageContextEnum AArenaRGameMode::GetStageContext()
{
	if (!IsValid(GameInstance->ArenaManager))
		return EStageContextEnum::VE_Fail;

	//if Result is already Update then Not Check Win Deck
	if (GameContextState == EStageContextEnum::VE_Fail)
	{
		GameInstance->ArenaManager->SetMyWin(false);
		return GameContextState;
	}
	else if (GameContextState == EStageContextEnum::VE_Success)
	{
		GameInstance->ArenaManager->SetMyWin(true);
		return GameContextState;
	}

	if (bIsTimeOut)
	{
		GameContextState = EStageContextEnum::VE_Fail;
		GameInstance->ArenaManager->DecideWinner();
	}
	else
	{
		if (GameInstance->ArenaManager->IsCombatEnd())
		{
			if (GameInstance->ArenaManager->GetAliveMyHero() > GameInstance->ArenaManager->GetAliveEnemy())
			{
				GameContextState = EStageContextEnum::VE_Success;
				GameInstance->ArenaManager->SetMyWin(true);
				return GameContextState;
			}
			else
			{
				GameContextState = EStageContextEnum::VE_Fail;
				GameInstance->ArenaManager->SetMyWin(false);
				return GameContextState;
			}
		}
	}

	return GameContextState;
}

void AArenaRGameMode::CheckGameModeEndToResult()
{
	if (WinSequenceEnd && ReceiveArenaPlayDoneRp)
		GameInstance->EventManager->OnGameMode_ReadyToResult.Broadcast();
}

void AArenaRGameMode::SpawnArenaMatchHeroes()
{
	UBattleRecordManager* BattleRecordMgr = GameInstance->BattleRecordManager;
	GameInstance->ArenaManager->AttachBattleRecordManager();
	AActor* SpawnedCharacter = nullptr;
	GameInstance->ArenaManager->mMyHeroList.Empty();
	// MyHero Spawn
	FTransform spawn_pos;
	int32 my_hero_count = GameInstance->ArenaManager->m_ArenaMyHeroUDs.Num();
	for (int n = 0; n < my_hero_count; ++n)
	{
		GameInstance->ArenaManager->GetMyHeroSpawnPosition(n, spawn_pos);
		FHERO* Hero = GameInstance->RInventory->GetHeroWithUD(GameInstance->ArenaManager->m_ArenaMyHeroUDs[n]);
		if (Hero == nullptr)
			continue;
		auto CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(Hero->heroId);

		SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacterForArena(spawn_pos, Hero->heroId, ECharacterCategoryTypeEnum::VE_Hero,
			GameInstance->UserInfo->kId, Hero->heroUD, GroupKey_SINGLE_MY, PartyID_PC_MY, PLAYER_UK_NONE,
			Hero->level, CharacterInfo->Grade, n);

		BattleRecordMgr->SetHeroBattleInfoDelegate(SpawnedCharacter);
		GameInstance->ArenaManager->mMyHeroList.Emplace(SpawnedCharacter);
	}
	MyTeamKID = GameInstance->UserInfo->kId;
	BattleRecordMgr->SetUserNickName(MyTeamKID, GameInstance->UserInfo->Avatar.nick, GameInstance->UserInfo->Avatar.nick);
	
	// 피아식별을 위해서 리더만 세팅한다.(IsEnemy 에서 리더사용)
	if(0 < GameInstance->ArenaManager->mMyHeroList.Num())
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

		if(0 <= nValidIndex)
			GameInstance->PlayStateInfo->SetSpawnedCharacter(nValidIndex, GameInstance->ArenaManager->mMyHeroList[0]);
	}
	
	// MatchTargetHero Spawn
	GameInstance->ArenaManager->mEnemyList.Empty();
	FBATTLE_USER MatchTarget = GameInstance->ArenaManager->m_MatchList;
	int32 target_hero_count = MatchTarget.decks.defense.Num();
	FEQUIPED_HERO	HeroInfo;

	for (int n = 0; n < target_hero_count; ++n)
	{
		if (MatchTarget.decks.defense.IsValidIndex(n))
		{
			if (GameInstance->OtherHeroManager->GetOtherHeroData(MatchTarget.kId, MatchTarget.decks.defense[n], HeroInfo) == false)
				continue;

			GameInstance->ArenaManager->GetMatchHeroSpawnPosition(n, spawn_pos);

			auto CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInfo.heroId);

			SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacterForArena(spawn_pos, HeroInfo.heroId, ECharacterCategoryTypeEnum::VE_Hero,
				MatchTarget.kId, HeroInfo.heroUD, GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, PLAYER_UK_NONE,
				HeroInfo.level, CharacterInfo->Grade, n + COMMANDSLOT_MAX);

			BattleRecordMgr->SetHeroBattleInfoDelegate(SpawnedCharacter);
			GameInstance->ArenaManager->mEnemyList.Emplace(SpawnedCharacter);
		}

	}
	EnemyTeamKID = MatchTarget.kId;
	BattleRecordMgr->SetUserNickName(EnemyTeamKID, MatchTarget.nick, MatchTarget.guild);
}

void AArenaRGameMode::OnSequenceEndArenaWin(FName InSequenceKey)
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

void AArenaRGameMode::OnSequenceEndArenaStart(FName InSequenceKey)
{
	checkf(GameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	GameInstance->EventManager->OnGameMode_ReadyToStart.Broadcast();

	UUtilFunctionLibrary::ViewTargetBaseCamera(GameInstance->GetWorld());
	GameInstance->ArenaManager->SetCameraModeAfterGameStart(EArenaCameraMode::VE_Default);
}

void AArenaRGameMode::OnPlayModeArenaDoneRpDelegate()
{
	ReceiveArenaPlayDoneRp = true;
	CheckGameModeEndToResult();
}

void AArenaRGameMode::EndGameModeDelay()
{
	Super::EndGameMode();
	WinSequenceEnd = true;

	GameInstance->EventManager->OnGameMode_ReadyToResult.RemoveDynamic(this, &AArenaRGameMode::OnReadyToResult);
	GameInstance->EventManager->OnGameMode_ReadyToResult.AddDynamic(this, &AArenaRGameMode::OnReadyToResult);
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (!pGameMode)
		return;

	UArenaManager* ArenaMgr = GameInstance->ArenaManager;
	if (false == IsValid(ArenaMgr))
		return;
	pGameMode->ObjectHelper.SetAllObjectHPBarVisible(false);
	GameInstance->SequenceManager->AllPause();
	UUtilFunctionLibrary::ViewTargetBaseCamera(GetWorld());

	if (ArenaMgr->IsMyWin())
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
					
					if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &AArenaRGameMode::OnSequenceEndArenaWin))
					{
						SequenceActor->OnSequenceEndEvent.AddDynamic(this, &AArenaRGameMode::OnSequenceEndArenaWin);
					}
				}
			}
		}
	}

	CheckGameModeEndToResult();
}

void AArenaRGameMode::OnCompleteResourcePreLoading()
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
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &AArenaRGameMode::OnCompleteResourcePreLoading));
	}
	else
	{
		Super::OnCompleteResourcePreLoading();

		if (GetWorld())
		{
			SpawnArenaMatchHeroes();
			GameInstance->ArenaManager->StopHeroAI();
			GameInstance->ArenaManager->StopEnemyAI();
		}

		UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
		NextGameModeState = EGameModeStateEnum::VE_Ready;
	}
	
}

void AArenaRGameMode::OnReadyToStart()
{
	NextGameModeState = EGameModeStateEnum::VE_Start;
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
		NextGameModeState = EGameModeStateEnum::VE_Start;
	}
}

void AArenaRGameMode::OnReadyToResult()
{
	NextGameModeState = EGameModeStateEnum::VE_Result;
}

void AArenaRGameMode::TimeUnPause()
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (BaseStageInfo)
		BaseStageInfo->UnPauseTimer();
}
