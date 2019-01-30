// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"



#include "Character/Components/FunctionalComponents/PathFindingComponent.h"
#include "UtilFunctionIntegrated.h"
#include "MapFunctionLibrary.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"
#include "Runtime/Engine/Classes/AI/Navigation/RecastNavMesh.h"

//#include "Runtime/Landscape/Classes/Landscape.h"

//#define		TESTCOW
//#define	DRAW_DEBUG_INFO
//#define DRAW_DEBUG

// bool UMapFunctionLibrary::GetNearestPositionAroundPivotPositionWithTarget(AActor* PivotOwner, AActor* TargetOwner, FVector& PivotPosition, FVector& NearestPosition)
// {
// 	if (!IsValid(PivotOwner))
// 		return false;
// 
// 	auto RGameInstance = RGAMEINSTANCE(GEngine);
// 	if ( !IsValid(RGameInstance->GridMapManager))
// 		return false;
// 
// 	auto PathFindingComponent = Cast<UPathFindingComponent>(PivotOwner->GetComponentByClass(UPathFindingComponent::StaticClass()));
// 	if (!IsValid(PathFindingComponent))
// 		return false;
// 
// 	if (PathFindingComponent->GetNearestPositionAroundPivotPositionWithTarget(TargetOwner, PivotPosition, NearestPosition) == false)
// 		return false;
// 
// 
// 	FCellIndex PivotCell, TargetCell;
// 	if (RGameInstance->GridMapManager->GetCellIndexFromWorldPos(PivotPosition, PivotCell) == false)
// 		return false;
// 
// 	if (RGameInstance->GridMapManager->GetCellIndexFromWorldPos(TargetOwner->GetActorLocation(), TargetCell) == false)
// 		return false;
// 
// 	if (RGameInstance->GridMapManager->FindNearestPivotCellWithTargetCell(PivotCell, TargetCell) == false)
// 		return false;
// 
// 	RGameInstance->GridMapManager->GetWorldPosFromCellIndex(PivotCell, NearestPosition);
// 	NearestPosition.Z = PivotPosition.Z;
// 
// 	return true;
// }



// bool UMapFunctionLibrary::GetNearestPositionAroundPivotPosition(AActor* PivotOwner, const FVector& PivotPosition, FVector& NearestPosition)
// {
// 	if (!IsValid(PivotOwner))
// 		return false;
// 
// 	auto RGameInstance = RGAMEINSTANCE(GEngine);
// 	if ( !IsValid(RGameInstance->GridMapManager))
// 		return false;
// 
// 	auto PathFindingComponent = Cast<UPathFindingComponent>(PivotOwner->GetComponentByClass(UPathFindingComponent::StaticClass()));
// 	if (!IsValid(PathFindingComponent))
// 		return false;
// 
// 	if (PathFindingComponent->GetNearestPositionArountPivotPosition(PivotPosition, NearestPosition) == false)
// 		return false;
// 
// 	FCellIndex PivotCell;
// 	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(PivotPosition, PivotCell);
// 	if (RGameInstance->GridMapManager->FindNearestPivotCellWithTargetCell(PivotCell, PivotCell) == false)
// 		return false;
// 
// 	RGameInstance->GridMapManager->GetWorldPosFromCellIndex(PivotCell, NearestPosition);
// 	NearestPosition.Z = PivotPosition.Z;
// 
// 	return true;
// }

bool UMapFunctionLibrary::IsValidLocation(const FVector& TargetPosition)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);

	return RGameInstance->GridMapManager->IsTargetGridValidByWorldPos(TargetPosition);
}

/*
////////////////////////////////////////////////////////////////////////////////////////////
Line Trace
///////////////////////////////////////////////////////////////////////////////////////////
*/
// bool UMapFunctionLibrary::GetValidLocationAroundPivotLocation(UWorld* InWorld, const FVector& StartPos, const FVector& EndPos, FVector& ValidPos, bool bSweep)
// {
// 	if (InWorld == nullptr)
// 		return false;
// 
// 	auto GameInstance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(GameInstance) || !IsValid(GameInstance->GridMapManager))
// 		return false;
// 
// #ifdef DRAW_DEBUG
// 	//DrawDebugLine(GameInstance->GetWorld(), StartPos, EndPos, FColor(255, 0, 0, 1), false, 10.0f, 0.0f, 2.0f);
// #endif
// 	FCellIndex StartCellIndex, EndCellIndex;
// 	GameInstance->GridMapManager->GetCellIndexFromWorldPos(StartPos, StartCellIndex);
// 	GameInstance->GridMapManager->GetCellIndexFromWorldPos(EndPos, EndCellIndex);
// 
// 	auto DistXCell = static_cast<int32>(FMath::Abs(StartCellIndex.XIdx - EndCellIndex.XIdx));
// 	auto DistYCell = static_cast<int32>(FMath::Abs(StartCellIndex.YIdx - EndCellIndex.YIdx));
// 	float IntervalPos = 0.0f;
// 	if (DistXCell < DistYCell)
// 	{
// 		IntervalPos = FMath::Abs(StartPos.X - EndPos.X) / static_cast<float>(DistYCell);
// 	}
// 	else
// 	{
// 		IntervalPos = FMath::Abs(StartPos.Y - EndPos.Y) / static_cast<float>(DistXCell);
// 	}
// 
// 	bool		bIsFindValidGrid = false;
// 
// 	if (FMath::IsNearlyZero(StartPos.X - EndPos.X))
// 	{
// 		if (bSweep)
// 		{
// 			if (StartCellIndex.YIdx > EndCellIndex.YIdx)
// 			{
// 				for (float YWorldPos = StartPos.Y; YWorldPos >= EndPos.Y; YWorldPos -= IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);
// 
// 					bIsFindValidGrid = true;
// 					auto bIsGridValid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (!bIsGridValid)
// 						break;
// 
// 					bIsFindValidGrid = true;
// 					ValidPos = TargetWorldPos;
// 				}
// 			}
// 			else
// 			{
// 				for (float YWorldPos = StartPos.Y; YWorldPos <= EndPos.Y; YWorldPos += IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);
// 					auto bIsGridValid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsGridValid == true)
// 					{
// 						bIsFindValidGrid = true;
// 						ValidPos = TargetWorldPos;
// 					}
// 
// 					if (!bIsGridValid)
// 						break;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if (EndCellIndex.YIdx < StartCellIndex.YIdx)
// 			{
// 				for (float YWorldPos = EndPos.Y; YWorldPos <= StartPos.Y; YWorldPos += IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);
// 					bIsFindValidGrid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsFindValidGrid)
// 					{
// 						ValidPos = TargetWorldPos;
// 						break;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				for (float YWorldPos = EndPos.Y; YWorldPos >= StartPos.Y; YWorldPos -= IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);
// 					bIsFindValidGrid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsFindValidGrid)
// 					{
// 						ValidPos = TargetWorldPos;
// 						break;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	else if (FMath::IsNearlyZero(StartPos.Y - EndPos.Y))
// 	{
// 		if (bSweep)
// 		{
// 			if (StartCellIndex.XIdx > EndCellIndex.XIdx)
// 			{
// 				for (float XWorldPos = StartPos.X; XWorldPos >= EndPos.X; XWorldPos -= IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
// 					auto bIsGridValid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsGridValid == true)
// 					{
// 						bIsFindValidGrid = true;
// 						ValidPos = TargetWorldPos;
// 					}
// 
// 					if (!bIsGridValid)
// 						break;
// 				}
// 			}
// 			else
// 			{
// 				for (float XWorldPos = StartPos.X; XWorldPos <= EndPos.X; XWorldPos += IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
// 					auto bIsGridValid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsGridValid == true)
// 					{
// 						bIsFindValidGrid = true;
// 						ValidPos = TargetWorldPos;
// 					}
// 
// 					if (!bIsGridValid)
// 						break;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if (EndCellIndex.XIdx < StartCellIndex.XIdx)
// 			{
// 				for (float XWorldPos = EndPos.X; XWorldPos <= StartPos.X; XWorldPos += IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
// 					bIsFindValidGrid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsFindValidGrid)
// 					{
// 						ValidPos = TargetWorldPos;
// 						break;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				for (float XWorldPos = EndPos.X; XWorldPos >= StartPos.X; XWorldPos -= IntervalPos)
// 				{
// 					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
// 					bIsFindValidGrid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsFindValidGrid)
// 					{
// 						ValidPos = TargetWorldPos;
// 						break;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	else //y =ax + b
// 	{
// 		float	M = static_cast<float>(EndPos.Y - StartPos.Y) / static_cast<float>(EndPos.X - StartPos.X);
// 		auto	Y = [&M, &StartPos](float XPos)->float { return M * (XPos - StartPos.X) + StartPos.Y; };
// 		if (bSweep)
// 		{
// 			if (StartCellIndex.XIdx > EndCellIndex.XIdx)
// 			{
// 				for (float XWorldPos = StartPos.X; XWorldPos >= EndPos.X; XWorldPos -= IntervalPos)
// 				{
// 					float YWorldPos = Y(XWorldPos);
// 					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
// 					auto bIsGridValid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsGridValid == true)
// 					{
// 						bIsFindValidGrid = true;
// 						ValidPos = TargetWorldPos;
// 					}
// 
// 					if (!bIsGridValid)
// 						break;
// 				}
// 			}
// 			else
// 			{
// 				for (float XWorldPos = StartPos.X; XWorldPos <= EndPos.X; XWorldPos += IntervalPos)
// 				{
// 					float YWorldPos = Y(XWorldPos);
// 					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
// 					auto bIsGridValid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsGridValid == true)
// 					{
// 						bIsFindValidGrid = true;
// 						ValidPos = TargetWorldPos;
// 					}
// 
// 					if (!bIsGridValid)
// 						break;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if (EndCellIndex.XIdx < StartCellIndex.XIdx)
// 			{
// 				for (float XWorldPos = EndPos.X; XWorldPos <= StartPos.X; XWorldPos += IntervalPos)
// 				{
// 					float YWorldPos = Y(XWorldPos);
// 					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
// 					bIsFindValidGrid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsFindValidGrid)
// 					{
// 						ValidPos = TargetWorldPos;
// 						break;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				for (float XWorldPos = EndPos.X; XWorldPos >= StartPos.X; XWorldPos -= IntervalPos)
// 				{
// 					float YWorldPos = Y(XWorldPos);
// 					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
// 					bIsFindValidGrid = IsValidLocation(InWorld, TargetWorldPos);
// 					if (bIsFindValidGrid)
// 					{
// 						ValidPos = TargetWorldPos;
// 						break;
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	if (bIsFindValidGrid)
// 	{
// 		float Height = 0.0f;
// 		if (GetHeightFloor(InWorld, ValidPos, Height) == false)
// 			return false;
// 
// 		ValidPos.Z = Height;
// #ifdef DRAW_DEBUG
// 		DrawDebugPoint(GameInstance->GetWorld(), ValidPos, 10, FColor(0, 255, 0, 1), false, 10.0f);
// #endif
// 	}
// 
// 	return bIsFindValidGrid;
// }
//
void UMapFunctionLibrary::AdjustActorLocation(AActor* Entity, FVector& InLocation)
{
	if (nullptr == Entity)
		return;

	FVector	FloorPos;
	//if (UMapFunctionLibrary::GetProjectionToFloorHitLandscape(Entity->GetWorld(), InLocation, FloorPos))
	if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(Entity, InLocation, FloorPos))
	{
		UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(Entity);
		if (ActorCapsuleComponent)
		{
			FloorPos.Z += ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
		}

		InLocation = FloorPos;
	}
}

bool UMapFunctionLibrary::GetValidPlacableProjectionPos(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint, AActor* InActor)
{	
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);

	if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(InObject, InPoint + FVector(0, 0, 200.0f), ProjectionPoint) == false)
		return false;

	UCapsuleComponent* CapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(InActor);
	if (CapsuleComponent)
		ProjectionPoint.Z = ProjectionPoint.Z + CapsuleComponent->GetScaledCapsuleHalfHeight();

	return true;
}

bool UMapFunctionLibrary::GetHeightFloor(UObject* InObject, const FVector& Pos, float& Height)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);

	FVector ProjectionPoint;
	if (GetProjectionToFloorHitBlockingVolume(InObject, Pos, ProjectionPoint) == false && GetProjectionToFloorHitLandscape(InObject, Pos, ProjectionPoint) == false)
		return false;

	Height = ProjectionPoint.Z;

	return true;
}

bool UMapFunctionLibrary::GetProjectionToFloorHitLandscape(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint)
{
	//URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(InObject, EGetWorldErrorMode::Assert);
	checkf(InObject != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	TArray<FHitResult> OutHitArray;
	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("Find_Floor_BlockingVolume_trace")), false, nullptr);
	CollisionParams.bTraceComplex = false;

	FVector StartLocation = InPoint, EndLocation = InPoint;
	StartLocation.Z = InPoint.Z + 1000000.0f;
	EndLocation.Z = InPoint.Z - 1000000.0f;

#ifdef DRAW_DEBUG
	DrawDebugLine(InWorld, StartLocation, EndLocation, FColor(255, 0, 0, 1), false, 30.0f, 0, 3);
#endif

	GameWorld->LineTraceMultiByObjectType(OutHitArray, StartLocation, EndLocation, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects), CollisionParams);
	if (OutHitArray.Num() <= 0)
		return false;

	bool bFindHitLocation = false;

	for (auto& HitElem : OutHitArray)
	{
		if (HitElem.bBlockingHit == false)
			continue;

		if (!HitElem.Actor.IsValid())
			continue;

		auto HitActor = HitElem.Actor.Get();
		
		auto ActorName = HitActor->GetName();
		
		if (HitActor == nullptr)
			continue;

		if (ActorName.Contains("Landscape") == true)
		{
			ProjectionPoint = HitElem.Location;
			bFindHitLocation = true;
			break;
		}
	}

#ifdef DRAW_DEBUG
	DrawDebugPoint(InWorld, ProjectionPoint, 10, FColor(0, 255, 0, 1), false, 10.0f);
#endif

	return bFindHitLocation;
}

bool UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint, AActor* IgnoreActor)
{
//	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(InObject, EGetWorldErrorMode::Assert);
	checkf(InObject != nullptr, TEXT("%s[InObject is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	TArray<FHitResult> OutHitArray;
	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("Find_Floor_BlockingVolume_trace")), false, IgnoreActor);
	CollisionParams.bTraceComplex = false;

	FVector StartLocation	= InPoint, EndLocation = InPoint;
	StartLocation.Z			= InPoint.Z + 1000000.0f;
	EndLocation.Z			= InPoint.Z - 1000000.0f;

#ifdef DRAW_DEBUG
	DrawDebugLine(InWorld, StartLocation, EndLocation, FColor(255, 0, 0, 1), false, 30.0f, 0, 3);
#endif

	GameWorld->LineTraceMultiByObjectType(OutHitArray, StartLocation, EndLocation, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects), CollisionParams);
	if (OutHitArray.Num() <= 0)
		return false;

	bool bFindHitLocation = false;

	for (auto& HitElem : OutHitArray)
	{
		if (HitElem.bBlockingHit == false)
			continue;

		if (!HitElem.Actor.IsValid())
			continue;

		auto HitActorBlockingVolume = Cast<ABlockingVolume>(HitElem.Actor.Get());
		auto ActorName = HitElem.Actor.Get()->GetName();

		if (HitActorBlockingVolume != nullptr || ActorName.Contains("Landscape") == true)
		{
			bFindHitLocation = true;
			ProjectionPoint = HitElem.Location;

			break;
		}
	}

#ifdef DRAW_DEBUG
	DrawDebugPoint(InWorld, ProjectionPoint, 10, FColor(0, 255, 0, 1), false, 10.0f);
#endif

	return bFindHitLocation;
}

// bool UMapFunctionLibrary::IsInPeaceZone(UWorld* InWorld, const FVector& InPoint)
// {
// 	if (InWorld == nullptr)
// 		return false;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(RGameInstance))
// 		return false;
// 
// 	if (!IsValid(RGameInstance->GridMapManager))
// 		return false;
// 
// 	FMapCellData CellData = RGameInstance->GridMapManager->GetMapInfoFromWorldPos(InPoint);
// 	if (EMapCellStateEnum::VE_Movable == CellData.CellInfo)
// 	{
// 		return (EMapCellZoneType::VE_PeaceZone == CellData.CellZone);
// 	}
// 
// 	return false;
// }
//
// bool UMapFunctionLibrary::GetProjectionPointToDirection(UWorld* InWorld, const FVector& WorldLocation, const FVector& WorldDirection, FVector& ValidPos)
// {
// 	if (InWorld == nullptr)
// 		return false;
// 
// 	TArray<FHitResult> OutHitArray;
// 	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("Trace")), false, nullptr);
// 	CollisionParams.bTraceComplex = false;
// 
// 
// #ifdef TESTCOW
// 	DrawDebugLine(InWorld, WorldLocation, WorldLocation + WorldDirection * 10000.0f, FColor(255, 0, 0, 1), false, 10.0f, 0.0f, 2.0f);
// #endif
// 
// 	InWorld->LineTraceMultiByChannel(OutHitArray, WorldLocation, WorldLocation + WorldDirection * 10000.0f, ECC_WorldStatic, CollisionParams);
// 	if (OutHitArray.Num() == 0)
// 	{
// 		return false;
// 	}
// 
// 	for (FHitResult& hitResult : OutHitArray)
// 	{
// 		ValidPos = hitResult.Location;
// 		return true;
// 	}
// 
// 	return false;
// }

bool UMapFunctionLibrary::IsCollideInMoving(AActor* MoveActor, const FVector& GoalPos)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
// 	checkf(RGameInstance->GameObjectMgr != nullptr, TEXT("%s[RGameInstance->GameObjectMgr is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	if (nullptr == MoveActor)
		return true;

	TArray<FHitResult> OutHitArray;
	FVector StartPos = MoveActor->GetActorLocation();
	FQuat Rot = FQuat::Identity;
	ECollisionChannel ColChannel = ECollisionChannel::ECC_WorldStatic;

	FCollisionQueryParams CollisionParams;
	CollisionParams.TraceTag = FName("StageWall");
	CollisionParams.bTraceComplex = false;

	TArray<TWeakObjectPtr<AActor>> ActorList;
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		pGamePlayMode->GetObjectList(ActorList);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.GetObjectList(ActorList);
	}
// 	RGameInstance->GameObjectMgr->GetObjectList(ActorList);
	for (auto IgnoredActor : ActorList)
	{
		CollisionParams.AddIgnoredActor(IgnoredActor.Get());
	}

	if (GameWorld->LineTraceMultiByChannel(OutHitArray, StartPos, GoalPos, ColChannel, CollisionParams))
	{
		for (FHitResult& OutHit : OutHitArray)
		{
			AActor* BlockActor = OutHit.GetActor();
			//UE_LOG(LogGamePlay, Log, TEXT("UMapFunctionLibrary::IsCollideInMoving() BlockActor(%s)"), *(AActor::GetDebugName(BlockActor)));

			if (BlockActor)
			{
				if (BlockActor->ActorHasTag(FName("StageWall")))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool UMapFunctionLibrary::GetAdjacentCellNearestWithTargetPositionOnNavyMesh(FCellIndex& AdjacentCell, const FVector& TargetPos, int32 MinDepth, int32 MaxDepth)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);

	FCellIndex PivotCell;
	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(TargetPos, PivotCell);

	return GetAdjacentCellNearestWithPivotCellnOnNavyMesh(PivotCell, AdjacentCell, MinDepth, MaxDepth);
}

bool UMapFunctionLibrary::GetAdjacentPosNearestWithTargetPositionOnNavyMesh(FVector& AdjacentPos, const FVector& TargetPos, int32 MinDepth, int32 MaxDepth)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);

	FCellIndex AdjacentCell;
	if (GetAdjacentCellNearestWithTargetPositionOnNavyMesh(AdjacentCell, TargetPos, 0, 4) == false)
		return false;

	AdjacentPos = RGameInstance->GridMapManager->GetCellWorldPos(AdjacentCell);

	return true;
}

bool UMapFunctionLibrary::GetAdjacentCellNearestWithPivotCellnOnNavyMesh(const FCellIndex& PivotCell, FCellIndex& AdjacentCell, int32 MinDepth, int32 MaxDepth)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);

	TArray<FCellIndex> AdjacentCells;
	if (GetAdjacentCellsOnGrid(PivotCell, MinDepth, MaxDepth, AdjacentCells) == false)
		return false;

	float MinDist = 100000000.0f;
	for (auto& Elem : AdjacentCells)
	{
		FVector OutWorldPos, ProjectionPos;
		RGameInstance->GridMapManager->GetWorldPosFromCellIndex(Elem, OutWorldPos);

		/*
		if (GetProjectionToFloorHitBlockingVolume(InWorld, OutWorldPos, ProjectionPos) == false||
			UUtilFunctionLibrary::IsEnableNavigationPos(InWorld, OutWorldPos + FVector::UpVector * 10000.0f, FVector::ZeroVector, true) == false ||
			IsValidLocation(InWorld, OutWorldPos) == false)
			*/
		if(CanSpawnPostion(RGameInstance, OutWorldPos) == false)
			continue;
			
		float Dist = static_cast<float>(Elem.GetCellDistCost(PivotCell));
		if (Dist < MinDist)
		{
			MinDist = Dist;
			AdjacentCell = Elem;
		}
	}

	return true;
}

bool UMapFunctionLibrary::GetAdjacentCellsOnGrid(const FCellIndex& PivotCell, int32 MinDepth, int32 MaxDepth, TArray<FCellIndex>& AdjacentCells)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);

	auto DepthOneOffsets = RGameInstance->GridMapManager->GetDepthOneOffSets();

	TQueue<FBfs_Cell_Info>		BFS_Queue;
	TSet<int32>					CheckCells;

	FBfs_Cell_Info TargetCell;
	TargetCell.CellIndex = PivotCell;
	TargetCell.Width		= 0;
	BFS_Queue.Enqueue(TargetCell);

	while (BFS_Queue.IsEmpty() == false)
	{
		if (BFS_Queue.Dequeue(TargetCell) == false)
		{
			break;
		}

		CheckCells.Emplace(TargetCell.CellIndex.GetCustomHashValue());

		if (MinDepth <= TargetCell.Width && TargetCell.Width <= MaxDepth)
		{
			AdjacentCells.Emplace(TargetCell.CellIndex);
		}

		for (auto& DepthOffsetElem : DepthOneOffsets)
		{
			FCellIndex AdjacentCell;
			AdjacentCell.XIdx = TargetCell.CellIndex.XIdx + DepthOffsetElem.XIdx;
			AdjacentCell.YIdx = TargetCell.CellIndex.YIdx + DepthOffsetElem.YIdx;

			if (CheckCells.Contains(AdjacentCell.GetCustomHashValue()))
				continue;

			FBfs_Cell_Info				AdjacentCellInfo;
			AdjacentCellInfo.CellIndex	= AdjacentCell;
			AdjacentCellInfo.Width		= TargetCell.Width + 1;

			if (AdjacentCellInfo.Width > MaxDepth)
				continue;

			BFS_Queue.Enqueue(AdjacentCellInfo);
			CheckCells.Emplace(AdjacentCell.GetCustomHashValue());
		}
	}

	return true;
}


FVector UMapFunctionLibrary::GetWorldPosFromCell(FCellIndex& InCellIndex)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);

	return RGameInstance->GridMapManager->GetCellWorldPos(InCellIndex);
}

bool UMapFunctionLibrary::IsProjectionPointOnNavyMesh(UObject* InObject, FVector& Pos, FVector Extent, bool bFindFloorPoint)
{
//	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(InObject, EGetWorldErrorMode::Assert);
	checkf(InObject != nullptr, TEXT("%s[InObject is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(GameWorld);
	if (IsValid(navSystem))
	{
		if (bFindFloorPoint)
		{
			FVector ProjectionPoint;
			if (GetProjectionToFloorHitBlockingVolume(InObject, Pos, ProjectionPoint) == false)
				return false;

			Pos = ProjectionPoint;
		}

#ifdef DRAW_DEBUG_INFO
		DrawDebugPoint(InWorld, Pos, 10.0f, FColor::Yellow, false, 1000.0f);
#endif
		// 			if (ProjectionPoint.Equals(FVector::ZeroVector))
		// 			{
		// 				return false;
		// 			}
		// 			else
		// 			{
		FNavLocation navLocation;
		bool Result = navSystem->ProjectPointToNavigation(Pos, navLocation, Extent);
		if (Result)
		{
#ifdef DRAW_DEBUG_INFO
		//	DrawDebugPoint(InWorld, navLocation.Location, 10.0f, FColor::Blue, false, 1000.0f);
#endif
			return Result;
		}
	}

	return false;
}

bool UMapFunctionLibrary::CanSpawnPostion(UObject* InObject, FVector& Pos)
{
	if (IsProjectionPointOnNavyMesh(InObject, Pos) == false ||IsValidLocation(Pos) == false)
		return false;

	return true;
}

bool UMapFunctionLibrary::CanMoveToTargetPosition(AActor* Character, FVector InTargetPos)
{
	if (IsValid(Character) == false)
		return false;

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);

	if (CanSpawnPostion(RGameInstance,InTargetPos) == false)
		return false;

	auto MapState = RGameInstance->GridMapManager->GetMapInfoFromWorldPos(InTargetPos);
	if (MapState.CellInfo == EMapCellStateEnum::VE_Invalid || MapState.CellInfo == EMapCellStateEnum::VE_None)
		return true;

	if (CanMoveUp(Character, MapState.Height) == false)
		return false;

	return true;
}

bool UMapFunctionLibrary::CanMoveUp(AActor* Character, const float& GridHeight)
{
	auto CharacterHeight = Character->GetActorLocation().Z;
	auto DiffHeight = GridHeight - CharacterHeight;
	if (DiffHeight <= MoveUpMaxHeight)
	{
		return true;
	}

	return false;
}

bool UMapFunctionLibrary::GetProjectionPointToFloor(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint)
{
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(InObject, EGetWorldErrorMode::Assert);
	checkf(InObject != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("ProjectionPointToFloor")), false, nullptr);
	CollisionParams.bTraceComplex = false;
	FVector StartPosition = InPoint;
	FVector EndPosition = InPoint + FVector(0.0f, 0.0f, -1.0f) * 100000.0f;
	GameWorld->LineTraceSingleByChannel(HitResult, StartPosition, EndPosition, ECC_WorldStatic, CollisionParams);

#ifdef DRAW_DEBUG_INFO
	//DrawDebugLine(InWorld, StartPosition, EndPosition, FColor::Red, false, 1000.0f);
#endif
	if (HitResult.bBlockingHit == true)
	{
		ProjectionPoint = HitResult.Location;

		return true;
	}

	return false;
}


bool UMapFunctionLibrary::IsEnableNavigationPos(UObject* InObject,FVector pos, FVector InExtent, bool bFindFloorPoint)
{
	//URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(InObject, EGetWorldErrorMode::Assert);
	checkf(InObject != nullptr, TEXT("%s[InObject is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(GameWorld);
	if (IsValid(navSystem))
	{
		if (bFindFloorPoint)
		{
			FVector ProjectionPoint;
			if (GetProjectionPointToFloor(InObject, pos, ProjectionPoint) == false)
				return false;

			pos = ProjectionPoint;
		}

#ifdef DRAW_DEBUG_INFO
		DrawDebugPoint(GameWorld, pos, 10.0f, FColor::Yellow, false, 1000.0f);
#endif
		// 			if (ProjectionPoint.Equals(FVector::ZeroVector))
		// 			{
		// 				return false;
		// 			}
		// 			else
		// 			{
		FNavLocation navLocation;
		bool Result = navSystem->ProjectPointToNavigation(pos, navLocation, InExtent);
		if (Result)
		{
#ifdef DRAW_DEBUG_INFO
			DrawDebugPoint(GameWorld, navLocation.Location, 10.0f, FColor::Blue, false, 1000.0f);
#endif
			return Result;
		}
		//}
	}
	return false;
}

// bool UMapFunctionLibrary::IsEnableNavigationPosFromActor(AActor* InActor, FVector pos, FVector InExtent)
// {
// 	auto World = InActor->GetWorld();
// 	if (IsValid(World))
// 	{
// 		UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(World);
// 		if (IsValid(navSystem))
// 		{
// 			FNavLocation navLocation;
// 			return navSystem->ProjectPointToNavigation(pos, navLocation, InExtent);
// 		}
// 	}
// 	return false;
// }
//
// bool UMapFunctionLibrary::IsValidNavigationPath(AActor* OriginActor, FVector Start, FVector End)
// {
// 	if (IsValid(OriginActor))
// 	{
// 		UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OriginActor->GetWorld());
// 		if (IsValid(navSystem))
// 		{
// 			UNavigationPath* ResultPath = navSystem->FindPathToLocationSynchronously(OriginActor->GetWorld(), Start, End);
// 			if (ResultPath)
// 			{
// 				if (ResultPath->IsValid())
// 					return true;
// 			}
// 		}
// 	}
// 	return false;
// }
//
// TArray< FVector > UMapFunctionLibrary::GetNavigationPaths(AActor* OriginActor, FVector Start, FVector End)
// {
// 	if (IsValid(OriginActor))
// 	{
// 		UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OriginActor->GetWorld());
// 		if (IsValid(navSystem))
// 		{
// 			UNavigationPath* ResultPath = navSystem->FindPathToLocationSynchronously(OriginActor->GetWorld(), Start, End);
// 			if (ResultPath)
// 			{
// 				return ResultPath->PathPoints;
// 			}
// 		}
// 	}
// 	return TArray< FVector >();
// }
//
// bool UMapFunctionLibrary::GetPawnRandomPointInNavigableRadius(AActor* OriginPawn, float Radius, FNavLocation& ResultLocation, FVector InLocationSearch /*= FVector::ZeroVector*/)
// {
// 	if (IsValid(OriginPawn))
// 	{
// 		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OriginPawn);
// 		UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OriginPawn->GetWorld());
// 		if (IsValid(navSystem) && IsValid(MovementComp))
// 		{
// 			const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
// 			ANavigationData* NavData = navSystem->GetNavDataForProps(AgentProperties);
// 			if (NavData == NULL)
// 			{
// 				NavData = navSystem->GetMainNavData(FNavigationSystem::DontCreate);
// 			}
// 
// 			if (IsValid(NavData))
// 			{
// 				if (InLocationSearch.IsNearlyZero())
// 				{
// 					if (navSystem->GetRandomPointInNavigableRadius(OriginPawn->GetActorLocation(), Radius, ResultLocation, NavData))
// 					{
// 						return true;
// 					}
// 				}
// 				else
// 				{
// 					if (navSystem->GetRandomPointInNavigableRadius(InLocationSearch, Radius, ResultLocation, NavData))
// 					{
// 						return true;
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	return false;
// }
//
// bool UMapFunctionLibrary::GetPawnRandomReachablePointInRadius(AActor* OriginPawn, float Radius, FNavLocation& ResultLocations)
// {
// 	if (IsValid(OriginPawn))
// 	{
// 		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OriginPawn);
// 		UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OriginPawn->GetWorld());
// 		if (IsValid(navSystem) && IsValid(MovementComp))
// 		{
// 			const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
// 			ANavigationData* NavData = navSystem->GetNavDataForProps(AgentProperties);
// 			if (NavData == NULL)
// 			{
// 				NavData = navSystem->GetMainNavData(FNavigationSystem::DontCreate);
// 			}
// 
// 			if (IsValid(NavData))
// 			{
// 				if (navSystem->GetRandomReachablePointInRadius(OriginPawn->GetActorLocation(), Radius, ResultLocations, NavData))
// 				{
// 					return true;
// 				}
// 			}
// 		}
// 	}
// 
// 	return false;
// }
//
// const ARecastNavMesh* UMapFunctionLibrary::GetPawnNavPolysWithinPathingDistance(AActor* OriginPawn, float PathingDistance, TArray<NavNodeRef>& FoundPolys, FVector InLocationSearch /*= FVector::ZeroVector*/)
// {
// 	if (IsValid(OriginPawn))
// 	{
// 		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OriginPawn);
// 		UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OriginPawn->GetWorld());
// 		if (IsValid(navSystem) && IsValid(MovementComp))
// 		{
// 			const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
// 			const ANavigationData* NavData = navSystem->GetNavDataForProps(AgentProperties);
// 			if (NavData == NULL)
// 			{
// 				NavData = navSystem->GetMainNavData(FNavigationSystem::DontCreate);
// 			}
// 
// 			if (IsValid(NavData))
// 			{
// 				const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
// 				if (IsValid(NavMesh))
// 				{
// 					if (InLocationSearch.IsNearlyZero())
// 					{
// 						if (NavMesh->GetPolysWithinPathingDistance(OriginPawn->GetActorLocation(), PathingDistance, FoundPolys))
// 						{
// 							return NavMesh;
// 						}
// 					}
// 					else
// 					{
// 						if (NavMesh->GetPolysWithinPathingDistance(InLocationSearch, PathingDistance, FoundPolys))
// 						{
// 							return NavMesh;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	return nullptr;
// }

bool UMapFunctionLibrary::GetMovePawnRandomReachablePointInRadius(AActor* OriginPawn, float Radius, FVector& StartLocations, FNavLocation& ResultLocations)
{
	if (IsValid(OriginPawn))
	{
		UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OriginPawn);
		if (MovementComponent)
		{
			UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OriginPawn->GetWorld());
			if (IsValid(navSystem) && IsValid(MovementComponent))
			{
				const FNavAgentProperties& AgentProperties = MovementComponent->GetNavAgentPropertiesRef();
				ANavigationData* NavData = navSystem->GetNavDataForProps(AgentProperties);
				if (NavData == NULL)
				{
					NavData = navSystem->GetMainNavData(FNavigationSystem::DontCreate);
				}

				if (IsValid(NavData))
				{
					if (navSystem->GetRandomReachablePointInRadius(StartLocations, Radius, ResultLocations, NavData))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

FVector UMapFunctionLibrary::GetNavAgentLocation(AActor* InActor)
{
	if (nullptr == InActor)
	{
		return FVector::ZeroVector;
	}

	FVector ActorLocation = InActor->GetActorLocation();

	UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(InActor);
	if (ActorCapsuleComponent)
	{
		ActorLocation.Z -= ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
	}

	return ActorLocation;
}

void UMapFunctionLibrary::LoadLevel(FName LevelName, bool IsStreamLevel /*= false*/, FName CompleteFunction, UObject* CallbackTarget)
{
	if (CallbackTarget != nullptr)
	{
		FString CallbackTargetName = CallbackTarget->GetName();
		UE_LOG(LogGamePlayMode, Log, TEXT("ENTER LoadLevel to LevelName[%s], IsStreamLevel[%d], CompleteFunction[%s], CallbackTarget[%s]"), *LevelName.ToString(), (IsStreamLevel == true ? 1 : 0), *CompleteFunction.ToString(), *CallbackTargetName);
	}
	else
	{
		UE_LOG(LogGamePlayMode, Log, TEXT("ENTER LoadLevel to LevelName[%s], IsStreamLevel[%d], CompleteFunction[%s], CallbackTarget is Nullptr"), *LevelName.ToString(), (IsStreamLevel == true ? 1 : 0), *CompleteFunction.ToString());
	}

	if (IsStreamLevel)
	{
		LoadSubLevel(LevelName, CompleteFunction, CallbackTarget);
	}
	else
	{
		UGameplayStatics::OpenLevel(RGAMEINSTANCE(GEngine), LevelName);
	}
}

void UMapFunctionLibrary::LoadSubLevel(FName LevelName, FName ComplentFunction, UObject* CallbackTarget, FTransform transform)
{

	static int32 uuid = 1;

	FLatentActionInfo LatentActionInfo;
	if (CallbackTarget != nullptr)
	{
		LatentActionInfo.ExecutionFunction = ComplentFunction;
		LatentActionInfo.CallbackTarget = CallbackTarget;
		LatentActionInfo.UUID = uuid++;
		LatentActionInfo.Linkage = 0;
		UE_LOG(LogGamePlayMode, Log, TEXT("ENTER 1 LoadSubLevel to LevelName[%s], CompleteFunction[%s], CallbackTarget[%s]"), *LevelName.ToString(), *ComplentFunction.ToString(), *CallbackTarget->GetName());
	}

	UGameplayStatics::LoadStreamLevel(RGAMEINSTANCE(GEngine), LevelName, true, true, LatentActionInfo);
	UE_LOG(LogGamePlayMode, Log, TEXT("ENTER 2 LoadSubLevel to LevelName[%s], CompleteFunction[%s]"), *LevelName.ToString(), *ComplentFunction.ToString());
	if (CallbackTarget)
	{
		FLatentActionManager& LatentManager = RGAMEINSTANCE(GEngine)->GetWorld()->GetLatentActionManager();
		FStreamLevelAction* LevelAction = LatentManager.FindExistingAction<FStreamLevelAction>(LatentActionInfo.CallbackTarget, LatentActionInfo.UUID);
		if (LevelAction)
		{
			if (LevelAction->Level)
			{
				LevelAction->Level->LevelTransform = transform;
			}
		}
	}
}

void UMapFunctionLibrary::UnLoadSubLevel(FName LevelName, FName CompleteFunction /*= FName("")*/, UObject* CallbackTarget /*= nullptr*/)
{
	static int32 uuid = 1;

	FLatentActionInfo LatentActionInfo;
	if (CallbackTarget != nullptr)
	{
		LatentActionInfo.ExecutionFunction = CompleteFunction;
		LatentActionInfo.CallbackTarget = CallbackTarget;
		LatentActionInfo.UUID = uuid++;
		LatentActionInfo.Linkage = 0;
	}

	UGameplayStatics::UnloadStreamLevel(RGAMEINSTANCE(GEngine), LevelName, LatentActionInfo);
}

bool UMapFunctionLibrary::isTargetPointonNavMesh(FVector InTargetLocation)
{
	UWorld* world = RGAMEINSTANCE(GEngine)->GetWorld();
	if (world)
	{
		UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(world);
		if (navSystem)
		{
			FNavLocation navLocation;
			bool bSucess = navSystem->ProjectPointToNavigation(InTargetLocation, navLocation);
			if (bSucess)
				return true;
		}
	}
	
	return false;
}
