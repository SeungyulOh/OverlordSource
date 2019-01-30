// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "SharedConstants/GlobalConstants.h"


#include "HUDBase.h"
#include "GlobalIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"
#include "CustomStructures/InfiniteSpawnerInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Character/BattleObject/BattleBaseObject.h"
#include "Character/Bullets/BulletBase.h"
#include "Character/Components/WidgetDecoratorComponent.h"//temp
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Etc/DeadActor.h"
#include "Character/AnimNotification/AttachResourceFXNotify.h"

#include "Animation/AnimTypes.h"

#include "Skill_V2/ContinuousSkillApplierActor.h"

#include "UI/RWidgetManager.h"
#include "UI/InGameHUD/UP_InGameHUD_Arena.h"
#include "UI/InGameHUD/UP_InGameHUD_Mirror.h"
#include "DamageFloater_PS.h"

#include "ContentGameMode/Modes/StageGameMode.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"

#include "DirectionTrackManager.h"


AProjectRGameMode::AProjectRGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->SetTickableWhenPaused(true);
	PrimaryActorTick.bCanEverTick = true;
	PlayerControllerClass = ABasePlayerController::StaticClass();
	DefaultPawnClass = nullptr;

	DamageFloaterMgr = CreateDefaultSubobject<UDamageFloaterManager>(FName(TEXT("DamageFloaterManager")));
	DirectionTrackManager = CreateDefaultSubobject<UDirectionTrackManager>(FName(TEXT("DirectionTrackManager")));

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MTFinder(TEXT("MaterialInstanceConstant'/Game/Shaders/Postprocess/Outline/PP_Outline_MI_Android.PP_Outline_MI_Android'"));
	if (MTFinder.Succeeded())
		MIC_Android = MTFinder.Object;


}

void AProjectRGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	StopGameMode();
	BaseStageInfo->Reset();
	
		
	
	Super::EndPlay(EndPlayReason);
}

void AProjectRGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

void AProjectRGameMode::OnCompleteResourcePreLoading()
{
	TArray<FSoftObjectPath> Assets = URAsyncLoader::GetInstancePtr()->GetAssetPath();
	for (size_t i = 0; i < Assets.Num(); ++i)
	{
		if (Assets[i].IsValid())
		{
			if(Cast<UClass>(Assets[i].ResolveObject()))
				ObjectHelper.CachedUClassArray.AddUnique(Cast<UClass>(Assets[i].ResolveObject()));
			else if (Assets[i].ResolveObject())
				ObjectHelper.CachedResourceArray.AddUnique(Assets[i].ResolveObject());
			else
			{
				FString str = TEXT("Fail to load asset : ") + Assets[i].GetAssetName();
				UE_LOG(LogClass, Log, TEXT("%s") , *str);
			}
		}
	}

	if(IsValid(DamageFloaterMgr))
		DamageFloaterMgr->Init();
	
}

void AProjectRGameMode::ChangeGameModeState(EGameModeStateEnum state, bool InForceExecute /*=false*/)
{
	NextGameModeState = state;
	CheckGameModeState(InForceExecute);
}

void AProjectRGameMode::StopGameMode()
{
	
}

void AProjectRGameMode::CheckGameModeState(bool inForceExecute/* = false */)
{
	if (inForceExecute == false && GameModeState == NextGameModeState)
		return;
	if (GameModeState > NextGameModeState && NextGameModeState != EGameModeStateEnum::VE_Init)
		return;

	URAsyncLoader::GetInstancePtr()->ClearFxQueue();
	GameModeState = NextGameModeState;

#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif

	switch (GameModeState)
	{
	case EGameModeStateEnum::VE_Init:		InitGameMode();		break;
	case EGameModeStateEnum::VE_Prepare:	PrepareGameMode();	break;
	case EGameModeStateEnum::VE_Ready:		ReadyGameMode();	break;
	case EGameModeStateEnum::VE_Start:		StartGameMode();	break;
	case EGameModeStateEnum::VE_End:		EndGameMode();		break;
	case EGameModeStateEnum::VE_Result:		ResultGameMode();	break;
	}

#if LOG_TIME
	double PrintTime = FPlatformTime::Seconds() - StartTime;
	UE_LOG(LogGamePlay, Log, TEXT("%f ____CheckGameModeState(%s)"), PrintTime, *(UUtilFunctionLibrary::GetEnumToString(TEXT("EGameModeStateEnum"),(int32)GameModeState)));
#endif
}


void AProjectRGameMode::InitGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	GameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

#if LOG_TIME
	LoadingStartTime = FPlatformTime::Seconds();
#endif
	ObjectHelper.RemoveObjectAll();
	//SetGamePlayMode(BaseStageInfo->GetGameMode());
	URAsyncLoader::GetInstancePtr()->ClearFxQueue();
	BaseStageInfo->Init();

	GameInstance->LoadingSceneManager->ShowLoadingScene(GameInstance->CurrentGameMode);

	FName MapID = BaseStageInfo->MapTableKey;
	if (MapID.IsNone())
	{
		this->GameModeSequenceKey = MapID;
	}
	else
	{
		FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapID);
		if (MapTableInfo)
			this->GameModeSequenceKey = MapTableInfo->GameModeSequenceKey;
	}

	URenderFunctionLibrary::SetPostProcessSetting(BaseStageInfo->MapTableKey);
	
#if PLATFORM_ANDROID
	for (auto& postProcessVolume : GetWorld()->PostProcessVolumes)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			if (PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(0))
			{
				PPVolume->Settings.WeightedBlendables.Array[0].Object = MIC_Android;
			}
			break;
		}
	}
#endif

	if (IsValid(DirectionTrackManager))
		DirectionTrackManager->Initialize();
	
		
}

void AProjectRGameMode::PrepareGameMode()
{
	
}


void AProjectRGameMode::ReadyGameMode()
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();


	

	USoundManager::GetInstancePtr()->PlayBattleBGMusic(BaseStageInfo->MapTableKey);
	GameInstance->GridMapManager->LoadMapInfoFromActor();
	GameInstance->LoadingSceneManager->HideLoadingScene();

	iTryCntToLoadSync = 0;
}

void AProjectRGameMode::StartGameMode()
{	
	
}

void AProjectRGameMode::EndGameMode()
{
	URAsyncLoader::GetInstancePtr()->ClearFxQueue();

}

void AProjectRGameMode::ResultGameMode()
{
// #if ENABLE_FORCE_GC
// 	if( GetWorld() )
// 		GetWorld()->ForceGarbageCollection(true);
// #endif
	switch (GetStageContext())
	{
		case EStageContextEnum::VE_Success:
		{
			GamePlayModeEnd(true);
		}
		break;
		case EStageContextEnum::VE_Fail:
		{
			GamePlayModeEnd(false);
		}
		break;
	}
}

/*
Override functions..
*/
void AProjectRGameMode::Tick(float DeltaTime)
{	
	

#ifndef RULE_TESTDELETE
	Super::Tick(DeltaTime);
#endif
	if (!IsValid(GameInstance))
		return;

	switch (GameInstance->GetCurrentStateType())
	{
	case EGameStateEnum::VE_Play:
	case EGameStateEnum::VE_Tutorial:
		break;
	default:
		return;
	}

	if (Cast<AStageGameMode>(this) && GameModeState == EGameModeStateEnum::VE_End)
		return;
	CheckGameModeState();
	if (GameModeState == EGameModeStateEnum::VE_Start)
	{
		GameContextState = GetStageContext();
		if (GameContextState == EStageContextEnum::VE_Success || GameContextState == EStageContextEnum::VE_Fail)
		{
			ChangeGameModeState(EGameModeStateEnum::VE_End);
		}
	}
	URAsyncLoader::GetInstancePtr()->LoadFx();

	if (IsValid(DirectionTrackManager) && DirectionTrackManager->GetSizeHeap())
		DirectionTrackManager->Update(DeltaTime);


	if (!SceneCapturer)
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->GameEnvironmentVar.OutLineElapsedTime >= RGameInstance->GameEnvironmentVar.OutLineDuration)
	{
		SceneCapturer->SetActive(false);
		return;
	}

	SceneCapturer->SetActive(true);
	RGameInstance->GameEnvironmentVar.OutLineElapsedTime += DeltaTime;
	URenderFunctionLibrary::SetOutLineEffect(RGameInstance->GameEnvironmentVar.OutLineElapsedTime);
}

bool AProjectRGameMode::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = false;
	//if (IsValid(GameInstance))
	{
		bResult = GameInstance->ConsoleCheatManager->ProcessConsoleExec(Cmd, Ar, Executor);
	}

	if (bResult == false)
		bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);

	return bResult;
}

void AProjectRGameMode::DelayNextStage(EGameModeStateEnum InDelayedNextModeState, float DelayTime)
{
	DelayedGameModeState = InDelayedNextModeState;

	if (DelayTime > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedGameMode_TimeHandler, this, &AProjectRGameMode::OnChangeDelayedNextStage, DelayTime);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedGameMode_TimeHandler, this, &AProjectRGameMode::OnChangeDelayedNextStage, 0.01f, true);
	}
}

void AProjectRGameMode::OnChangeDelayedNextStage()
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	
	GetWorld()->GetTimerManager().ClearTimer(DelayedGameMode_TimeHandler);
	NextGameModeState = DelayedGameModeState;
}

void AProjectRGameMode::ClearDelayedNextStage()
{
	DelayedGameModeState = EGameModeStateEnum::VE_None;
	GetWorld()->GetTimerManager().ClearTimer(DelayedGameMode_TimeHandler);
}

void AProjectRGameMode::StageClearSuccess(EPlayStageStarEnum InGrade)
{
	GameContextState = EStageContextEnum::VE_Success;
	StarGrade = InGrade;
}

void AProjectRGameMode::StageClearFail()
{
	GameContextState = EStageContextEnum::VE_Fail;
	StarGrade = EPlayStageStarEnum::VE_None;;
}

void AProjectRGameMode::InitCameraTransform(FVector CameraPosition)
{
	if (GameInstance == nullptr)
		return;
	if (IsValid(GameInstance->CameraActor))
		GameInstance->CameraActor->SetActorLocation(CameraPosition);
}

void AProjectRGameMode::GamePlayModeEnd(bool bResult)
{
	if (!IsValid(GameInstance))
		return;	
	
	GameInstance->IsSuccess = bResult;
	
	if (GameInstance->HttpClient->IsValidNetwork())
	{
		GamePlayModeEnd();
		ResultUI();
	}
	else
	{
		ResultUI();
	}
}

void AProjectRGameMode::GamePlayModeEnd()
{

}

void AProjectRGameMode::AddTryCntLoadSync(FString id)
{
	++iTryCntToLoadSync;

	FString str = TEXT("this following Resource should be cached before entering stage..   Resource Name : ") + id;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);

	UE_LOG(LogClass, Warning, TEXT("%s"), *str);
}

void AProjectRGameMode::ResultUI()
{

}

void AProjectRGameMode::DestroyResource()
{
	ObjectHelper.RemoveObjectAll();
	ObjectHelper.AllObjectVisible = true;
}

void FObjectHelper::GetObjectList(TArray< TWeakObjectPtr< AActor > >& outObjectList)
{
	outObjectList = ObjectList;
}

void FObjectHelper::GetObjectList(TArray<int32> UOKeyList, TArray< TWeakObjectPtr< AActor > >& outObjectList)
{
	outObjectList.Empty();

	for (int32 PlayerUK : UOKeyList)
	{
		TWeakObjectPtr< AActor > findObject = GetObjectByPlayerUK(PlayerUK);
		if (findObject.IsValid())
		{
			outObjectList.Emplace(findObject);
		}
	}
}

void FObjectHelper::GetFriendList(TArray< TWeakObjectPtr< AActor > >& outFriendList, AActor* Source)
{
	if (!IsValid(Source))
		return;
	auto Source_BaseProperty = Cast<IEntityBaseProperty>(Source);
	if (Source_BaseProperty == nullptr)
		return;

	bool EnemyTarget = false;

	for (auto& objectInstance : ObjectList)
	{
		if (!objectInstance.IsValid())
			continue;
		auto Target_BaseProperty = Cast<IEntityBaseProperty>(objectInstance.Get());
		if (Target_BaseProperty == nullptr)
			continue;

		EnemyTarget = URenewal_BattleFunctionLibrary::IsEnemy(Source_BaseProperty->GetPlayerUK(), Source_BaseProperty->GetGroupKey(),
			Target_BaseProperty->GetPlayerUK(), Target_BaseProperty->GetGroupKey());

		if (EnemyTarget == false)
		{
			outFriendList.Emplace(objectInstance);
		}
	}
}

void FObjectHelper::GetPartyList(TArray< TWeakObjectPtr< AActor > >& outPartyList, AActor* Source)
{
	if (!IsValid(Source))
		return;

	auto Source_BaseProperty = Cast<IEntityBaseProperty>(Source);
	if (Source_BaseProperty == nullptr)
		return;

	outPartyList.Empty();
	int32 MyPartyKey = Source_BaseProperty->GetPartyID();
	for (auto& objectInstance : ObjectList)
	{
		if (!objectInstance.IsValid())
			continue;
		auto Target_BaseProperty = Cast<IEntityBaseProperty>(objectInstance.Get());
		if (Target_BaseProperty == nullptr)
			continue;

		if (Target_BaseProperty->GetPartyID() == MyPartyKey)
			outPartyList.Emplace(objectInstance);
	}

}

void FObjectHelper::GetEnemyList(TArray< TWeakObjectPtr< AActor > >& outEnemyList, AActor* Source)
{
	if (!IsValid(Source))
		return;
	auto Source_BaseProperty = Cast<IEntityBaseProperty>(Source);
	if (Source_BaseProperty == nullptr)
		return;

	bool EnemyTarget = false;

	for (auto& objectInstance : ObjectList)
	{
		if (!objectInstance.IsValid())
			continue;
		auto Target_BaseProperty = Cast<IEntityBaseProperty>(objectInstance.Get());
		if (Target_BaseProperty == nullptr)
			continue;
		auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(objectInstance.Get());
		if (!IsValid(Target_BattleEntityComponent))
			continue;
		if (Target_BattleEntityComponent->IsDead() || Target_BattleEntityComponent->IsSearchSkip())
			continue;

		if (Target_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_NeutralNpc)
			continue;

		EnemyTarget = URenewal_BattleFunctionLibrary::IsEnemy(Source_BaseProperty->GetPlayerUK(), Source_BaseProperty->GetGroupKey(),
			Target_BaseProperty->GetPlayerUK(), Target_BaseProperty->GetGroupKey());

		if (EnemyTarget == true)
		{
			outEnemyList.Emplace(objectInstance);
		}
	}
}

void FObjectHelper::GetMySummonList(TArray< TWeakObjectPtr< AActor > >& outMySummonList, AActor* Source)
{
	if (IsValid(Source))
	{
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Source);
		if (IsValid(BattleComp))
		{
			outMySummonList += BattleComp->GetBattleInfo()->GetSummonedMinions();
		}
	}
}

void FObjectHelper::GetFriendSummonList(TArray< TWeakObjectPtr< AActor > >& outFriendSummonList)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (IsValid(RGameInstance->PlayStateInfo))
	{
		auto& SelecteCharacterArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
		for (auto& CharacterInfo : SelecteCharacterArray)
		{
			auto Character = CharacterInfo.SpawnedTargetActor;

			if (IsValid(Character))
			{
				UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Character);
				if (IsValid(BattleComp))
				{
					outFriendSummonList += BattleComp->GetBattleInfo()->GetSummonedMinions();
				}
			}
		}
	}
}

void FObjectHelper::AddObject(AActor* obj, int32 InPlayerUK)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

// 	if (!IsValid(RGameInstance))
// 		return;
	if (!IsValid(obj))
		return;
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(obj);
	if (BaseProperty == nullptr)
		return;
	if (!ObjectList.Contains(obj))
	{
		ObjectList.Emplace(obj);
	}
	RegisterPlayerUK(obj, InPlayerUK);
	RGameInstance->PartyManager->AddPartyMember(BaseProperty->GetPartyID(), BaseProperty->GetPlayerUK());
}

void FObjectHelper::RemoveObject(AActor * obj, bool bDestory)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

// 	if (!IsValid(RGameInstance))
// 		return;
	if (!IsValid(obj))
		return;
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(obj);
	if (BaseProperty == nullptr)
		return;
	RGameInstance->PartyManager->RemovePartyMember(BaseProperty->GetPartyID(), BaseProperty->GetPlayerUK());
	PlayerUKMap.Remove(BaseProperty->GetPlayerUK());
	if (ObjectList.Contains(obj))
		ObjectList.Remove(obj);
	if (bDestory)
	{
		obj->Destroy();
		obj = nullptr;
	}
}

void FObjectHelper::RemoveObject(int32 InPlayerUK, bool bDestory)
{
	auto FindObject = GetObjectByPlayerUK(InPlayerUK);
	if (FindObject.IsValid() == false)
		return;

	RemoveObject(FindObject.Get(), bDestory);
}

TWeakObjectPtr< AActor > FObjectHelper::GetObjectByPlayerUK(int32 InPlayerUK)
{
	if (0 > InPlayerUK)
		return nullptr;

	if (PlayerUKMap.Contains(InPlayerUK))
	{
		return *PlayerUKMap.Find(InPlayerUK);
	}

	return nullptr;
}

void FObjectHelper::RegisterPlayerUK(AActor* obj, int32 InPlayerUK)
{
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(obj);
	if (BaseProperty)
	{
		if (InPlayerUK != PLAYER_UK_NONE)
		{
			BaseProperty->SetPlayerUK(InPlayerUK);
		}
		else
		{
			BaseProperty->SetPlayerUK(GeneratePlayerUK());
		}

		PlayerUKMap.Emplace(BaseProperty->GetPlayerUK(), obj);
	}
}

void FObjectHelper::RemoveObjectByKey(int32 InPlayerUK)
{
	TWeakObjectPtr< AActor > obj = *PlayerUKMap.Find(InPlayerUK);
	if (obj.IsValid())
	{
		RemoveObject(obj.Get());
		// don't remove keymap entity in here, removeObject will remove key map entity.
	}
}

int32 FObjectHelper::GeneratePlayerUK()
{
	++PlayerUKSerial;
	int64 CalcPlayerUK = PlayerUKSerial * PLAYER_UK_UNIT;
	if (MAX_int32 <= CalcPlayerUK)
	{
		PlayerUKSerial = 1;
		CalcPlayerUK = PlayerUKSerial * PLAYER_UK_UNIT;
	}

	return CalcPlayerUK;
}

void FObjectHelper::RemoveObjectAll(ECharacterCategoryTypeEnum CharacterCategory)
{
	TArray< TWeakObjectPtr<AActor>> DestroyList;
	for (TWeakObjectPtr<AActor> actor : ObjectList)
	{
		if (actor.IsValid())
		{
			auto BaseProperty = Cast<IEntityBaseProperty>(actor.Get());
			if (BaseProperty)
			{
				if (BaseProperty->GetCharacterCategory() == CharacterCategory)
				{
					DestroyList.Emplace(actor);
				}
			}
		}
	}

	for (TWeakObjectPtr<AActor> actor : DestroyList)
	{
		if (actor.IsValid())
			RemoveObject(actor.Get(), true);
	}
}

void FObjectHelper::RemoveObjectAll(TArray< TWeakObjectPtr< AActor >> ignoreList)
{
	TArray< TWeakObjectPtr<AActor>> DestroyList;
	for (TWeakObjectPtr<AActor> actor : ObjectList)
	{
		if (actor.IsValid())
		{
			if (!ignoreList.Contains(actor))
			{
				DestroyList.Emplace(actor);
			}
		}
	}

	for (TWeakObjectPtr<AActor> actor : DestroyList)
	{
		if (actor.IsValid())
			RemoveObject(actor.Get(), true);
	}
}

void FObjectHelper::RemoveSelectObjectAll(TArray< TWeakObjectPtr< AActor >> DeleteList)
{
	TArray< TWeakObjectPtr<AActor>> DestroyList;
	for (TWeakObjectPtr<AActor> actor : ObjectList)
	{
		if (actor.IsValid())
		{
			if (DeleteList.Contains(actor))
			{
				DestroyList.Emplace(actor);
			}
		}
	}

	for (TWeakObjectPtr<AActor> actor : DestroyList)
	{
		if (actor.IsValid())
			RemoveObject(actor.Get(), true);
	}
}

void FObjectHelper::RemoveObjectAll()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

	
	{
		TArray<AActor* > arraylist;
		UGameplayStatics::GetAllActorsOfClass(RGameInstance->GetWorld(), ADeadActor::StaticClass(), arraylist);
		for (AActor* actor : arraylist)
		{
			if (IsValid(actor))
			{
				actor->Destroy();
			}
		}
	}

	ObjectList.Empty();
	CachedResourceArray.Empty();
	CachedUClassArray.Empty();


	PlayerUKSerial = 0;

	for (TActorIterator<ABattleBaseCharacter> ActorItr(RGameInstance->GetWorld()); ActorItr; ++ActorItr)
	{
		//ActorItr.ClearCurrent();
		if (ActorItr->IsValidLowLevel())
		{
			ActorItr->Destroy();
		}
	}

}

AActor* FObjectHelper::GetOverlapTargets(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, EFriendshipTypeEnum friendshipType, float overlapScale)
{
	if (!IsValid(Source))
		return nullptr;

	auto Source_BaseProperty = Cast<IEntityBaseProperty>(Source);
	if (Source_BaseProperty == nullptr)
		return nullptr;

	UBattleEntityComponent* SourceBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Source);
	if (!IsValid(SourceBattleComp))
		return nullptr;

	float fMinDist = 100000.0f;
	AActor* NearestActor = nullptr;

	float SourceRadius = SourceBattleComp->BoundingRadius;
	for (auto& objectInstance : ObjectList)
	{
		if (objectInstance.Get() == Source)
			continue;
		auto Target_BaseProperty = Cast<IEntityBaseProperty>(objectInstance.Get());
		if (Target_BaseProperty == nullptr)
			continue;
		auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(objectInstance.Get());
		if (!IsValid(Target_BattleEntityComponent))
			continue;
		if (Target_BattleEntityComponent->IsDead() || Target_BattleEntityComponent->IsSearchSkip())
			continue;

		bool enemyTarget = URenewal_BattleFunctionLibrary::IsEnemy(Source_BaseProperty->GetPlayerUK(), Source_BaseProperty->GetGroupKey(),
			Target_BaseProperty->GetPlayerUK(), Target_BaseProperty->GetGroupKey());

		switch (friendshipType)
		{
		case EFriendshipTypeEnum::VE_Friend:
			if (enemyTarget)
				continue;
			break;
		case EFriendshipTypeEnum::VE_Enemy:
			if (!enemyTarget)
				continue;
			break;
		}

		float fUnitRadius = Target_BattleEntityComponent->BoundingRadius;
		AActor* TargetActor = objectInstance.Get();
		if (TargetActor)
		{
			FVector DistanceVector = Source->GetActorLocation() - TargetActor->GetActorLocation();
			float CurDistSquared = DistanceVector.SizeSquared();
			if (CurDistSquared <= (((fUnitRadius + SourceRadius) * (fUnitRadius + SourceRadius) * overlapScale)))
			{
				if (CurDistSquared < fMinDist)
				{
					fMinDist = CurDistSquared;
					NearestActor = objectInstance.Get();
				}
				outArray.Emplace(TargetActor);
			}
		}
	}
	return NearestActor;
}

AActor* FObjectHelper::GetOverlapTargetFromPosition(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, FVector SourceLocation, float overlapRange, EFriendshipTypeEnum friendshipType)
{
	float fMinDist = 100000.0f;
	AActor* NearestActor = nullptr;

	if (!IsValid(Source))
		return NearestActor;
	auto Source_BaseProperty = Cast<IEntityBaseProperty>(Source);
	if (Source_BaseProperty == nullptr)
		return NearestActor;

	for (auto& objectInstance : ObjectList)
	{
		if (objectInstance.Get() == Source)
			continue;
		auto Target_BaseProperty = Cast<IEntityBaseProperty>(objectInstance.Get());
		if (Target_BaseProperty == nullptr)
			continue;
		auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(objectInstance.Get());
		if (!IsValid(Target_BattleEntityComponent))
			continue;
		if (Target_BattleEntityComponent->IsDead() || Target_BattleEntityComponent->IsSearchSkip())
			continue;

		bool enemyTarget = URenewal_BattleFunctionLibrary::IsEnemy(Source_BaseProperty->GetPlayerUK(), Source_BaseProperty->GetGroupKey(),
			Target_BaseProperty->GetPlayerUK(), Target_BaseProperty->GetGroupKey());

		switch (friendshipType)
		{
		case EFriendshipTypeEnum::VE_Friend:
			if (enemyTarget)
				continue;
			break;
		case EFriendshipTypeEnum::VE_Enemy:
			if (!enemyTarget)
				continue;
			break;
		}

		float fUnitRadius = Target_BattleEntityComponent->BoundingRadius;
		AActor* TargetActor = objectInstance.Get();
		if (TargetActor)
		{
			FVector DistanceVector = SourceLocation - TargetActor->GetActorLocation();
			float CurDistSquared = DistanceVector.SizeSquared();
			if (CurDistSquared <= ((fUnitRadius + overlapRange) * (fUnitRadius + overlapRange)))
			{
				if (CurDistSquared < fMinDist)
				{
					fMinDist = CurDistSquared;
					NearestActor = objectInstance.Get();
				}
				outArray.Emplace(TargetActor);
			}
		}
	}
	return NearestActor;
}

void FObjectHelper::FindTargetFriendType(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, EFriendshipTypeEnum friendType)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

	outArray.Reset();
// 	if (!IsValid(RGameInstance))
// 		return;
	if (!IsValid(Source))
		return;
	auto Source_BaseProperty = Cast<IEntityBaseProperty>(Source);
	if (Source_BaseProperty == nullptr)
		return;

	if (EFriendshipTypeEnum::VE_FriendParty == friendType)
	{
		GetPartyList(outArray, Source);
		return;
	}

	for (auto& objectInstance : ObjectList)
	{
		if (!objectInstance.IsValid())
			continue;
		auto Target_BaseProperty = Cast<IEntityBaseProperty>(objectInstance.Get());
		if (Target_BaseProperty == nullptr)
			continue;
		auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(objectInstance.Get());
		if (!IsValid(Target_BattleEntityComponent) || Target_BattleEntityComponent->IsDead())
			continue;

		bool enemyTarget = URenewal_BattleFunctionLibrary::IsEnemy(Source_BaseProperty->GetPlayerUK(), Source_BaseProperty->GetGroupKey(),
			Target_BaseProperty->GetPlayerUK(), Target_BaseProperty->GetGroupKey());

		switch (friendType)
		{
		case EFriendshipTypeEnum::VE_Friend:
			if (!enemyTarget)
				outArray.Emplace(objectInstance.Get());
			break;
		case EFriendshipTypeEnum::VE_Enemy:
			if (enemyTarget)
				outArray.Emplace(objectInstance.Get());
			break;
		case EFriendshipTypeEnum::VE_FriendExceptMe:
			if (!enemyTarget && Source != objectInstance)
				outArray.Emplace(objectInstance.Get());
			break;
		}
	}
}

void FObjectHelper::FindTarget(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, FVector targetLocation, EFriendshipTypeEnum friendshipType, ESearchShapeTypeEnum applyType, FVector value0, float rotate, bool bDrawDebug)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

// 	if (!IsValid(RGameInstance))
// 		return;

	if (!IsValid(Source))
		return;

	auto Source_BaseProperty = Cast<IEntityBaseProperty>(Source);
	if (Source_BaseProperty == nullptr)
		return;

#if WITH_EDITOR
	if (bDrawDebug)
	{
		targetLocation.Z = Source->GetActorLocation().Z;
		ACharacter* Character = Cast<ACharacter>(Source);
		if (Character)
		{
			targetLocation.Z-=Character->GetDefaultHalfHeight();
		}
		float fLifeTime = 1.0f;
		switch (applyType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		{
			FRotator rotator = FRotator::ZeroRotator;
			rotator.Pitch = 90.0f;
			FMatrix matrix = FRotationTranslationMatrix::Make(rotator, targetLocation);
			DrawDebugCircle(Source->GetWorld(), matrix, value0.X, 32, FColorList::VioletRed, false, fLifeTime);
		}
		break;
		case ESearchShapeTypeEnum::VE_Sphere:
		{
			DrawDebugSphere(Source->GetWorld(), targetLocation, value0.X, 32, FColorList::VioletRed, false, fLifeTime);
		}
		break;
		case ESearchShapeTypeEnum::VE_Cylinder:
		{
			DrawDebugCylinder(Source->GetWorld(), targetLocation, targetLocation + FVector(0.0f, 0.0f, value0.Y), value0.X, 32, FColorList::VioletRed, false, fLifeTime);
		}
		break;
		case ESearchShapeTypeEnum::VE_Box:
		{
			FRotator rotator = Source->GetActorRotation();
			ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(Source);
			if (IsValid(BattleObject))
			{
				rotator = BattleObject->GetBattleObjectRotator();
			}
			rotator.Yaw += rotate;

			FVector forward = FRotationMatrix(rotator).GetScaledAxis(EAxis::X);
			FVector up = FRotationMatrix(rotator).GetScaledAxis(EAxis::Z);
			FVector right = FRotationMatrix(rotator).GetScaledAxis(EAxis::Y);

			//DrawDebugBox(Source->GetWorld(), targetLocation, FVector(value0.X * 0.5f, value0.Y * 0.5f, value0.Z * 0.5f), FQuat(rotator), FColorList::VioletRed, false, fLifeTime);
			DrawDebugBox(Source->GetWorld(), targetLocation + forward * value0.X * 0.5f, FVector(value0.X * 0.5f, value0.Y * 0.5f, 1.0f), FQuat(rotator), FColorList::Red, false, fLifeTime);

			
			/*DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + forward * value0.X * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation - forward * value0.X * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + right * value0.Y * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation - right * value0.Y * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + up * value0.Z * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation - up * value0.Z * 0.5f, FColorList::White, false, fLifeTime);*/
		}
		break;
		case ESearchShapeTypeEnum::VE_Box2D:
		{
			FRotator rotator = Source->GetActorRotation();
			ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(Source);
			if (IsValid(BattleObject))
			{
				rotator = BattleObject->GetBattleObjectRotator();
			}
			rotator.Yaw += rotate;
			DrawDebugBox(Source->GetWorld(), targetLocation, FVector(value0.X * 0.5f, value0.Y * 0.5f, 1.0f), FQuat(rotator), FColorList::VioletRed, false, fLifeTime);

			FVector forward = FRotationMatrix(rotator).GetScaledAxis(EAxis::X);
			FVector right = FRotationMatrix(rotator).GetScaledAxis(EAxis::Y);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + forward * value0.X * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation - forward * value0.X * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + right * value0.Y * 0.5f, FColorList::White, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation - right * value0.Y * 0.5f, FColorList::White, false, fLifeTime);
		}
		break;
		case ESearchShapeTypeEnum::VE_Sector:
		case ESearchShapeTypeEnum::VE_Sector2D:
		{
			FRotator rotator = FRotator::ZeroRotator;
			rotator.Yaw = rotate;

			UBattleEntityComponent* SourceBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Source);
			if (SourceBattleEntityComponent == nullptr)
				break;

			FVector forward = rotator.RotateVector(SourceBattleEntityComponent->GetForwardVector());
			ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(Source);
			if (IsValid(BattleObject))
			{
				forward = rotator.RotateVector(BattleObject->GetBattleObjectForward());
			}
			float fDist = value0.X;

			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + forward * fDist, FColorList::VioletRed, false, fLifeTime);

			rotator.Yaw = rotate + value0.Y * 0.5f;
			FVector left = rotator.RotateVector(SourceBattleEntityComponent->GetForwardVector());
			if (IsValid(BattleObject))
			{
				left = rotator.RotateVector(BattleObject->GetBattleObjectForward());
			}
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + left * fDist, FColorList::VioletRed, false, fLifeTime);

			rotator.Yaw = rotate - value0.Y * 0.5f;
			FVector right = rotator.RotateVector(SourceBattleEntityComponent->GetForwardVector());
			if (IsValid(BattleObject))
			{
				right = rotator.RotateVector(BattleObject->GetBattleObjectForward());
			}
			DrawDebugLine(Source->GetWorld(), targetLocation, targetLocation + right * fDist, FColorList::VioletRed, false, fLifeTime);

			DrawDebugLine(Source->GetWorld(), targetLocation + forward * fDist, targetLocation + left * fDist, FColorList::VioletRed, false, fLifeTime);
			DrawDebugLine(Source->GetWorld(), targetLocation + forward * fDist, targetLocation + right * fDist, FColorList::VioletRed, false, fLifeTime);
		}
		break;
		}
	}
#endif

	TArray< TWeakObjectPtr< AActor > > CandidateObjectList;

	switch (friendshipType)
	{
	case EFriendshipTypeEnum::VE_Friend:
	case EFriendshipTypeEnum::VE_FriendMagic:
	case EFriendshipTypeEnum::VE_FriendPhysical:
	case EFriendshipTypeEnum::VE_FriendExceptMe:
	case EFriendshipTypeEnum::VE_FriendLowestHP:
		GetFriendList(CandidateObjectList, Source);
		break;

	case EFriendshipTypeEnum::VE_FriendParty:
		GetPartyList(CandidateObjectList, Source);
		break;

	case EFriendshipTypeEnum::VE_Enemy:
	case EFriendshipTypeEnum::VE_EnemyLowestHP:
		GetEnemyList(CandidateObjectList, Source);
		break;

	case EFriendshipTypeEnum::VE_MySummon:
		GetMySummonList(CandidateObjectList, Source);
		break;
	case EFriendshipTypeEnum::VE_SummonFriend:
		GetFriendSummonList(CandidateObjectList);
		break;
	}

	for (auto& objectInstance : CandidateObjectList)
	{
		auto Target_BaseProperty = Cast<IEntityBaseProperty>(objectInstance.Get());
		if (Target_BaseProperty == nullptr)
			continue;
		if (Target_BaseProperty->IsReady() == false)
			continue;
		auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(objectInstance.Get());
		if (!IsValid(Target_BattleEntityComponent))
			continue;
		if (Target_BattleEntityComponent->IsDead() || Target_BattleEntityComponent->IsSearchSkip())
			continue;

		bool enemyTarget = URenewal_BattleFunctionLibrary::IsEnemy(Source_BaseProperty->GetPlayerUK(), Source_BaseProperty->GetGroupKey(),
			Target_BaseProperty->GetPlayerUK(), Target_BaseProperty->GetGroupKey());

		switch (friendshipType)
		{
		case EFriendshipTypeEnum::VE_Friend:
			if (enemyTarget)
				continue;
			break;
		case EFriendshipTypeEnum::VE_FriendMagic:
			if (enemyTarget)
				continue;
			if (Target_BattleEntityComponent->GetBattleInfo()->GetCharacterProperty<ECharacterPropertyTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackPropertyType) != ECharacterPropertyTypeEnum::VE_MagicAttackPower)
				continue;
			break;
		case EFriendshipTypeEnum::VE_FriendPhysical:
			if (enemyTarget)
				continue;
			if (Target_BattleEntityComponent->GetBattleInfo()->GetCharacterProperty<ECharacterPropertyTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackPropertyType) != ECharacterPropertyTypeEnum::VE_PhysicalAttackPower)
				continue;
			break;
		case EFriendshipTypeEnum::VE_FriendExceptMe:
			if (enemyTarget)
				continue;

			if (objectInstance == Source)
				continue;

			break;
		case EFriendshipTypeEnum::VE_Enemy:
			if (!enemyTarget)
				continue;
			break;

		case EFriendshipTypeEnum::VE_FriendParty:
		case EFriendshipTypeEnum::VE_MySummon:
			if (enemyTarget)
				continue;
			break;
		}

		AActor* Debug = objectInstance.Get();
		float fUnitRadius = Target_BattleEntityComponent->GetBattleInfo()->CharacterProperty.UnitRadius;

		switch (applyType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		case ESearchShapeTypeEnum::VE_Sphere:
		case ESearchShapeTypeEnum::VE_Cylinder:
		{
			float fDist = value0.X;
			// Apply target bounding raduis
			FVector diff = objectInstance->GetActorLocation() - targetLocation;
			float heightDiff = FMath::Abs(diff.Z);
			if (applyType == ESearchShapeTypeEnum::VE_Cylinder)
			{
				if (heightDiff >= value0.Y)
					continue;
			}
			if (applyType == ESearchShapeTypeEnum::VE_Circle || applyType == ESearchShapeTypeEnum::VE_Cylinder)
				diff.Z = 0.0f;

			if (diff.SizeSquared() <= (fDist + fUnitRadius) * (fDist + fUnitRadius))
			{
				// check floor..( denied... because actor can search between difference floor.. )
				//if( URenewal_BattleFunctionLibrary::IsSameFloor( objectInstance.Get(), heightDiff, FLOOR_DIFF_DIST ) )
				outArray.Emplace(objectInstance.Get());
			}
		}
		break;
		case ESearchShapeTypeEnum::VE_Box2D:
		case ESearchShapeTypeEnum::VE_Box:
		{
			FVector curPos = objectInstance->GetActorLocation();
			FVector localVector = curPos - targetLocation;
			float heightDiff = FMath::Abs(localVector.Z);
			localVector.Z = 0.0f;

			float minX = -value0.X * 0.5f;
			float maxX = value0.X * 0.5f;
			float minY = -value0.Y * 0.5f;
			float maxY = value0.Y * 0.5f;
			
			FRotator actorRotator = FRotator::ZeroRotator;
			ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(Source);
			if (IsValid(BattleObject))
			{
				actorRotator = BattleObject->GetBattleObjectRotator();
			}

			FVector Dir = targetLocation - Source->GetActorLocation();
			Dir = Dir.GetSafeNormal2D();

			float Angle = UUtilFunctionLibrary::GetBetweenDegree(FVector(1.f, 0.f, 0.f), Dir);
			actorRotator.Yaw = Angle;


			actorRotator.Yaw += rotate;

			localVector = actorRotator.UnrotateVector(localVector);

			if ((localVector.X - fUnitRadius) > maxX || (localVector.X + fUnitRadius) < minX)
				continue;
			if ((localVector.Y - fUnitRadius) > maxY || (localVector.Y + fUnitRadius) < minY)
				continue;

			outArray.Emplace(objectInstance.Get());
		}
		break;
		case ESearchShapeTypeEnum::VE_Sector:
		case ESearchShapeTypeEnum::VE_Sector2D:
		{
			float fDistSquared = (fUnitRadius + value0.X) * (value0.X + fUnitRadius);
			FVector diff = objectInstance->GetActorLocation() - Source->GetActorLocation();
			float heightDiff = FMath::Abs(diff.Z);
			if (applyType == ESearchShapeTypeEnum::VE_Sector)
			{
				if (heightDiff > value0.Z)
					continue;
			}

			diff.Z = 0.0f;
			if (diff.SizeSquared() <= fDistSquared)
			{
				FRotator rotator = FRotator::ZeroRotator;
				rotator.Yaw = rotate;

				UBattleEntityComponent* SourceBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Source);
				if (SourceBattleEntityComponent == nullptr)
					break;

				FVector Direction = targetLocation - Source->GetActorLocation();
				if (Direction.IsNearlyZero())
					Direction = SourceBattleEntityComponent->GetForwardVector();
				else
					rotator.Yaw = rotate;

				Direction = Direction.GetSafeNormal2D();

				FVector forward = rotator.RotateVector(Direction);
				ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(Source);
				if (IsValid(BattleObject))
				{
					forward = rotator.RotateVector(BattleObject->GetBattleObjectForward());
				}

				float fValue = FVector::DotProduct(forward, diff.GetSafeNormal());
				float fDegree = FMath::RadiansToDegrees(FMath::Acos(fValue));

				if (fDegree <= value0.Y * 0.5f)
				{
					//if (URenewal_BattleFunctionLibrary::IsSameFloor(objectInstance.Get(), heightDiff, FLOOR_DIFF_DIST))
					outArray.Emplace(objectInstance.Get());
				}
				else
				{
					// Check Bounding raduis
					rotator = FRotator::ZeroRotator;
					rotator.Yaw = rotate + value0.Y * 0.5f;
					FVector left = rotator.RotateVector(Direction);
					if (IsValid(BattleObject))
					{
						left = rotator.RotateVector(BattleObject->GetBattleObjectForward());
					}

					if (FMath::LineSphereIntersection(Source->GetActorLocation(), left, 10000.0f, targetLocation, fUnitRadius))
					{
						//if (URenewal_BattleFunctionLibrary::IsSameFloor(objectInstance.Get(), heightDiff, FLOOR_DIFF_DIST))
						outArray.Emplace(objectInstance.Get());
						continue;
					}

					rotator.Yaw = rotate - value0.Y * 0.5f;
					FVector right = rotator.RotateVector(Direction);
					if (IsValid(BattleObject))
					{
						right = rotator.RotateVector(BattleObject->GetBattleObjectForward());
					}

					if (FMath::LineSphereIntersection(Source->GetActorLocation(), right, 10000.0f, targetLocation, fUnitRadius))
					{
						//if (URenewal_BattleFunctionLibrary::IsSameFloor(objectInstance.Get(), heightDiff, FLOOR_DIFF_DIST))
						outArray.Emplace(objectInstance.Get());
						continue;
					}
				}
			}
		}
		break;
		}
	}
}

void FObjectHelper::SetAllObjectVisible(bool IsVisible, TArray< AActor* > IgnoreObject /*= nullptr*/)
{
	AllObjectVisible = IsVisible;

	for (auto comp : ObjectList)
	{
		if (IgnoreObject.Num() > 0 && IgnoreObject.Contains(comp.Get()))
		{
			continue;
		}

		UEntityRenderComponent* RenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(comp.Get());
		if (IsValid(RenderComp))
			RenderComp->SetVisible(IsVisible);
		UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(comp.Get());
		if (IsValid(AIComp) && IsVisible == false)
		{
			AIComp->StopMove();
			AIComp->AIStop();
			AIComp->SetForcePause(true);
		}
		else
		{
			if (IsValid(AIComp))
				AIComp->AIRestart();
		}
	}

	for (auto comp : BulletList)
	{
		if (comp.IsValid())
		{
			comp->SetActorHiddenInGame(!(IsVisible));
		}
	}
	//Skill
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

	TArray<AActor* > arraylist;
	UGameplayStatics::GetAllActorsOfClass(RGameInstance->GetWorld(), AContinuousSkillApplierActor::StaticClass(), arraylist);
	for (auto Skill : arraylist)
	{
		if (IsValid(Skill))
		{
			auto ConSkill = Cast<AContinuousSkillApplierActor>(Skill);
			if (ConSkill && ConSkill->SkillEffect)
			{
				ConSkill->SkillEffect->SetHiddenInGame(!(IsVisible));
			}
		}
	}
}

void FObjectHelper::SetAllObjectPause(float SlowTime, float SlowRate, AActor* IgnoreObject)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

// 	if (!IsValid(RGameInstance))
// 		return;

	if (SlowTime <= 0.0f)
	{
		return;
	}

	if (IsValid(IgnoreObject))
	{
		if (!IgnorePauseList.Contains(IgnoreObject))
		{
			IgnorePauseList.Emplace(IgnoreObject);
		}

		UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(IgnoreObject);
		if (IsValid(AIComp))
			AIComp->SetForcePause(false);

		if (SlowRate > 0.0f)
		{
			IgnoreObject->CustomTimeDilation = SlowRate;
		}
	}

	for (auto comp : ObjectList)
	{
		if (comp.IsValid())
		{
			if (!IgnorePauseList.Contains(comp.Get()))
			{
				UEntityAIComponent* AIComp1 = UCharacterHelpFunctionLibrary::GetEntityAIComponent(comp.Get());
				if (IsValid(AIComp1))
					AIComp1->SetForcePause(true, SlowTime);
			}
		}
	}

	for (auto comp : BulletList)
	{
		if (comp.IsValid())
		{
			comp->SetForcePause(true, SlowTime);
		}
	}
}

void FObjectHelper::SetAllObjectBreak(bool InIsNotBreak /*= false*/, bool InPauseDamage /*= false*/)
{
	IgnorePauseList.Empty();

	for (auto comp : ObjectList)
	{
		if (comp.IsValid())
		{
			UEntityAIComponent* AIComp1 = UCharacterHelpFunctionLibrary::GetEntityAIComponent(comp.Get());
			if (IsValid(AIComp1))
				AIComp1->SetForcePause(InIsNotBreak);

			auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(comp.Get());
			if (Target_BattleEntityComponent)
			{
				Target_BattleEntityComponent->SetIsPauseState(InPauseDamage);
			}
		}
	}

	for (auto comp : BulletList)
	{
		if (comp.IsValid())
		{
			comp->SetForcePause(InIsNotBreak);
		}
	}
}

void FObjectHelper::SetAllAIStop()
{
	for (size_t i = 0; i < ObjectList.Num(); ++i)
	{
		if (ObjectList.IsValidIndex(i) && ObjectList[i].IsValid())
		{
			UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(ObjectList[i].Get());
			if (AIComp)
			{
				AIComp->AIStop();
			}
		}
	}
}

void FObjectHelper::CheckIgnoreForcePauseTimeEnd(AActor* obj)
{
	if (IsValid(obj))
	{
		obj->CustomTimeDilation = 1.0f;

		if (IgnorePauseList.Contains(obj))
		{
			IgnorePauseList.Remove(obj);
		}
	}

	if (IgnorePauseList.Num())
	{
		auto lastObj = IgnorePauseList.Last();

		if (IsValid(obj) && lastObj.IsValid())
		{
			UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(lastObj.Get());
			if (IsValid(AIComp))
			{
				float remainTime = AIComp->GetIgnoreForcePauseTime();
				if (remainTime > 0.0f)
				{
					AIComp->SetForcePause(true, remainTime);
				}
			}
		}
	}
}

void FObjectHelper::SetAllObjectBlackOut(AActor* IgnoreObject)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

// 	if (!IsValid(RGameInstance))
// 		return;

	if (IsValid(IgnoreObject))
	{
		if (!IgnoreBlackOutList.Contains(IgnoreObject))
		{
			IgnoreBlackOutList.Emplace(IgnoreObject);
		}

		URenderFunctionLibrary::IgnoreDarkenTarget(IgnoreObject, true);
	}

	SetBlackOut(true);
}

void FObjectHelper::CheckIgnoreBlackOutTimeEnd(AActor* obj)
{
	if (IsValid(obj))
	{
		if (IgnoreBlackOutList.Contains(obj))
		{
			IgnoreBlackOutList.Remove(obj);
		}

		URenderFunctionLibrary::IgnoreDarkenTarget(obj, false);
	}

	if (IgnoreBlackOutList.Num() == 0)
	{
		SetBlackOut(false);
	}
}

void FObjectHelper::SetBlackOut(bool bEnable)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

// 	if (!IsValid(RGameInstance))
// 		return;

	URenderFunctionLibrary::SetDarken(bEnable);
}

void FObjectHelper::SetAllObjectHPBarVisible(bool IsVisible)
{
	for (auto comp : ObjectList)
	{
		if (comp.IsValid())
		{
			AActor* Actor = comp.Get();

			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Actor);
			if (EntityRenderComponent)
			{
#ifdef RULE_TESTCHANGE
				EntityRenderComponent->SetActiveWidgetComp(IsVisible);
#else
				UWidgetDecoratorComponent* HpBarWidgetComponent = EntityRenderComponent->GetHpBarWidgetComponent();
				if (IsValid(HpBarWidgetComponent))
				{
					HpBarWidgetComponent->SetActiveWidgetDecorator(IsVisible);
				}
#endif
			}
		}
	}
}

void FObjectHelper::HpBarWidgetDestroy()
{
	for (auto comp : ObjectList)
	{
		if (comp.IsValid() == false)
			continue;

		UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(comp.Get());
		if (EntityRenderComponent)
		{
			EntityRenderComponent->HpBarWidgetDestroy();
		}
	}
}

void FObjectHelper::SetAssetResource(TArray<FSoftObjectPath>& InAssets, FName& InHeroID, bool InNpc/* = false */)
{
	FCharacterTableInfo* CharacterInfo;
	if (InNpc)
		CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(InHeroID);
	else
		CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InHeroID);

	if (CharacterInfo == nullptr)
		return;

	if(!CharacterInfo->HeroClass.IsNull())
		InAssets.AddUnique(CharacterInfo->HeroClass.ToSoftObjectPath());
	if (!CharacterInfo->MeshMaterial.IsNull())
		InAssets.AddUnique(CharacterInfo->MeshMaterial.ToSoftObjectPath());
	if (!CharacterInfo->MeshMaterial2.IsNull())
		InAssets.AddUnique(CharacterInfo->MeshMaterial2.ToSoftObjectPath());
	if (!CharacterInfo->DecoParticle.IsNull())
		InAssets.AddUnique(CharacterInfo->DecoParticle.ToSoftObjectPath());
	if (!CharacterInfo->DecoMesh.IsNull())
		InAssets.AddUnique(CharacterInfo->DecoMesh.ToSoftObjectPath());
	if (!CharacterInfo->RWeaponType.IsNull())
		InAssets.AddUnique(CharacterInfo->RWeaponType.ToSoftObjectPath());
	if (!CharacterInfo->LWeaponType.IsNull())
		InAssets.AddUnique(CharacterInfo->LWeaponType.ToSoftObjectPath());
	
	//Spawn Sequence
	if (!CharacterInfo->SpawnSequence.IsNone())
	{
		FResourceSequenceTableInfo* SequenceTableInfo = UTableManager::GetInstancePtr()->GetResourceSequenceRow(CharacterInfo->SpawnSequence);
		if (SequenceTableInfo)
		{
			InAssets.AddUnique(SequenceTableInfo->SequenceClass.ToSoftObjectPath());

			//if (SequenceTableInfo->SequenceClass.ToString().Contains(TEXT("_C")))
			//{
			//	FString path = SequenceTableInfo->SequenceClass.ToString();
			//	path = path.LeftChop(2);

			//	FSoftObjectPath SoftObjectPath;
			//	SoftObjectPath.SetPath(path);

			//	InAssets.AddUnique(SoftObjectPath);
			//}

		}
			
	}

	//HitEffect
	FResourceFXTableInfo* HitFXTable = UTableManager::GetInstancePtr()->GetResourceFXRow(CharacterInfo->HitEffect);
	if (HitFXTable)
	{
		if (HitFXTable != nullptr && !HitFXTable->Particle.IsNull())
			InAssets.AddUnique(HitFXTable->Particle.ToSoftObjectPath());
		if (HitFXTable != nullptr && !HitFXTable->Sound.IsNull())
			InAssets.AddUnique(HitFXTable->Sound.ToSoftObjectPath());
	}
		

	//	Bullet 관련
	FBulletTableInfo* BulletTable = UTableManager::GetInstancePtr()->GetBulletRow(CharacterInfo->BulletID);
	if (BulletTable != nullptr)
	{
		if(!BulletTable->BulletClass.IsNull())
			InAssets.AddUnique(BulletTable->BulletClass.ToSoftObjectPath());
		
		FResourceFXTableInfo* BulletFXTable = UTableManager::GetInstancePtr()->GetResourceFXRow(BulletTable->BulletHitEffect);
		if (BulletFXTable != nullptr && !BulletFXTable->Particle.IsNull())
			InAssets.AddUnique(BulletFXTable->Particle.ToSoftObjectPath());
		if (BulletFXTable != nullptr && !BulletFXTable->Sound.IsNull())
			InAssets.AddUnique(BulletFXTable->Sound.ToSoftObjectPath());
	}

	//	Skill 관련
	FHERO* CurHeroInfo = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithID(InHeroID);
	TArray<int32>		skills = CurHeroInfo == nullptr ? TArray<int32>() : CurHeroInfo->skills;

	TArray<FName>	SkillID;
	if (!CharacterInfo->ActiveSkill01.IsNone())
	{
		FString skillstr = CharacterInfo->ActiveSkill01.ToString();
		if(!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_ActiveSkill01) ? skills[(int32)ESkillCategoryTypeEnum::VE_ActiveSkill01] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}
	if (!CharacterInfo->ActiveSkill02.IsNone())
	{
		FString skillstr = CharacterInfo->ActiveSkill02.ToString();
		if(!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_ActiveSkill02) ? skills[(int32)ESkillCategoryTypeEnum::VE_ActiveSkill02] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}
	if (!CharacterInfo->ActiveSkill03.IsNone())
	{
		FString skillstr = CharacterInfo->ActiveSkill03.ToString();
		if(!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_ActiveSkill03) ? skills[(int32)ESkillCategoryTypeEnum::VE_ActiveSkill03] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}
	if (!CharacterInfo->PassiveSkill01.IsNone())
	{
		FString skillstr = CharacterInfo->PassiveSkill01.ToString();
		if(!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_PassiveSkill01) ? skills[(int32)ESkillCategoryTypeEnum::VE_PassiveSkill01] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}

	for (int32 i = 0; i < SkillID.Num(); i++)
	{
		if (SkillID.IsValidIndex(i) == false)
			continue;
		if (SkillID[i].IsNone() || SkillID[i].IsValid() == false)
			continue;

		FSkillTableInfo_V2* SkillInfo_V2 = nullptr;
		SkillInfo_V2 = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillID[i]);

		if (SkillInfo_V2 == nullptr)
			continue;

		auto Recursive_IfSummon = [=, &InAssets](ESkillApplierTypeEnum InType, FName SummonID) {
			if (((uint8)InType >= (uint8)ESkillApplierTypeEnum::VE_i_summon_1Max1) &&
				((uint8)InType <= (uint8)ESkillApplierTypeEnum::VE_i_summon_Once5))
			{
				SetAssetResource(InAssets, SummonID, true);
			}
		};

		Recursive_IfSummon(SkillInfo_V2->TargetEffect1, FName(*FString::FromInt(SkillInfo_V2->TargetValue1)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect2, FName(*FString::FromInt(SkillInfo_V2->TargetValue2)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect3, FName(*FString::FromInt(SkillInfo_V2->TargetValue3)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect4, FName(*FString::FromInt(SkillInfo_V2->TargetValue4)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect5, FName(*FString::FromInt(SkillInfo_V2->TargetValue5)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect6, FName(*FString::FromInt(SkillInfo_V2->TargetValue6)));


		FSkillResourceTableInfo* SkillResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo_V2->SkillResourceID);
		if (SkillResourceInfo == nullptr)
			continue;

		if (!SkillResourceInfo->CastingEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->CastingEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->CastingBeamEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->CastingBeamEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->ActionEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->ActionEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->AreaEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->AreaEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->ScreenEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->ScreenEffect.ToSoftObjectPath());

		FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
		if (BulletTableInfo != nullptr)
		{
			if (!BulletTableInfo->BulletClass.IsNull())
				InAssets.AddUnique(BulletTableInfo->BulletClass.ToSoftObjectPath());
		}
			

		FResourceFXTableInfo* SkillResource = UTableManager::GetInstancePtr()->GetResourceFXRow(SkillResourceInfo->HitEffect);
		if (SkillResource == nullptr)
			continue;

		if (!SkillResource->Particle.IsNull())
			InAssets.AddUnique(SkillResource->Particle.ToSoftObjectPath());
		if (!SkillResource->Sound.IsNull())
			InAssets.AddUnique(SkillResource->Sound.ToSoftObjectPath());
		
	}
	//	Skill 관련

	// Animation Particle
	TSubclassOf<ABattleBaseCharacter> HeroBPClass = CharacterInfo->GetHeroBlueprintClass();
	AActor* Character_CDO = HeroBPClass->GetDefaultObject<AActor>();
	if (Character_CDO)
	{
		USkeletalMeshComponent* SkelComp = Character_CDO->FindComponentByClass<USkeletalMeshComponent>();
		if (SkelComp && SkelComp->AnimClass)
		{
			TSubclassOf<UAnimInstance> AnimInstanceClass = SkelComp->AnimClass;
			UBattleBaseAnimInstance* BattleAnimInst_CDO = AnimInstanceClass->GetDefaultObject<UBattleBaseAnimInstance>();

			if (BattleAnimInst_CDO)
			{
				if (!BattleAnimInst_CDO->AnimMontageIdle.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageIdle.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageRun.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageRun.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageAttack.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageAttack.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageAbnormal.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageAbnormal.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageEvent.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageEvent.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageDamageResult.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageDamageResult.ToSoftObjectPath());

				auto AddAssets = [&InAssets](TAssetPtr<UAnimMontage> InAsset) {
					UAnimMontage* AttackAnim = UUtilFunctionLibrary::GetAnimMontageFromAssetID(InAsset);
					if (AttackAnim)
					{
						for (auto Element : AttackAnim->Notifies)
						{
							UAttachResourceFXNotify* FXNoti = Cast<UAttachResourceFXNotify>(Element.Notify);
							if (FXNoti)
							{
								FResourceFXTableInfo* info = UTableManager::GetInstancePtr()->GetResourceFXRow(FXNoti->FXKey);
								if (info)
								{
									if (!info->Particle.IsNull())
										InAssets.AddUnique(info->Particle.ToSoftObjectPath());
									if (!info->Sound.IsNull())
										InAssets.AddUnique(info->Sound.ToSoftObjectPath());
								}
							}
						}
					}
				};

				AddAssets(BattleAnimInst_CDO->AnimMontageAttack);
				
				for (auto Element : BattleAnimInst_CDO->AnimMontageSkillMap)
					AddAssets(Element.Value);
				
			}

			
		}
	}
	
	
}

void FObjectHelper::SetAssetResource_FromTables(TArray<FSoftObjectPath>& InAssets)
{
	UTableManager* TableManager = UTableManager::GetInstancePtr();
	if (TableManager)
	{
		TArray<FName> AbnormalRows = TableManager->GetAbnormalTableInfoRowNames();
		for (size_t i = 0 ; i < AbnormalRows.Num() ; ++i)
		{
			FAbnormalStateGroupTableInfo_V2* tableinfo = TableManager->GetAbnormalStateGroupRow_V2(AbnormalRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Effect.IsNull())
					InAssets.AddUnique(tableinfo->Effect.ToSoftObjectPath());
				if (!tableinfo->EffectMaterial.IsNull())
					InAssets.AddUnique(tableinfo->EffectMaterial.ToSoftObjectPath());
			}
		}
		
		TArray<FName> CastingRows = TableManager->GetSkillCastingTableInfoRowNames();
		for (size_t i = 0; i < CastingRows.Num(); ++i)
		{
			FSkillCastingEffectTableInfo* tableinfo = TableManager->GetSkillCastingEffectRow(CastingRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->SkillCastingDecal.IsNull())
					InAssets.AddUnique(tableinfo->SkillCastingDecal.ToSoftObjectPath());
				if (!tableinfo->ParticleSystem.IsNull())
					InAssets.AddUnique(tableinfo->ParticleSystem.ToSoftObjectPath());
			}
		}

		TArray<FName> MaterialRows = TableManager->GetResourceMaterialRowNames();
		for (size_t i = 0; i < MaterialRows.Num(); ++i)
		{
			FResourceMaterialTableInfo* tableinfo = TableManager->GetResourceMaterialRow(MaterialRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}

		TArray<FName> ParticleRows = TableManager->GetResourceParticleRowNames();
		for (size_t i = 0; i < ParticleRows.Num(); ++i)
		{
			FResourceParticleTableInfo* tableinfo = TableManager->GetResourceParticleRow(ParticleRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}

		TArray<FName> DamageFloaterTexRows = TableManager->GetResourceDamageFloater2DRowNames();
		for (size_t i = 0; i < DamageFloaterTexRows.Num(); ++i)
		{
			FResourceTexture2DTableInfo* tableinfo = TableManager->GetResourceDamageFloaterTex2DRow(DamageFloaterTexRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}

		TArray<FName> StaticMeshRows = TableManager->GetResourceStaticMeshRowNames();
		for (size_t i = 0; i < StaticMeshRows.Num(); ++i)
		{
			FResourceStaticMeshTableInfo* tableinfo = TableManager->GetResourceStaticMeshRow(StaticMeshRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}
		

		TArray<FName> DropRows = TableManager->GetDropItemRowNames();
		for (size_t i = 0; i < DropRows.Num(); ++i)
		{
			FDropItemResourceInfo* tableinfo = TableManager->GetDropItemResourceRow(DropRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->SkeletalMesh.IsNull())
					InAssets.AddUnique(tableinfo->SkeletalMesh.ToSoftObjectPath());

				if (!tableinfo->Animation_Drop.IsNull())
					InAssets.AddUnique(tableinfo->Animation_Drop.ToSoftObjectPath());

				if (!tableinfo->Animation_Idle.IsNull())
					InAssets.AddUnique(tableinfo->Animation_Idle.ToSoftObjectPath());
			}
		}
		
	}
}
