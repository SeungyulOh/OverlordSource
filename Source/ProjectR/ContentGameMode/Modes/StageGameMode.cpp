// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "StageGameMode.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Global/RGameInstance.h"
#include "Global/TableManager.h"
#include "Global/BattleRecordManager.h"
#include "UtilFunctionIntegrated.h"
#include "CustomActors/DropItemActor.h"
#include "CustomActors/RSceneCapturer.h"

#include "UI/UP_LoadingScene.h"




AStageGameMode::AStageGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	PlayerControllerClass = AStagePlayerController::StaticClass();
}

void AStageGameMode::BeginPlay()
{
	Super::BeginPlay();

	UObject* FoundObject = StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("Material'/Game/UMG/Materials/Mat_CustomDepth_PS.Mat_CustomDepth_PS'"));
	if (Cast<UMaterial>(FoundObject))
	{
		MID = UMaterialInstanceDynamic::Create(Cast<UMaterial>(FoundObject), nullptr);
	}
}

void AStageGameMode::InitGameMode()
{
	Super::InitGameMode();


	GameModeState = EGameModeStateEnum::VE_Init;
	GameContextState = EStageContextEnum::VE_None;
	SaveStarGrade = EPlayStageStarEnum::VE_None;

	GameInstance->BattleRecordManager->ResetBattleRecordData();
	UObjectFunctionLibrary::SetMyTeamKey(GroupKey_SINGLE_MY);
	UObjectFunctionLibrary::SetMyPartyID(PartyID_PC_MY);
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
}

void AStageGameMode::PrepareGameMode()
{
	Super::PrepareGameMode();

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	
	DropRewards = BaseStageInfo->DropRewards;

	DropItemOrderCheck();

	GameInstance->EventManager->OnDeadActor.RemoveDynamic(this, &AStageGameMode::CheckItemDrop);
	GameInstance->EventManager->OnDeadActor.AddDynamic(this, &AStageGameMode::CheckItemDrop);

	GameModeState = EGameModeStateEnum::VE_Prepare;

	GameInstance->PlayStateInfo->ReadyToPlay();
	
	if (GameInstance->GetCurrentStateType() == EGameStateEnum::VE_Play)
		USoundManager::GetInstancePtr()->PlayBattleBGMusic(BaseStageInfo->MapTableKey);
	
	ObjectHelper.SetBlackOut(false);


	/*Cache Resource*/
	ObjectHelper.AssetsQueue.Empty();
	TArray<FHeroSlotInfo> HeroSlots = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (FHeroSlotInfo& charInfo : HeroSlots)
		ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, charInfo.HeroIDName);

	FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
	for (FName Monster : FloorStageTable->MonsterIDList)
		ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, Monster, true);



	if(!FloorStageTable->Friend1.IsNone())
		ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, FloorStageTable->Friend1, true);
	if (!FloorStageTable->Friend2.IsNone())
		ObjectHelper.SetAssetResource(ObjectHelper.AssetsQueue, FloorStageTable->Friend2, true);

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
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &AStageGameMode::OnCompleteResourcePreLoading));

	if (IsValid(GameInstance->EventManager))
	{
		GameInstance->EventManager->OnCampaignResultEvent.RemoveDynamic(this, &AStageGameMode::OnCampResultEvent);
		GameInstance->EventManager->OnCampaignResultEvent.AddDynamic(this, &AStageGameMode::OnCampResultEvent);
	}
}

void AStageGameMode::DropItemOrderCheck()
{
	int32 BestItemIndex = -1;

	for (int32 i = 0; i < DropRewards.Num(); ++i)// 등급 높은 아이템을 리스트 끝으로
	{
		FREWARD& reward = DropRewards[i];
		if (reward.type == EREWARD_TYPE::ITEM)
		{
			if (BestItemIndex != -1)
			{
				FItemTableInfo*		lhs = UTableManager::GetInstancePtr()->GetCharacterItemRow(DropRewards[BestItemIndex].rewardId);
				FItemTableInfo*		rhs = UTableManager::GetInstancePtr()->GetCharacterItemRow(reward.rewardId);

				if (lhs->ItemGrade < rhs->ItemGrade)
				{
					BestItemIndex = i;
				}
			}
			else
			{
				BestItemIndex = i;
			}
		}
	}

	if (BestItemIndex != -1)
	{
		DropRewards.Swap(BestItemIndex, DropRewards.Num() - 1);
	}
}

void AStageGameMode::ReadyGameMode()
{
	Super::ReadyGameMode();

#if LOG_TIME
	double StartTime1 = FPlatformTime::Seconds();
#endif
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	GameModeState = EGameModeStateEnum::VE_Ready;

	

	StageTime = BaseStageInfo->GetPlayTime();

	BaseStageInfo->SetTimer(StageTime);
	BaseStageInfo->PauseTimer();

	GameInstance->EventManager->OnGameMode_ReadyToStart.RemoveDynamic(this, &AStageGameMode::OnReadyToStart);
	GameInstance->EventManager->OnGameMode_ReadyToStart.AddDynamic(this, &AStageGameMode::OnReadyToStart);

	
	
	bool bExe = UGamePlayBluePrintFunction::IsShowDialog();

	if (!bExe)
		ShowReadyToStartUI();
	else
		GameInstance->EventManager->OnGameMode_Ready.Broadcast();
	

	if (IsValid(GameInstance->RWidgetManager))
		GameInstance->RWidgetManager->ChangeInputModeUIOnly(false);

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime1;

	UE_LOG(LogGamePlay, Log, TEXT(" ____basecamp ReadyGameMode(%f)"), PrintTime);
#endif
}

void AStageGameMode::StartGameMode()
{
	Super::StartGameMode();

	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (UGamePlayBluePrintFunction::IsShowDialog() == false)
	{
		GameInstance->GameDataManager->SpawnNextTargetActor();

		UUIFunctionLibrary::ActivateJoystick(GetWorld(), true);
		BaseStageInfo->UnPauseTimer();

		GameModeState = EGameModeStateEnum::VE_Start;
		GameContextState = EStageContextEnum::VE_Progress;

		UGamePlayBluePrintFunction::GameStartEvent();
		GetWorld()->GetTimerManager().ClearTimer(StartStageDelayTimeHandle);
	}

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
}

void AStageGameMode::EndGameMode()
{
	Super::EndGameMode();

	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;

	UUIFunctionLibrary::AttachHUDPage(GetWorld(), false);
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);

	GameInstance->PlayStateInfo->SetClearTime(BaseStageInfo->GetElapsedGameTime());
	GameInstance->PlayStateInfo->SetRemainingTime(BaseStageInfo->GetRemainingGameTime());
	

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

	GameInstance->GridMapManager->LeaveFromPlayLevel();
	
	GameInstance->EventManager->OnGameMode_ReadyToResult.RemoveDynamic(this, &AStageGameMode::OnReadyToResult);
	GameInstance->EventManager->OnGameMode_ReadyToResult.AddDynamic(this, &AStageGameMode::OnReadyToResult);

	GameModeState = EGameModeStateEnum::VE_Result;	
}

void AStageGameMode::ResultGameMode()
{
	Super::ResultGameMode();

	GameModeState = EGameModeStateEnum::VE_Result;

	
	USoundManager::GetInstancePtr()->FadeOutBGM();
	if (IsValid(GameInstance->EventManager))
		GameInstance->EventManager->OnGameMode_ReadyToResult.RemoveDynamic(this, &AStageGameMode::OnReadyToResult);
	if (IsValid(GameInstance->RWidgetManager))
		GameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_SkipButton);
}

void AStageGameMode::GamePlayModeEnd()
{
	Super::GamePlayModeEnd();
	UGamePlayFunctionLibrary::RequestGameEnd((int32)StarGrade);
}

void AStageGameMode::StopGameMode()
{
	Super::StopGameMode();
	RGAMEINSTANCE(this)->EventManager->OnGameMode_ReadyToResult.RemoveDynamic(this, &AStageGameMode::OnReadyToResult);
}

void AStageGameMode::ResultUI()
{
	UUIFunctionLibrary::OnShowInGameHUD(GetWorld(), false);
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
// 	if (RGameInstance->CampaignManager->bChangeQuest)
// 	{
// 		UUIFunctionLibrary::ShowQuestCompletePopup();
// 		GetWorld()->GetTimerManager().SetTimer(ResultUIDelayTimeHandle, this, &AStageGameMode::OnDelayedQuestCompletePopupHide, 3.0f);
// 	}
// 	else
	{
		GetWorld()->GetTimerManager().SetTimer(ResultUIDelayTimeHandle, this, &AStageGameMode::OnDelayedResultUI, 1.0f);
	}
}

void AStageGameMode::DestroyResource()
{
	for (int32 i = 0; i < DropItemActors.Num(); ++i)
	{
		if (DropItemActors.IsValidIndex(i))
		{
			if (IsValid(DropItemActors[i]))
			{
				DropItemActors[i]->Destroy();
				DropItemActors[i] = nullptr;
			}
		}
	}
	
	DropItemActors.Empty();

	Super::DestroyResource();
}

void AStageGameMode::OnDelayedQuestCompletePopupHide()
{
	UUIFunctionLibrary::HideQuestCompletePopup();
	OnDelayedResultUI();
}

void AStageGameMode::OnDelayedResultUI()
{
	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_CampaignResult);
}

void AStageGameMode::OnCompleteResourcePreLoading()
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
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &AStageGameMode::OnCompleteResourcePreLoading));
	}
	else
	{
		Super::OnCompleteResourcePreLoading();

		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("LVActor Spawn!"));
		BaseStageInfo->SetLevelActorBP();

		NextGameModeState = EGameModeStateEnum::VE_Ready;
	}


	
}

void AStageGameMode::OnCampResultEvent()
{
	RGAMEINSTANCE(this)->EventManager->OnCampaignResultEvent.RemoveDynamic(this, &AStageGameMode::OnCampResultEvent);
}

void AStageGameMode::OnReadyToStart()
{
	NextGameModeState = EGameModeStateEnum::VE_Start;

	if (IsValid(GameInstance->EventManager))
		GameInstance->EventManager->OnGameMode_ReadyToStart.RemoveDynamic(this, &AStageGameMode::OnReadyToStart);
	if (IsValid(GameInstance->RWidgetManager))
		GameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_CampaignStart);
}

void AStageGameMode::OnReadyToResult()
{
	NextGameModeState = EGameModeStateEnum::VE_Result;
}

void AStageGameMode::ShowReadyToStartUI()
{
	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	UUIFunctionLibrary::ReadyToStartUI_Campaign(BaseStageInfo->StageID);
}

EStageContextEnum AStageGameMode::GetStageContext()
{
	if (GameContextState != EStageContextEnum::VE_Fail && GameContextState != EStageContextEnum::VE_Success)
	{
		if (bIsTimeOut)
		{
			GameContextState = EStageContextEnum::VE_Fail;
			SaveStarGrade = StarGrade;
			StarGrade = EPlayStageStarEnum::VE_None;
		}
		else
		{
			int32 heroDieCount = RGAMEINSTANCE(this)->PlayStateInfo->GetHeroDieCount();

			UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
			FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
			checkf(FloorStageTable, TEXT("StageID Invalid"));

			int iMatchCount = 1;
			if (BaseStageInfo->bPlayAlone)
			{
				if (BaseStageInfo->GetElapsedGameTime() < FloorStageTable->StarCondition1)
					iMatchCount++;
				if (BaseStageInfo->GetElapsedGameTime() <= FloorStageTable->StarCondition2)
					iMatchCount++;
			}
			else
			{
				if (BaseStageInfo->GetElapsedGameTime() < FloorStageTable->StarCondition1)
					iMatchCount++;
				if (heroDieCount <= FloorStageTable->StarCondition2)
					iMatchCount++;
			}

			StarGrade = (EPlayStageStarEnum)iMatchCount;
			UpdateStarHUD(StarGrade);
		}
	}
	return GameContextState;
}

void AStageGameMode::UpdateStarHUD(EPlayStageStarEnum StarCnt)
{
	UUserWidget* InGameHudWidget = UUIFunctionLibrary::GetInGameHUD();
	if (InGameHudWidget)
	{
		URBaseIngameHUD* ThreeCampaignHUD = Cast<URBaseIngameHUD>(InGameHudWidget);
		if (ThreeCampaignHUD)
			ThreeCampaignHUD->SetStarHUD(StarGrade);

	}

}

void AStageGameMode::CheckItemDrop(AActor* DeadActor, AActor* FromActor)
{
	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(DeadActor);
	if (BattleComp == nullptr || !BattleComp->AmIEnemyWithMyPlayer()) // ally actor
		return;
	
	if (BattleComp->IsBoss())
	{
		// all item drop
		
		while(DropRewards.Num() != 0)
		{
			DropItem(DeadActor, true);
		}

		return;
	}

	if (20.0f < FMath::RandRange(0.0f, 100.0f))// 20% ?
		return;

	DropItem(DeadActor);
}

void AStageGameMode::DropItem(AActor* ItemGiveActor, bool bLastItem)
{
	int32 nMin = bLastItem ? 0 : 1;
	if (nMin < DropRewards.Num()) // 마지막 아이템은 보스에서
	{
		FREWARD& item = DropRewards[0];

		// 무기
		// 방어구
		// 악세사리
		// 룬
		// 기타
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = ItemGiveActor;
		SpawnParams.Instigator = ItemGiveActor->Instigator;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.bNoFail = true;
		
		USkeletalMeshComponent* skelcomp = ItemGiveActor->FindComponentByClass<USkeletalMeshComponent>();
		FVector location = skelcomp->GetSocketLocation(SOCKET_ROOT);
		location += FMath::VRand();
		ADropItemActor* itemActor = GetWorld()->SpawnActor<ADropItemActor>(ADropItemActor::StaticClass(), location, FRotator::ZeroRotator, SpawnParams);
		
		FDropItemResourceInfo* DropItemResourceInfo = nullptr;
		int32 nGrade = 0;
		
		switch (item.type)
		{

		case EREWARD_TYPE::ITEM:
		{
			FItemTableInfo*		ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(item.rewardId);
			if ((uint8)EItemEquipTypeEnum::VE_Sword <= (uint8)ItemTableInfo->EquipType && (uint8)ItemTableInfo->EquipType <= (uint8)EItemEquipTypeEnum::VE_Wand)
			{
				// case weapon
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropWeapon")));
			}
			else if ((uint8)EItemEquipTypeEnum::VE_HeavyArmor <= (uint8)ItemTableInfo->EquipType && (uint8)ItemTableInfo->EquipType <= (uint8)EItemEquipTypeEnum::VE_FabricPants)
			{
				// case armor
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropArmor")));
			}
			else if ((uint8)EItemEquipTypeEnum::VE_Necklace <= (uint8)ItemTableInfo->EquipType && (uint8)ItemTableInfo->EquipType <= (uint8)EItemEquipTypeEnum::VE_Ring)
			{
				// case necklace, ring
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropAccessory")));
			}

			nGrade = (int32)ItemTableInfo->ItemGrade;
		}
		break;

		case EREWARD_TYPE::WALLET:
		{
			FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(item.rewardId);
			if (WalletInfo->PropertyType == EWalletPropertyType::Rune)
			{
				// case rune
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropRune")));
			}
			else
			{
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropEtc")));
			}

			nGrade = (WalletInfo->Grade == 0) ? 0 : WalletInfo->Grade - 1;
		}
		break;

		default:
		{
			// case etc
			DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropEtc")));
			nGrade = 1;
		}
		break;
		}

		if(DropItemResourceInfo)
		{
			USkeletalMesh* SkelMesh = UUtilFunctionLibrary::GetSkeletalMeshFromAssetID(DropItemResourceInfo->SkeletalMesh);
			UAnimationAsset* DropAnimAsset = UUtilFunctionLibrary::GetAnimationAssetFromAssetID(DropItemResourceInfo->Animation_Drop);
			UAnimationAsset* IdleAnimAsset = UUtilFunctionLibrary::GetAnimationAssetFromAssetID(DropItemResourceInfo->Animation_Idle);

			itemActor->SetDropItemData(SkelMesh, DropAnimAsset, IdleAnimAsset, nGrade);
			DropItemActors.Emplace(itemActor);
		}

		DropRewards.RemoveAt(0);
	}
}