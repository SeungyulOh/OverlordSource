// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Global/GridMapManager.h"
#include "PathFindingComponent.generated.h"

class UGridMapManager;




UCLASS()
class PROJECTR_API UGridDistanceInfo : public UObject
{
	GENERATED_BODY()
public:
	UGridDistanceInfo() 
	{
		
	}
	FCellIndex	ParentCellIndex;
	FCellIndex  MyCellIndex;
	float       Sum = 0;
	float       CurrentDistance = 0;
	float       RemainDistance = 0;
	EGridType	GridType;
	void		CalcCostFromParent(FCellIndex& InMyCellIndex, FCellIndex& DestCell, UGridDistanceInfo* InParent);
	void		Clear()
	{
		ParentCellIndex = FCellIndex(-1,-1);
		MyCellIndex =FCellIndex(-1,-1);
		Sum = 0;
		CurrentDistance = 0;
		RemainDistance = 0;
		GridType = EGridType::VE_None;
	}
};

//Priority Queue predicate
struct GridInfoPredicate
{
	bool operator() (const UGridDistanceInfo& Grid1, const UGridDistanceInfo& Grid2) const 
	{
		return (Grid1.Sum < Grid2.Sum);
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTR_API UPathFindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPathFindingComponent();

	// Called when the game starts
	virtual void			BeginPlay() override;

	
	//------------------------------------------------------------------------------------------------------------------------------------------------
	// Path Finding public method
	//------------------------------------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = PathFinding)
	bool					PathFindSourceToTarget(FCellIndex InSourceCellIndex, FCellIndex InTargetCellIndex);

	bool					PathFind(const FVector& DestinationPos, TArray<FVector>& InResultPath);
	bool					GetNearestPositionArountPivotPosition(const FVector& Destination, FVector& NearestPosition);
	bool					GetNearestPositionAroundPivotPositionWithTarget(AActor* TargetOwner, const FVector& Destination, FVector& NearestPosition);
private:
	//------------------------------------------------------------------------------------------------------------------------------------------------
	// Path Finding private method
	//------------------------------------------------------------------------------------------------------------------------------------------------


	bool					InitPathFindingContainer();
	bool					InitOpenList();
	
	bool					IsFindTargetCell(FCellIndex& CellIndex);
	bool					IsValidLocation(FCellIndex& CellIndex);

	void					AddCellOpenList(UGridDistanceInfo* InGridInfo);
	void					AddCellInCloseList(UGridDistanceInfo* GridInfo);

	bool					AStarPathFinding();
	void					InvalidateDistance(UGridDistanceInfo* ParentCell);
	void					SearchAdjacentCell(FCellIndex& PivotCell, TArray<FCellIndex>& InAdjacentCellList);

	bool					CheckDestinationCellIsValid(FCellIndex& InDestinationCell);
	bool					FindNearestPivotCellWithTargetCell(FCellIndex& InPivotCell, FCellIndex& InTargetCell);

	void					GetResultPath(TArray<FVector>& InResultPath);
	void					SetMinimumDistancePath();

	void					DrawGridBox();

	void					ClearObjectPool();

	void					SetBlackBoardTargetLocation(FCellIndex& InCellIndex);
	void					SaveMinimumDestinationCell(UGridDistanceInfo* InGridDistanceInfo);
	bool					FindNewMinimumDestination(FCellIndex& InOriginalCell);
	FCellIndex				GetAdjecentCellAroundTargetCell(FCellIndex& TargetCellIndex, FCellIndex& CellIndexOffset);
	UGridDistanceInfo*		FindMinimumDistanceCellInOpenList();
	FVector					GetDestinationPosByCellIndex(FCellIndex& CellIndex);
	UGridDistanceInfo*		GetNewGridDistanceInfo(const FCellIndex& CellIndex);

	//------------------------------------------------------------------------------------------------------------------------------------------------
	// Path Finding private container
	//------------------------------------------------------------------------------------------------------------------------------------------------
	TArray<UGridDistanceInfo*>				PriorityQueue_OpenList;
	TArray<UGridDistanceInfo*>				Path;
	FCellIndex								SourceCellIndex, TargetCellIndex;
	FCellIndex								MinimumCellIndex;
	UGridDistanceInfo*						MinimumDestinationGridInfo;
	
	bool									bSuccessPathFinding = false;	
	
};