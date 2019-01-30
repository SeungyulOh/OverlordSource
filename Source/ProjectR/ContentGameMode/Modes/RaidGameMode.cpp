// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RaidGameMode.h"
#include "SharedConstants/GlobalConstants.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "Network/RTSManager.h"
#include "GameInfo/PlayStateInfo.h"
#include "UI/UI_Battle/RCampaignHUD_RaidThreeControl.h"
#include "UI/UI_Battle/RCampaignHUD_RaidOneControl.h"
#include "UI/UP_LoadingScene.h"

#include "ContentGameMode/Controllers/RaidPlayerController.h"
#include "Character/Components/FunctionalComponents/RemoteEntityAIComponent.h"
#include "Object/WallActor.h"

#include "CustomActors/RSceneCapturer.h"

#include "RSkillSet_New.h"

ARaidRGameMode::ARaidRGameMode(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = ARaidPlayerController::StaticClass();
}

void ARaidRGameMode::InitGameMode()
{
	// before Super::InitGameMode
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		RGAMEINSTANCE(this)->CurrentGameMode = EGamePlayModeTypeEnum::VE_Colosseum;

	Super::InitGameMode();

	GameModeState = EGameModeStateEnum::VE_Init;
	GameContextState = EStageContextEnum::VE_None;
	GameInstance->BattleRecordManager->ResetBattleRecordData();
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
	GameInstance->RealTimeModManager->SetGameMode(this);
}

void ARaidRGameMode::PrepareGameMode()
{
	Super::PrepareGameMode();
	GameModeState = EGameModeStateEnum::VE_Prepare;
	GameContextState = EStageContextEnum::VE_None;
	UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	ObjectHelper.SetBlackOut(false);
	if (BaseStageInfo)
	{
		StageTime = BaseStageInfo->GetPlayTime();
		BaseStageInfo->SetTimer(StageTime);
		BaseStageInfo->PauseTimer();
		
		if (URTSManager::GetInstancePtr()->IsConnected())
		{
			//check URTSManager::IsConnectedMode
			EPVP RTSManagerCurrentMode = URTSManager::GetInstancePtr()->getConnectedMode();
			switch (RTSManagerCurrentMode)
			{
				case EPVP::MR:
				{
					BaseStageInfo->bPlayAlone = true;
				}
				break;
				case EPVP::SR:
				case EPVP::CS:
				{
					BaseStageInfo->bPlayAlone = false;
				}
				break;
			}
		}

		/*Resource Cache!*/
		ObjectHelper.AssetsQueue.Empty();

		//PC 
		TMap<int16, FRTS_ACTOR> ActorList = RGAMEINSTANCE(this)->RealTimeModManager->ActorList;
		for (auto& Element : ActorList)
			ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, Element.Value.actorId);

		//NPC
		TArray<FName> NPCList;
		URTSManager::GetInstancePtr()->NPC_CACHE_LIST(BaseStageInfo->StageID, NPCList);
		for (size_t i = 0; i < NPCList.Num(); ++i)
			ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, NPCList[i], true);

		//Tables
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
			URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &ARaidRGameMode::OnCompleteResourcePreLoading));
	}
	
}

void ARaidRGameMode::ReadyGameMode()
{
	Super::ReadyGameMode();
	GameInstance->PlayStateInfo->MakeRealTimeHeroSlot();
	
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
	GameModeState = EGameModeStateEnum::VE_Ready;
	GameContextState = EStageContextEnum::VE_None;
	UUtilFunctionLibrary::ViewTargetBaseCamera(RGAMEINSTANCE(this)->GetWorld());
	ObjectHelper.SetAllObjectHPBarVisible(false);
	UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->PauseTimer();
}

void ARaidRGameMode::StartGameMode()
{
	Super::StartGameMode();

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (PCM)
	{
		FTransform transform;

		UCameraComponent* CamComp = PCM->ViewTarget.Target->FindComponentByClass<UCameraComponent>();
		float FOV = 90.f;
		if (CamComp)
		{
			FOV = CamComp->FieldOfView;
		}

		transform.SetRotation(PCM->GetCameraRotation().Quaternion());
		transform.SetLocation(PCM->GetCameraLocation());

		SceneCapturer = GetWorld()->SpawnActor<ARSceneCapturer>(ARSceneCapturer::StaticClass(), transform);
		SceneCapturer->SetFOV(FOV);
	}

	UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	UUtilFunctionLibrary::ViewTargetBaseCamera(GetWorld());
	ObjectHelper.SetAllObjectVisible(true);
	ObjectHelper.SetAllObjectHPBarVisible(true);

	UUIFunctionLibrary::AttachHUDPage(GetWorld());
	UUIFunctionLibrary::OnShowInGameHUD(GetWorld(), true);
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), true);
	ARaidPlayerController *BasePlayerController = Cast<ARaidPlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GetWorld()));

	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) || URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
	{
		UHUD_RaidThreeControl *RaidHUD = Cast<UHUD_RaidThreeControl>(UUIFunctionLibrary::GetInGameHUD());

		if (BasePlayerController && RaidHUD)
		{
			FHeroSlotInfo &LeaderSlot = GameInstance->PlayStateInfo->GetSelectedCharacterSlot(dMAIN_SLOT_HERO);
			BasePlayerController->ActorSettingOnTheCamera(LeaderSlot.SpawnedTargetActor, RaidHUD->CharacterFocusCameraTime, RaidHUD->CharacterFocusCameraExp);
			BaseStageInfo->bPlayAlone = false;
			
			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
			{
				UUIFunctionLibrary::ReadyToStartUI();

				TArray<AActor*> Actors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
				for (auto& Element : Actors)
				{
					ACharacter* character = Cast<ACharacter>(Element);
					if (character)
						character->GetCharacterMovement()->GravityScale = 9.8f;

					if (Element->GetName().Contains(TEXT("Metal")))
						Element->SetActorHiddenInGame(false);
				}

				RaidHUD->HPBarSwitcher->SetActiveWidgetIndex(3);
			}
			else
			{
				RaidHUD->HPBarSwitcher->SetVisibility(ESlateVisibility::Collapsed);
			}
				
		}
	}
	else if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
	{
		URCampaignHUD_RaidOneControl *RaidHUD = Cast<URCampaignHUD_RaidOneControl>(UUIFunctionLibrary::GetInGameHUD());
		if (BasePlayerController && RaidHUD)
		{
			FHeroSlotInfo &LeaderSlot = GameInstance->PlayStateInfo->GetSelectedCharacterSlot(dMAIN_SLOT_HERO);
			BasePlayerController->ActorSettingOnTheCamera(LeaderSlot.SpawnedTargetActor, RaidHUD->CharacterFocusCameraTime, RaidHUD->CharacterFocusCameraExp);
			BaseStageInfo->bPlayAlone = true;
		}
	}
	else
	{
		checkf(false, TEXT("check %s"), __FUNCTION__);
	}

	BaseStageInfo->UnPauseTimer();
}

void ARaidRGameMode::EndGameMode()
{
	Super::EndGameMode();
}

void ARaidRGameMode::ResultGameMode()
{
	Super::ResultGameMode();
	UHUD_RaidThreeControl *RaidHUD = Cast<UHUD_RaidThreeControl>(UUIFunctionLibrary::GetInGameHUD());
	if (RaidHUD)
	{
		RaidHUD->SetVisibility(ESlateVisibility::Collapsed);
	}
	GameModeState = EGameModeStateEnum::VE_Result;
	USoundManager::GetInstancePtr()->FadeOutBGM();
}

void ARaidRGameMode::StopGameMode()
{
	Super::StopGameMode();
}

void ARaidRGameMode::GamePlayModeEnd()
{
	Super::GamePlayModeEnd();
	GameInstance->RealTimeModManager->ClearData();
	URTSManager::GetInstancePtr()->REQ_BT_EXIT();
}

void ARaidRGameMode::ResultUI()
{
}

EStageContextEnum ARaidRGameMode::GetStageContext()
{
	return EStageContextEnum::VE_Progress;
}

void ARaidRGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ARaidRGameMode::OnCompleteResourcePreLoading()
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
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &ARaidRGameMode::OnCompleteResourcePreLoading));
	}
	else
	{
		Super::OnCompleteResourcePreLoading();
		GameInstance->RealTimeModManager->OnMapLoadingComplete();
	}
}

void ARaidRGameMode::OnACK_BT_ACTOR_SPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_SPAWNT *Packet)
{
	for (auto &update : Packet->updates)
	{
		auto update_info = update.get();
		FRTS_ACTOR *actor = GameInstance->RealTimeModManager->Get_RTS_Actor(update_info->actorUK);
		if (actor == nullptr)
			continue;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(update_info->location->X, update_info->location->Y, update_info->location->Z));
		FRotator Rot = FVector(update_info->direction->X, update_info->direction->Y, update_info->direction->Z).Rotation();
		SpawnTransform.SetRotation(Rot.Quaternion());

		bool bRemote = true;

		if (GameInstance->HttpClient->IsValidKID(actor->kId))
			bRemote = false;
		int32 CommandSlotIndex = actor->slot - 1;
		AActor *SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacterWithRTS(SpawnTransform, actor->actorId, (ECharacterCategoryTypeEnum)actor->actorCategory,
			actor->kId, actor->heroUD, actor->team, actor->party, actor->actorUK, actor->level, actor->grade, true, CommandSlotIndex, bRemote, true);

		FString CategoryType = UUtilFunctionLibrary::GetEnumToString(TEXT("ECharacterCategoryTypeEnum"), actor->actorCategory);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_SPAWN ECharacterCategoryTypeEnum[%s]"), *CategoryType);

		if (!IsValid(SpawnedCharacter))
			return;

		GameInstance->BattleRecordManager->SetHeroBattleInfoDelegate(SpawnedCharacter);

		if (actor->isSummon)
		{
			TWeakObjectPtr<AActor> PrimaryActor = ObjectHelper.GetObjectByPlayerUK(actor->primaryUK);
			if (PrimaryActor.IsValid())
			{
				UBattleEntityComponent *PrimaryBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(PrimaryActor.Get());
				UBattleEntityComponent *SummonBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
				if (IsValid(PrimaryBattleEntityComponent) && IsValid(SummonBattleEntityComponent))
				{
					SummonBattleEntityComponent->SetSummonOwner(PrimaryActor.Get());
					SummonBattleEntityComponent->SetCurrentAttackTarget(PrimaryBattleEntityComponent->GetCurrentAttackTarget());
					PrimaryBattleEntityComponent->GetBattleInfo()->AddSummonedMinion(SpawnedCharacter);

					SummonBattleEntityComponent->GetBattleInfo()->CharacterProperty.MaxHP = actor->maxHp;
					SummonBattleEntityComponent->GetBattleInfo()->SetHP(actor->maxHp);
				}
			}
		}

		FRTS_PLAYER *OwnerPlayer = GameInstance->RealTimeModManager->Get_RTS_Player(actor->kId);
		if (OwnerPlayer)
			GameInstance->BattleRecordManager->SetUserNickName(OwnerPlayer->kId, OwnerPlayer->nick, actor->subNick);
		if (GameInstance->HttpClient->IsValidKID(actor->kId))
		{
			IEntityBaseProperty *BaseProp = Cast<IEntityBaseProperty>(SpawnedCharacter);
			MyGroupKey = BaseProp->GetGroupKey();

			GameInstance->PlayStateInfo->SetSpawnedCharacter(CommandSlotIndex, SpawnedCharacter);

			if (CommandSlotIndex == ROLE_LEADER_INDEX)
			{
				UWorld *GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
				ARaidPlayerController *BasePlayerController = Cast<ARaidPlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameWorld));
				BasePlayerController->SetSelectedCharacter(SpawnedCharacter);
				BasePlayerController->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);
				BasePlayerController->ActorSettingOnTheCamera(SpawnedCharacter, 0.1f, 0.1f);
			}
		}
		else
		{
			IEntityBaseProperty *BaseProp = Cast<IEntityBaseProperty>(SpawnedCharacter);
			OtherGroupKey = BaseProp->GetGroupKey();

			if (BaseProp->GetGroupKey() != UObjectFunctionLibrary::GetMyTeamKey() &&
				BaseProp->GetPartyID() != UObjectFunctionLibrary::GetMyPartyID())
			{
				UBaseStageInfo *BaseInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
				if (BaseInfo)
				{
					if (actor->role == (uint8)ECharacterRoleEnum::VE_Boss)
					{
						URBaseIngameHUD* ingamehud = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());
						if (IsValid(ingamehud))
						{
							ingamehud->HPBarSwitcher->SetActiveWidgetIndex(2);
							ingamehud->HPBarSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						}
						BaseInfo->ArrBossInfo.Emplace(SpawnedCharacter);
						RGAMEINSTANCE(this)->EventManager->OnMultiBossSpawn.Broadcast({ SpawnedCharacter });
					}
					else if (actor->role != (uint8)ECharacterRoleEnum::VE_Neutral)
						BaseInfo->ArrMonsterInfo.Emplace(SpawnedCharacter);
				}
			}
		}
	}

	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		AttachBattleRecordManager();

}

void ARaidRGameMode::AttachBattleRecordManager()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->BattleRecordManager != nullptr, TEXT("RGameInstance->BattleRecordManager is Empty"));

// 	RGameInstance->BattleRecordManager->OnBattleRecord_UpdateHP.RemoveDynamic(this, &ARaidRGameMode::OnBattleRecord_UpdateHP);
// 	RGameInstance->BattleRecordManager->OnBattleRecord_UpdateHP.AddDynamic(this, &ARaidRGameMode::OnBattleRecord_UpdateHP);
}

void ARaidRGameMode::OnBattleRecord_UpdateHP()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->BattleRecordManager->UpdateforRTSActorHP(MyGroupKey);
	RGameInstance->BattleRecordManager->UpdateforRTSActorHP(OtherGroupKey);

// 	m_nMyHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(MyGroupKey);
// 	m_nTargetHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(OtherGroupKey);
	m_nMyHeroesCurrentHP = 0;
	m_nTargetHeroesCurrentHP = 0;

	for (TPair<int16, FRTS_ACTOR> Actor : RGameInstance->RealTimeModManager->ActorList)
	{
		FRTS_ACTOR actor = Actor.Value;
		if (actor.kId == RGameInstance->HttpClient->GetKID())
		{
			m_nMyHeroesCurrentHP += actor.curHp;
		}
		else
		{
			m_nTargetHeroesCurrentHP += actor.curHp;
		}

	}

	UHUD_RaidThreeControl *RaidHUD = Cast<UHUD_RaidThreeControl>(UUIFunctionLibrary::GetInGameHUD());
	if (RaidHUD)
	{
		RaidHUD->UpdateMyTotalHP(m_nMyHeroesCurrentHP, m_nMyHeroesMaxHP);
		RaidHUD->UpdateEnemyTotalHP(m_nTargetHeroesCurrentHP, m_nTargetHeroesMaxHP);
	}
}

void ARaidRGameMode::OnACK_BT_CHANGE_CREW(FRTS_ACTOR_UPDATE *CrewInfo)
{
	FRTS_ACTOR *actor = GameInstance->RealTimeModManager->Get_RTS_Actor(CrewInfo->actorUK);
	if (actor == nullptr)
		return;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(CrewInfo->location.X, CrewInfo->location.Y, CrewInfo->location.Z));
	SpawnTransform.SetRotation(FVector(CrewInfo->direction.X, CrewInfo->direction.Y, CrewInfo->direction.Z).Rotation().Quaternion());

	bool bRemote = true;

	if (GameInstance->HttpClient->IsValidKID(actor->kId))
		bRemote = false;
	int32 CommandSlotIndex = actor->slot - 1;
	AActor *SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacterWithRTS(SpawnTransform, actor->actorId, (ECharacterCategoryTypeEnum)actor->actorCategory,
																			 actor->kId, actor->heroUD, actor->team, actor->party, actor->actorUK, actor->level, actor->grade, true, CommandSlotIndex, bRemote, true);

	FString CategoryType = UUtilFunctionLibrary::GetEnumToString(TEXT("ECharacterCategoryTypeEnum"), actor->actorCategory);
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_CHANGE_CREW ECharacterCategoryTypeEnum[%s]"), *CategoryType);

	if (!IsValid(SpawnedCharacter))
		return;
	UBattleRecordManager* BattleRecordMgr = GameInstance->BattleRecordManager;
	BattleRecordMgr->SetHeroBattleInfoDelegate(SpawnedCharacter);

	if (actor->isSummon)
	{
		TWeakObjectPtr<AActor> PrimaryActor = ObjectHelper.GetObjectByPlayerUK(actor->primaryUK);
		if (PrimaryActor.IsValid())
		{
			UBattleEntityComponent *PrimaryBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(PrimaryActor.Get());
			UBattleEntityComponent *SummonBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
			if (IsValid(PrimaryBattleEntityComponent) && IsValid(SummonBattleEntityComponent))
			{
				SummonBattleEntityComponent->SetSummonOwner(PrimaryActor.Get());
				SummonBattleEntityComponent->SetCurrentAttackTarget(PrimaryBattleEntityComponent->GetCurrentAttackTarget());
				PrimaryBattleEntityComponent->GetBattleInfo()->AddSummonedMinion(SpawnedCharacter);
			}
		}
	}

	FRTS_PLAYER *OwnerPlayer = GameInstance->RealTimeModManager->Get_RTS_Player(actor->kId);
	if (OwnerPlayer)
		GameInstance->BattleRecordManager->SetUserNickName(OwnerPlayer->kId, OwnerPlayer->nick, actor->subNick);
	if (GameInstance->HttpClient->IsValidKID(actor->kId))
	{
		GameInstance->PlayStateInfo->SetSpawnedCharacter(CommandSlotIndex, SpawnedCharacter);
	}
}

void ARaidRGameMode::OnReadyToStart()
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

void ARaidRGameMode::OnACK_BT_DOOR_EVENT(uint16_t doorId, uint8 event)
{
	UWorld *InWorld = GEngine->GetWorldFromContextObject(RGAMEINSTANCE(this), EGetWorldErrorMode::Assert);
	if (InWorld == nullptr)
		return;
	for (TActorIterator<AWallActor> ActorItr(InWorld); ActorItr; ++ActorItr)
	{
		AWallActor *WallActor = Cast<AWallActor>(*ActorItr);
		if (WallActor->ActorID == doorId)
		{
			if (RTSZoneEvent::RTSZoneEvent_DoorClose == (RTSZoneEvent)event)
			{
				WallActor->SetActorHiddenInGame(false);
				WallActor->SetActorEnableCollision(true);
			}

			if (RTSZoneEvent::RTSZoneEvent_DoorOpen == (RTSZoneEvent)event)
			{
				WallActor->SetActorHiddenInGame(true);
				WallActor->SetActorEnableCollision(false);
			}
		}
	}
}
