// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PlayState.h"

#include "ProjectRGameMode.h"

#include "GlobalIntegrated.h"

#include "GameState/Stage/BaseLevelScriptActor.h"
#include "Network/Chat/ChatManager.h"
#include "UtilFunctionIntegrated.h"
#include "Table/MapTableInfo.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "Network/RTSManager.h"
#include "UI/RWidgetManager.h"

#include "Object/WallActor.h"

#include "DirectionTrackManager.h"

#include "ContentGameMode/Modes/TowerLobbyGameMode.h"



UPlayState::UPlayState()
{
	StateType = EGameStateEnum::VE_Play;
}

void UPlayState::Enter()
{
	UE_LOG(LogBattle, Log, TEXT("Enter play state "));
//	URAsyncLoader::GetInstancePtr()->ClearResource();
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (GameInstance)
	{
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
		if (pGameMode)
		{
			pGameMode->ObjectHelper.RemoveObjectAll();
		}

		if (GameInstance->RWidgetManager)
		{
			GameInstance->RWidgetManager->RemoveAllUserWidget();
		}

		if (GameInstance->EventManager)
		{
			GameInstance->EventManager->OnDeadActor.Clear();
		}

		if (IsValid(GameInstance->ChatManager))
		{
			switch (UUtilFunctionLibrary::GetGamePlayModeEnum())
			{
			case EGamePlayModeTypeEnum::VE_BattleField:
			case EGamePlayModeTypeEnum::VE_BattleGround:
			case EGamePlayModeTypeEnum::VE_GuildRaid:
				GameInstance->ChatManager->SubscribeToRealTimeChannel();
			break;
			}
		}

		// Persistant Level
		UMapFunctionLibrary::LoadLevel(BaseStageInfo->GetSelectedLevel(), false );
	}
}

void UPlayState::Leave()
{
	UE_LOG(LogBattle, Log, TEXT("Leave play state "));

	if (GameInstance)
	{
		SubLevels.Empty();
		USoundManager::GetInstancePtr()->StopBGM();
		
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
		if (pGameMode)
		{
			pGameMode->DestroyResource();
		}


		if (IsValid(GameInstance->RealTimeModManager))
		{
			GameInstance->RealTimeModManager->ClearRealTimeModData();
			GameInstance->RealTimeModManager->DisConnect();
		}

		if (IsValid(GameInstance->PlayStateInfo))
		{
			GameInstance->PlayStateInfo->ClearSelectedCharacter(true);
		}

		if (IsValid(GameInstance->OtherHeroManager))
		{
			GameInstance->OtherHeroManager->RemoveAll();
		}

		if (GameInstance->SequenceManager)
		{
			GameInstance->SequenceManager->ClearSequence();
		}

		if (GameInstance->GameDataManager)
		{
			GameInstance->GameDataManager->ClearNextTargetActor();
		}

		if (GameInstance->EventManager)
		{
			GameInstance->EventManager->OnDeadActor.Clear();
		}

		if (GameInstance->RWidgetManager)
		{
			GameInstance->RWidgetManager->RemoveAllUserWidget();
			//GameInstance->GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
		}

		if (IsValid(GameInstance->ChatManager))
		{
			switch (UUtilFunctionLibrary::GetGamePlayModeEnum())
			{
			case EGamePlayModeTypeEnum::VE_BattleField:
			case EGamePlayModeTypeEnum::VE_BattleGround:
			case EGamePlayModeTypeEnum::VE_GuildRaid:
				GameInstance->ChatManager->UnsubscribeFromRealTimeChannel();
				break;
			}
		}
		if (URealTimeModManager::IsRealTimeGameMode())
		{
			GameInstance->RealTimeModManager->ClearRealTimeModData();
		}
		USoundManager::GetInstancePtr()->ClearSoundList();
		URAsyncLoader::GetInstancePtr()->ClearResource();

		SpawnBoxMap.Empty();
		WallMap.Empty();
		WaypointMap.Empty();
		NextStageTableInfo = nullptr;

		
		
	}
}

void UPlayState::BeginState()
{
	Super::BeginState();

	if (GameInstance == nullptr)
		return;

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (NextStageTableInfo)
	{
		TArray<FName> FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
		BaseStageInfo->ClearRewardTableKey = NextStageTableInfo->ClearRewardTableKey;
		BaseStageInfo->MapTableKey = FName(*FString::FromInt(NextStageTableInfo->MapTableKey));
		BaseStageInfo->FloorInfoId = FName(*FString::FromInt(NextStageTableInfo->FloorInfoId));
		BaseStageInfo->StageIndex = NextStageTableInfo->StageIdx;
		BaseStageInfo->StageID = FloorStageIDs[NextStageTableInfo->StageIdx];
		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
		{
			BaseStageInfo->bPlayAlone = true;
		}
		else if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR))
		{
			BaseStageInfo->bPlayAlone = false;
		}
		else if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		{
			BaseStageInfo->bPlayAlone = true;
		}
		else
		{
			BaseStageInfo->bPlayAlone = NextStageTableInfo->PlayType == FName(TEXT("H")) ? true : false;
		}
		
		NextStageTableInfo = nullptr;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("Before Changing Init State"));
	UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Init);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("After Changing Init State"));
	// sublevel
	SubLevels = BaseStageInfo->GetSubLevels();
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("LoadingSublevel Start"));
	LoadSubLevels();
	
}

void UPlayState::Retry()
{
	if (GameInstance)
	{
		UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		checkf(BaseStageInfo, TEXT("CurrentStageInfo is Null"));
		TArray<FName> FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
		FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[BaseStageInfo->StageIndex]);
		checkf(FloorStageTable, TEXT("Failed to find FloorStageDetail Table"));

		bool IsEnoughStamina = !(GameInstance->RInventory->GetStamina() == 0 || FloorStageTable->Stamina > GameInstance->RInventory->GetStamina());
		if (!IsEnoughStamina)
			return;

		if (GameInstance->PlayModeManager)
		{
			FPlay_Heroes_Info CurrentInfo = GameInstance->PlayModeManager->GetPlayHeroInfo();
			UGamePlayFunctionLibrary::RequestGameStart(CurrentInfo);

			OnReceivedPlayPacket.BindUObject(this, &UPlayState::Callback_ReceivePlayPacket, true);
		}
	}
}

void UPlayState::NextStage()
{
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	checkf(BaseStageInfo, TEXT("CurrentStageInfo is Null"));

	TArray<FName> FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
	FFloorStageDetail* CurStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[BaseStageInfo->StageIndex]);
	checkf(CurStageTable, TEXT("Failed to find FloorStageDetail Table"));
	
		
	FMapTableInfo* CurrentMap = UTableManager::GetInstancePtr()->GetMapRow(FName(*FString::FromInt(CurStageTable->MapTableKey)));
	FMapTableInfo* NextMap = UTableManager::GetInstancePtr()->GetMapRow(FName(*FString::FromInt(NextStageTableInfo->MapTableKey)));


	FName CurrentPMap = CurrentMap->P_Map;
	FName NextPMap = NextMap->P_Map;
	isPMapChanged = CurrentPMap == NextPMap ? false : true;

	if (GameInstance)
	{
		bool IsEnoughStamina = !(GameInstance->RInventory->GetStamina() == 0 || NextStageTableInfo->Stamina > GameInstance->RInventory->GetStamina());
		if (!IsEnoughStamina)
			return;

		SpawnBoxMap.Empty();
		WallMap.Empty();
		WaypointMap.Empty();

		if (BaseStageInfo)
		{
			ALevelActor* LevelActor = BaseStageInfo->GetLevelActor();
			LevelActor->Destroy();

			BaseStageInfo->ClearRewardTableKey = NextStageTableInfo->ClearRewardTableKey;
			BaseStageInfo->MapTableKey = FName(*FString::FromInt(NextStageTableInfo->MapTableKey));
			BaseStageInfo->FloorInfoId = FName(*FString::FromInt(NextStageTableInfo->FloorInfoId));
			BaseStageInfo->StageIndex = NextStageTableInfo->StageIdx;
			BaseStageInfo->StageID = FloorStageIDs[NextStageTableInfo->StageIdx];
		}

		if (GameInstance->PlayModeManager)
		{
			FPlay_Heroes_Info CurrentInfo = GameInstance->PlayModeManager->GetPlayHeroInfo();
			UGamePlayFunctionLibrary::RequestGameStart(CurrentInfo);

			OnReceivedPlayPacket.BindUObject(this, &UPlayState::Callback_ReceivePlayPacket, false);
		}
	}

}

void UPlayState::Callback_ReceivePlayPacket(bool isRetry)
{
	SubLevels.Empty();

	USoundManager::GetInstancePtr()->StopBGM();
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->DestroyResource();
	}

	if (IsValid(GameInstance->RealTimeModManager))
	{
		GameInstance->RealTimeModManager->ClearRealTimeModData();
		GameInstance->RealTimeModManager->DisConnect();
	}

	if (IsValid(GameInstance->OtherHeroManager))
	{
		GameInstance->OtherHeroManager->RemoveAll();
	}

	if (GameInstance->SequenceManager)
	{
		GameInstance->SequenceManager->ClearSequence();
	}

	if (GameInstance->GameDataManager)
	{
		GameInstance->GameDataManager->ClearNextTargetActor();
	}

	if (GameInstance->EventManager)
	{
		GameInstance->EventManager->OnDeadActor.Clear();
	}

	if (GameInstance->RWidgetManager)
	{
		GameInstance->RWidgetManager->RemoveAllUserWidget();
	}

	UWorld* CurrentWorld = GameInstance->GetWorld();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(CurrentWorld, ABaseCameraActor::StaticClass(), OutActors);
	if (OutActors.Num())
	{
		ABaseCameraActor* CamActor = Cast<ABaseCameraActor>(OutActors[0]);
		APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(CurrentWorld, 0);
		if (PCM)
		{
			FViewTargetTransitionParams BlendParams;
			BlendParams.BlendTime = 0.f;
			PCM->SetViewTarget(CamActor, BlendParams);
			PCM->StopCameraFade();
		}
	}

	ABaseLevelScriptActor* BaseLSA = Cast<ABaseLevelScriptActor>(CurrentWorld->GetLevelScriptActor());
	if (BaseLSA)
	{
		URenderFunctionLibrary::SetVignetteIntensity(BaseLSA->InitialVignetteIntensity);
	}

	if (isRetry)
	{
		/*WayPoint Trigger Reset*/
		TArray<AActor*> OutWaypoints;
		UGameplayStatics::GetAllActorsOfClass(CurrentWorld, ASpawnerTransform::StaticClass(), OutWaypoints);
		for (size_t i = 0; i < OutWaypoints.Num(); ++i)
		{
			ASpawnerTransform* spawnerTransform = Cast<ASpawnerTransform>(OutWaypoints[i]);
			if (IsValid(spawnerTransform))
				spawnerTransform->InitTrigger();
		}
		/**/

		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Init);
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Prepare);
		if (GameInstance->BaseStageInfo)
		{
			ALevelActor* LevelActor = GameInstance->BaseStageInfo->GetLevelActor();
			LevelActor->Destroy();
			GameInstance->BaseStageInfo->SetLevelActorBP();
		}
	}
	else
	{
		UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		checkf(BaseStageInfo, TEXT("CurrentStageInfo is Null"));

		if (isPMapChanged)
		{
			FString backupUD = BaseStageInfo->PlayUD;

			UMapFunctionLibrary::LoadLevel(BaseStageInfo->GetSelectedLevel(), false);

			FTimerDelegate DelayedCall;
			DelayedCall.BindLambda([BaseStageInfo, backupUD]
			{
				BaseStageInfo->PlayUD = backupUD;
			});

			GameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(DelayedCall);
		}
		else
		{
			UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Init);
			SubLevels = BaseStageInfo->GetSubLevels();
			LoadSubLevels();
		}
	}
	
}

void UPlayState::OnCompleteLoadingLevel()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("Sublevel Loaded Sucessfully!"));

	TArray<class ULevelStreaming*> LSArray = RGAMEINSTANCE(GEngine)->GetWorld()->StreamingLevels;
	for (size_t i = 0; i < LSArray.Num(); ++i)
	{
		if (LSArray[i]->bShouldBeLoaded == 0)
			continue;

		FString str = LSArray[i]->GetWorldAssetPackageName();
		if (LSArray[i]->GetLoadedLevel()->IsValidLowLevel()  && !str.Contains(SubLevels[0].ToString()))
		{
			int32 iIdx = str.Find(TEXT("LF"));
			str = str.RightChop(iIdx);
			UE_LOG(LogClass, Log, TEXT("%s"), *str);
			UMapFunctionLibrary::UnLoadSubLevel(FName(*str), FName("OnCompleteLoadingLevel"), this);
			return;
		}
	}

	--SubLevelCount;

	if (SubLevelCount > 0)
		return;
	ComplateState();

	if (UGamePlayFunctionLibrary::IsCampaignMode(UUtilFunctionLibrary::GetGamePlayModeEnum()))
	{
		
		/*Cache spawner infos in advance*/
		for (size_t i = 0; i < LSArray.Num(); ++i)
		{
			if (LSArray[i]->bShouldBeLoaded == 1 && LSArray[i]->bShouldBeVisible == 1)
			{
				ULevel* LoadedLevel = LSArray[i]->GetLoadedLevel();
				for (size_t j = 0; j < LoadedLevel->Actors.Num(); ++j)
				{
					ASpawnBox* SpawnBox = Cast<ASpawnBox>(LoadedLevel->Actors[j]);
					if (SpawnBox)
						SpawnBoxMap.Emplace(SpawnBox->GetFName(), SpawnBox);

					AWallActor* WallActor = Cast<AWallActor>(LoadedLevel->Actors[j]);
					if (WallActor)
						WallMap.Emplace(WallActor->GetFName(), WallActor);

					ASpawnerTransform* WayPoint = Cast<ASpawnerTransform>(LoadedLevel->Actors[j]);
					if (WayPoint)
						WaypointMap.Emplace(WayPoint->GetFName(), WayPoint);

				}
				break;
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("Prepare State Start!"));
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Prepare);
	}
	else
	{
		CompleteLoadingState();
	}
}

void UPlayState::CompleteLoadingState()
{

	UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Prepare);
}

void UPlayState::LoadSubLevels()
{
	if (!IsValid( GameInstance) )
		return;
	SubLevelCount = 0;
	for (auto& SubLevel : SubLevels)
	{
		if (!SubLevel.IsNone())
		{
			UMapFunctionLibrary::LoadLevel(SubLevel, true, FName("OnCompleteLoadingLevel"), this);
			++SubLevelCount;
		}
	}
	if(SubLevelCount <= 0 )
	{
		UUtilFunctionLibrary::CheckGameModeState();
		CompleteLoadingState();
	}
}

