// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AutoPlayManager.h"
#include "RGameInstance.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"


// #include "Utils/AutoSpawner.h"





#include "ProjectRGameMode.h"

#include "GameInfo/PlayStateInfo.h"
#include "Object/SpawnerTransform.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/WaypointComponent.h"
#include "Runtime/Engine/Classes/Matinee/MatineeActor.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "LeaderControlHelper.h"
//#define DEBUG_DRAW



void UAutoPlayManager::Initialize()
{ 
	NextWayPointTransform.ID = 0xffff;
	NextWayPointTransform.Position = FVector::ZeroVector;
	NextWayPointTransform.Rotation = FRotator::ZeroRotator;
	CurrentAutoPlayMode = EAutoplayModeTypeEnum::VE_None;
	AutoPlayCount = 0;

	/*AutoSanwer = NewObject<UAutoSpawner>(this);
	AutoSanwer->Init();*/
}

void UAutoPlayManager::ActivateAutoPlay(EAutoplayModeTypeEnum InAutoPlayModeType, int32 count)
{
	CurrentAutoPlayMode = InAutoPlayModeType;
	AutoPlayCount = count;
}

EAutoplayModeTypeEnum UAutoPlayManager::UseAutoPlay()
{
	DecreaseAutoPlayModeCount();
	return CurrentAutoPlayMode;
}

void UAutoPlayManager::DecreaseAutoPlayModeCount()
{
	--AutoPlayCount;
	if (AutoPlayCount <= 0)
	{
		AutoPlayCount = 0;
		CurrentAutoPlayMode = EAutoplayModeTypeEnum::VE_None;
	}
}

EAutoplayModeTypeEnum UAutoPlayManager::GetAutoPlayMode()
{ 	
	return CurrentAutoPlayMode; 
}

void UAutoPlayManager::OnEnterStateSelect(FName DeckID)
{

}

// void UAutoPlayManager::OnReadyToStart()
// {
// 	if (CurrentAutoPlayMode != EAutoplayModeTypeEnum::VE_Campaign)
// 		return;
// 
// 	if (IsValid(GameInstance) == false)
// 		return;
// 
// 	FVector SpawnLocation;
// 
// 	auto gameMode = UGameplayStatics::GetGameMode(GameInstance);
// 	if (IsValid(gameMode))
// 	{
// 		AProjectRGameMode* ProjectGameMode = Cast<AProjectRGameMode>(gameMode);
// 		if (IsValid(ProjectGameMode))
// 		{
// 			UIGamePlayMode* GamePlayMode = Cast<UIGamePlayMode>(ProjectGameMode->GetGamePlayMode());
// 			if (IsValid(GamePlayMode))
// 			{
// 				AStageBase* StageBase = GamePlayMode->GetStageSpawner();
// 				if (StageBase)
// 				{
// 					FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(UGamePlayBluePrintFunction::GetBaseStageInfo()->SelectedMapID);
// 					if (MapTableInfo)
// 					{
// 						FSpawnerTransformInfo* SpanwTransform = StageBase->GetSpawnerTransform(MapTableInfo->StartPos);
// 						if (SpanwTransform)
// 							SpawnLocation = SpanwTransform->Position;
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
// 	if (IsValid(BasePlayerController))
// 	{
// 		FRotator MapRot(0, -90, 0);
// 		auto PlayStateInfo = GameInstance->PlayStateInfo;
// 		AActor* SpawnedCharacter = nullptr;
// 		for (int32 i = 0; i < COMMANDSLOT_MAX; i++)
// 		{
// 			ESpawnResultTypeEnum CanSpawnHeroResult = PlayStateInfo->CanSpawnHero(i);
// 			// Spawn new hero
// 			if (CanSpawnHeroResult == ESpawnResultTypeEnum::VE_Success)
// 			{
// 				if (PlayStateInfo->IsSpawnDelayTimer())
// 				{
// 					UUtilFunctionLibrary::ShowIndicator(this->GetWorld(), FText::FromString(TEXT("Cooling time is remaining")));
// 					return;
// 				}
// 
// 				SpawnedCharacter = BasePlayerController->SpawnHeroAtLocation(i, SpawnLocation, MapRot);
// 
// // 				if (IsValid(SpawnedCharacter))
// // 				{
// // 					PlayStateInfo->SetSpawnedCharacter(i, SpawnedCharacter);
// // 					GameInstance->EventManager->OnSpawnCharacterDelegate.Broadcast(i, SpawnedCharacter);
// // 				}
// 			}
// 		}		
// 	}	
// }


void UAutoPlayManager::OnCompleteCampaignResult()
{
	
}

//void UAutoPlayManager::InGameProcessAutoPlay()
//{
//	URGameInstance* GameInstance = RGAMEINSTANCE(this);
//	checkf(GameInstance != nullptr, TEXT("RGameInstance is Empty"));
//	if(GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->Tags.Contains( FName(TEXT("BGSFX") ) ) == false )
//		return;
//
//	AlreadyAllSpawned = 
//}

FVector	UAutoPlayManager::GetValidSpawnPointNearByControlledHero()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("GameInstance is Empty"));

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
	
	if (IsValid(BasePlayerController) == false )
	{
		return FVector::ZeroVector;
	}

	AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
	if (IsValid(SelectedCharacter) == false)
	{
		return FVector::ZeroVector;
	}
		
	auto BattleInfo = UCharacterHelpFunctionLibrary::GetBattleInfo(SelectedCharacter);
	if (IsValid(BattleInfo) == false)
	{
		return FVector::ZeroVector;
	}

	auto SelectedCharacterAttackType = BattleInfo->CharacterProperty.AttackType;

	return FVector::ZeroVector;
}

void UAutoPlayManager::SetNextWayPointTransform(FSpawnerTransformInfo InSpawnerTransform)
{
	NextWayPointTransform = InSpawnerTransform; 
	
}

void UAutoPlayManager::SetResetNextWayPoint(FSpawnerTransformInfo InSpawnerTransform)
{
	NextWayPointTransform = InSpawnerTransform;
}

void UAutoPlayManager::ClearWayPoint()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("GameInstance is Empty"));
	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
	if (IsValid(BasePlayerController))
	{
		AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SelectedCharacter);
		UWaypointComponent* WaypointComponent = EntityAIComponent->GetWaypointComponent();
		if (WaypointComponent == nullptr)
		{
			EntityAIComponent->AttachWaypointComponent();
			WaypointComponent = EntityAIComponent->GetWaypointComponent();
		}
		WaypointComponent->ClearWaypoint();
		EntityAIComponent->AIStop();
	}
}

/*
FVector	UAutoPlayManager::GetValidSpawnPointNearByControlledHero()
{
	if (IsValid(GameInstance) == false)
		return FVector::ZeroVector;

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
	if (IsValid(BasePlayerController) )
	{
		AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
		if (IsValid(SelectedCharacter))
		{
			if (IsValid(GameInstance->GridMapManager))
			{
				FCellIndex SrcCellIdx;
				if(GameInstance->GridMapManager->GetCellIndexFromWorldPos(SelectedCharacter->GetActorLocation(), SrcCellIdx) == false)
					return SelectedCharacter->GetActorLocation();
#ifdef DEBUG_DRAW
				DrawDebugPoint(GameInstance->GetWorld(), SelectedCharacter->GetActorLocation(), 10, FColor::Orange, false, 10.0f);
#endif // DEBUG_DRAW

				FCellIndex AdjacentCell;
				if (UMapFunctionLibrary::GetAdjacentCellNearestWithPivotCellnOnNavyMesh(GameInstance->GetWorld(), SrcCellIdx, AdjacentCell, 2, 5) == true)
				{
					FVector OutWorldPos;
					GameInstance->GridMapManager->GetWorldPosFromCellIndex(AdjacentCell, OutWorldPos );

					return OutWorldPos;
				}
				else
				{
					return SelectedCharacter->GetActorLocation();
				}


// 				TArray< FCellIndex > AdjacentCells;
// 				GameInstance->GridMapManager->GetAdjacentCells(AdjacentCells, SrcCellIdx, ENUM_TO_BITFIELD((int32)EMapCellOffsetDepthEnum::VE_Depth1), ENUM_TO_BITFIELD((int32)EMapCellStateEnum::VE_Movable ) ) ;
// 				if (AdjacentCells.Num() > 0)
// 				{
// 					FVector OutWorldPos;
// 					GameInstance->GridMapManager->GetWorldPosFromCellIndex(AdjacentCells[0], OutWorldPos );
// 					return OutWorldPos;
// 				}
			}
			else
			{
				return SelectedCharacter->GetActorLocation();
			}
		}		
	}

	return FVector::ZeroVector;
}
*/