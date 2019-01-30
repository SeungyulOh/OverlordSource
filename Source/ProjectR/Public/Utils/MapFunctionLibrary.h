// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Global/GridMapManager.h"
#include "Runtime/Engine/Classes/AI/Navigation/RecastNavMesh.h"
#include "MapFunctionLibrary.generated.h"

#define MoveUpMaxHeight 100.0f
/**
 * 
 */
UCLASS()
class PROJECTR_API UMapFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*
	Map Info
	*/
// 	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
// 	static bool GetNearestPositionAroundPivotPosition(AActor* PivotOwner, const FVector& PivotPosition, FVector& NearestPosition);
// 	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
// 	static bool GetNearestPositionAroundPivotPositionWithTarget(AActor* PivotOwner, AActor* TargetOwner, FVector& PivotPosition, FVector& NearestPosition);

	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
	static bool IsValidLocation(const FVector& TargetPosition);

	static void AdjustActorLocation(AActor* Entity, FVector& InLocation);
	static bool GetValidPlacableProjectionPos(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint, AActor* InActor);
	static bool GetHeightFloor(UObject* InObject, const FVector& Pos, float& Height);
	static bool GetProjectionToFloorHitBlockingVolume(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint, AActor* IgnoreActor = nullptr);
	static bool GetProjectionToFloorHitLandscape(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint);
//	static bool IsInPeaceZone(UWorld* InWorld, const FVector& InPoint);
//	static bool GetProjectionPointToDirection(UWorld* InWorld, const FVector& WorldLocation, const FVector& WorldDirection, FVector& ValidPos);
	static bool IsCollideInMoving(AActor* MoveActor, const FVector& GoalPos);

	static bool	GetAdjacentCellNearestWithTargetPositionOnNavyMesh(FCellIndex& AdjacentCell, const FVector& TargetPos, int32 MinDepth, int32 MaxDepth);
	static bool	GetAdjacentPosNearestWithTargetPositionOnNavyMesh(FVector& AdjacentPos, const FVector& TargetPos, int32 MinDepth, int32 MaxDepth);
	static bool	GetAdjacentCellNearestWithPivotCellnOnNavyMesh(const FCellIndex& PivotCell, FCellIndex& AdjacentCell, int32 MinDepth, int32 MaxDepth);
	static bool GetAdjacentCellsOnGrid(const FCellIndex& PivotCell, int32 MinDepth, int32 MaxDepth, TArray<FCellIndex>& AdjacentCells);
	static FVector GetWorldPosFromCell(FCellIndex& InCellIndex);
	//StartPos	: Actor Current Location
	//EndPos	: Actor Destination Location
	//ValidPos	: Valid Location that nearest with EndPos
	//bSweep	: true - Find from Front Grid to End, false - Find End Grid to Front.
	//ex: teleport's bSweep value is false, than counter attack's bSweep value is true
//	static bool GetValidLocationAroundPivotLocation(UWorld* InWorld, const FVector& StartPos, const FVector& EndPos, FVector& ValidPos, bool bSweep = true);
	
	static bool IsProjectionPointOnNavyMesh(UObject* InObject, FVector& Pos, FVector Extent = FVector::ZeroVector, bool bFindFloorPoint = true);
	static bool CanSpawnPostion(UObject* InObject, FVector& Pos);

	static bool CanMoveToTargetPosition(AActor* Character, FVector InTargetPos);
	static bool CanMoveUp(AActor* Character, const float& GridHeight);

	/*
	Path finding..
	*/
	static bool GetProjectionPointToFloor(UObject* InObject, const FVector& InPoint, FVector& ProjectionPoint);

	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
	static bool IsEnableNavigationPos(UObject* InObject, FVector pos, FVector InExtent = FVector::ZeroVector, bool bFindFloorPoint = true);

// 	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
// 	static bool IsEnableNavigationPosFromActor(AActor* InActor, FVector pos, FVector InExtent = FVector::ZeroVector);
// 	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
// 	static bool IsValidNavigationPath(AActor* OriginActor, FVector Start, FVector End);
// 	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
// 	static TArray< FVector > GetNavigationPaths(AActor* OriginActor, FVector Start, FVector End);

	UFUNCTION(BlueprintCallable, Category = UMapFunctionLibrary)
	static FVector GetNavAgentLocation(AActor* InActor);

//	static bool GetPawnRandomPointInNavigableRadius(AActor* OriginPawn, float Radius, FNavLocation& ResultLocation, FVector InLocationSearch = FVector::ZeroVector);
//	static bool GetPawnRandomReachablePointInRadius(AActor* OriginPawn, float Radius, FNavLocation& ResultLocation);
//	static const ARecastNavMesh* GetPawnNavPolysWithinPathingDistance(AActor* OriginPawn, float PathingDistance, TArray<NavNodeRef>& FoundPolys, FVector InLocationSearch = FVector::ZeroVector);
	static bool GetMovePawnRandomReachablePointInRadius(AActor* OriginPawn, float Radius, FVector& StartLocations, FNavLocation& ResultLocations);

	/*
	Load Level
	*/
	static void LoadLevel(FName LevelName, bool IsStreamLevel = false, FName ComplentFunction = FName(""), UObject* CallbackTarget = nullptr);

	static void LoadSubLevel(FName LevelName, FName ComplentFunction = FName(""), UObject* CallbackTarget = nullptr, FTransform transform = FTransform());

	/*
	UnLoad Level
	*/
	static void UnLoadSubLevel(FName LevelName, FName CompleteFunction = FName(""), UObject* CallbackTarget = nullptr);



	/*After 2018.09.15*/
	static bool isTargetPointonNavMesh(FVector InTargetLocation);
};
