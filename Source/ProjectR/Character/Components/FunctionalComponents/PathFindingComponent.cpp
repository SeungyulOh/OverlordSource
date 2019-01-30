// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"


//#include "Utils/MapNodeObjectPool.h"

#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"


#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Public/Containers/Queue.h"
#include "PathFindingComponent.h"
#include "UtilFunctionIntegrated.h"
//#define DRAW_DEBUG_BOX		
#define WEIGHT_VALUE              100.0f
#define DISTANCE_ERROR_VALUE      10.0f

// Sets default values for this component's properties
UPathFindingComponent::UPathFindingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPathFindingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


FVector       UPathFindingComponent::GetDestinationPosByCellIndex(FCellIndex& CellIndex)
{
	auto OwnerActor = GetOwner();
	if (IsValid(OwnerActor) == false)
		return FVector::ZeroVector;

	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->GridMapManager) == false)
		return FVector::ZeroVector;

	auto CellSize = RGameInstance->GridMapManager->GetCellSizeBymapStatus();

	FVector        DestinationPos;
	RGameInstance->GridMapManager->GetWorldPosFromCellIndex(CellIndex, DestinationPos);
	DestinationPos.X += CellSize * 0.5f;
	DestinationPos.Y += CellSize * 0.5f;
	DestinationPos.Z = OwnerActor->GetActorLocation().Z;
	
	return DestinationPos;
}


//----------------------------------------------------------------------------------------------------------------------
// Path Finding
//----------------------------------------------------------------------------------------------------------------------
bool UPathFindingComponent::PathFind(const FVector& DestinationPos, TArray<FVector>& InResultPath)
{
	auto OwnerActor = GetOwner();
	if (IsValid(OwnerActor) == false)
		return false;

	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	MinimumCellIndex = FCellIndex(-1, -1);
	auto OriginalPos = OwnerActor->GetActorLocation();
	FCellIndex OriginalCell, DestinationCell;
	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(OriginalPos, OriginalCell);
	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(DestinationPos, DestinationCell);

	if (OriginalCell == DestinationCell)
		return true;

	if (CheckDestinationCellIsValid(DestinationCell) == false)
		return false;

	bSuccessPathFinding = PathFindSourceToTarget(OriginalCell, DestinationCell);

	if (bSuccessPathFinding == false)
	{
		bSuccessPathFinding = FindNewMinimumDestination(OriginalCell);
	}


	GetResultPath(InResultPath);
	ClearObjectPool();

	return bSuccessPathFinding;
}

bool UPathFindingComponent::FindNewMinimumDestination(FCellIndex& InOriginalCell)
{
	auto DestinationCell = MinimumDestinationGridInfo->MyCellIndex;
	if (InOriginalCell == DestinationCell)
		return true;

	ClearObjectPool();
	auto bSuccessNewPathFinding = PathFindSourceToTarget(InOriginalCell, DestinationCell);
	if (bSuccessNewPathFinding)
	{
		SetBlackBoardTargetLocation(DestinationCell);
	}

	return bSuccessNewPathFinding;
}

bool UPathFindingComponent::CheckDestinationCellIsValid(FCellIndex& InDestinationCell)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	auto DestinationCellInfo = RGameInstance->GridMapManager->GetMapInfo(InDestinationCell.XIdx, InDestinationCell.YIdx);
	if (DestinationCellInfo.CellInfo == EMapCellStateEnum::VE_Movable)
		return true;

	return FindNearestPivotCellWithTargetCell(InDestinationCell, InDestinationCell);
}

bool UPathFindingComponent::FindNearestPivotCellWithTargetCell(FCellIndex& InPivotCell, FCellIndex& InTargetCell)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	auto DepthOneOffsets = RGameInstance->GridMapManager->GetDepthOneOffSets();

	TQueue<FBfs_Cell_Info>	BFS_Queue;
	TSet<int32>				CheckCells;
	FBfs_Cell_Info			CellElem;
	FBfs_Cell_Info			FindCellInfo;
	bool					FindDestination = false;
	int32					MinDistBetweenTargetAndFindCell = MAX_int32;
	
	CellElem.CellIndex	= InPivotCell;
	CellElem.Width		= 0;
	
	BFS_Queue.Enqueue(CellElem);
	CheckCells.Emplace(InPivotCell.GetCustomHashValue());
	while(BFS_Queue.IsEmpty() == false)
	{
		if (BFS_Queue.Dequeue(CellElem) == false)
			break;

		auto CellElemInfo = RGameInstance->GridMapManager->GetMapInfo(CellElem.CellIndex.XIdx, CellElem.CellIndex.YIdx);
		if (CellElem.CellIndex.IsValidCellIndex() &&
			(CellElemInfo.CellInfo != EMapCellStateEnum::VE_Invalid) &&
			(CellElemInfo.CellInfo != EMapCellStateEnum::VE_None)
			)
		{
			if (!FindDestination || (MinDistBetweenTargetAndFindCell > InTargetCell.GetCellDistCost(CellElem.CellIndex)))
			{
				FindDestination = true;
				FindCellInfo = CellElem;
				MinDistBetweenTargetAndFindCell = InTargetCell.GetCellDistCost(CellElem.CellIndex);
			}
		}

		for (auto DepthOffsetELem : DepthOneOffsets)
		{
			FCellIndex AdjacentGrid = GetAdjecentCellAroundTargetCell(CellElem.CellIndex, DepthOffsetELem);
			auto AdjacentGridInfo	= RGameInstance->GridMapManager->GetMapInfo(AdjacentGrid.XIdx, AdjacentGrid.YIdx);
			
			if (AdjacentGrid.IsValidCellIndex() == false)
				continue;

			if (CheckCells.Contains(AdjacentGrid.GetCustomHashValue()))
				continue;
			
			FBfs_Cell_Info				AdjacentCellInfo;
			AdjacentCellInfo.CellIndex	= AdjacentGrid;
			AdjacentCellInfo.Width		= CellElem.Width + 1;

			if (!FindDestination || (MinDistBetweenTargetAndFindCell > InTargetCell.GetCellDistCost(AdjacentGrid)))
			{
				BFS_Queue.Enqueue(AdjacentCellInfo);
				CheckCells.Emplace(AdjacentGrid.GetCustomHashValue());
			}
		}
	}
	 
	if (FindDestination == false)
	{
		InPivotCell = FCellIndex(-1, -1);
	}
	else
	{ 
		InPivotCell = FindCellInfo.CellIndex;
	}

	//SetBlackBoardTargetLocation(InTargetCell);
	 
	return FindDestination;
}

void UPathFindingComponent::SetBlackBoardTargetLocation(FCellIndex& InCellIndex)
{
	auto OwnerActor = GetOwner();
	if (IsValid(OwnerActor) == false)
		return;

	auto SpawnedCharacter_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor);
	if (SpawnedCharacter_EntityAIComponent)
	{
		if (InCellIndex.XIdx == -1 && InCellIndex.YIdx == -1)
		{
			SpawnedCharacter_EntityAIComponent->SetBlackboardForceTargetLocation(FVector::ZeroVector);
		}
		else
		{
			auto CellWorldPos = GetDestinationPosByCellIndex(InCellIndex);
			SpawnedCharacter_EntityAIComponent->SetBlackboardForceTargetLocation(CellWorldPos);
		}
	}
}

void UPathFindingComponent::GetResultPath(TArray<FVector>& InResultPath)
{
	InResultPath.Empty();

	for (int32 PathIndex = Path.Num() - 1; PathIndex >= 0; --PathIndex)
	{
		if (Path.IsValidIndex(PathIndex) == false)
			continue;

		auto DestinationPos = GetDestinationPosByCellIndex(Path[PathIndex]->MyCellIndex);
		InResultPath.Emplace(DestinationPos);
	}
}

bool UPathFindingComponent::PathFindSourceToTarget(FCellIndex InSourceCellIndex, FCellIndex InTargetCellIndex)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	if (InSourceCellIndex.IsValidCellIndex() == false || InTargetCellIndex.IsValidCellIndex() == false)
		return false;

	SourceCellIndex = InSourceCellIndex;
	TargetCellIndex = InTargetCellIndex;

	if (InitPathFindingContainer() == false)
		return false;

	bSuccessPathFinding = AStarPathFinding();
	if (bSuccessPathFinding == true)
	{
		SetMinimumDistancePath();
#ifdef DRAW_DEBUG_BOX
		DrawGridBox();
#endif
	}

	return bSuccessPathFinding;
}

void UPathFindingComponent::ClearObjectPool()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->ObjectPoolManager) == false)
		return;

	auto MapNodeObjectPool = Cast<UMapNodeObjectPool>(RGameInstance->ObjectPoolManager->GetObjectPoolByType(EObjectPoolTypeEnum::VE_GridDistanceInfo));
	if (IsValid(MapNodeObjectPool) == false)
		return;

	MapNodeObjectPool->ReturnAllObject();
}

bool UPathFindingComponent::InitPathFindingContainer()
{
	if (InitOpenList() == false)
		return false;

	if (PriorityQueue_OpenList.Num() > 0)
		MinimumDestinationGridInfo = PriorityQueue_OpenList[0];
	else
		return false;

	return true;
}

UGridDistanceInfo* UPathFindingComponent::GetNewGridDistanceInfo(const FCellIndex& CellIndex)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->ObjectPoolManager) == false)
		return nullptr;

	auto MapNodeObjectPool = Cast<UMapNodeObjectPool>(RGameInstance->ObjectPoolManager->GetObjectPoolByType(EObjectPoolTypeEnum::VE_GridDistanceInfo));
	if (IsValid(MapNodeObjectPool) == false)
		return nullptr;

	auto NewGridDistanceInfo = Cast<UGridDistanceInfo>(MapNodeObjectPool->GetUnusedNode(CellIndex.XIdx, CellIndex.YIdx));

	return NewGridDistanceInfo;
}

bool UPathFindingComponent::InitOpenList()
{
	if (SourceCellIndex.IsValidCellIndex() == false) return false;

	//Priority_Queue
	PriorityQueue_OpenList.Empty();
	UGridDistanceInfo* SourceGridInfo = GetNewGridDistanceInfo(SourceCellIndex);
	if (SourceGridInfo)
	{
		SourceGridInfo->CurrentDistance = 0;
		SourceGridInfo->MyCellIndex		= SourceCellIndex;
		SourceGridInfo->ParentCellIndex = FCellIndex(-1, -1);
		SourceGridInfo->RemainDistance	= 0;
		SourceGridInfo->Sum				= 1000000000;

		AddCellOpenList(SourceGridInfo);

		return true;
	}

	return false;

}

bool UPathFindingComponent::AStarPathFinding()
{
	bool bFindPath = false;

	while(PriorityQueue_OpenList.Num() > 0)
	{
		auto MinimumGrid = FindMinimumDistanceCellInOpenList();
		SaveMinimumDestinationCell(MinimumGrid);

		if (MinimumGrid == nullptr)									break;
		if (MinimumGrid->MyCellIndex.IsValidCellIndex() == false)	break;

		AddCellInCloseList(MinimumGrid);

		if (IsFindTargetCell(MinimumGrid->MyCellIndex) == true)
		{
			bFindPath = true;

			break;
		}
		
		InvalidateDistance(MinimumGrid);
	}

	return bFindPath;
}

void UPathFindingComponent::SaveMinimumDestinationCell(UGridDistanceInfo* InGridDistanceInfo)
{
	if (IsValid(InGridDistanceInfo) == false)
		return;

	if (MinimumDestinationGridInfo->Sum > InGridDistanceInfo->Sum || 
		((MinimumDestinationGridInfo->Sum == InGridDistanceInfo->Sum) && (MinimumDestinationGridInfo->RemainDistance > InGridDistanceInfo->RemainDistance))
		)
	{
		MinimumDestinationGridInfo	= InGridDistanceInfo;
		MinimumCellIndex			= MinimumDestinationGridInfo->MyCellIndex;
	}
}

void UPathFindingComponent::AddCellOpenList(UGridDistanceInfo* InGridInfo)
{
	if (InGridInfo == nullptr) return;
	
	InGridInfo->GridType = EGridType::VE_OpenGrid;
	
	//Priorit_Queue
	PriorityQueue_OpenList.HeapPush(InGridInfo, GridInfoPredicate());
}

void UPathFindingComponent::SearchAdjacentCell(FCellIndex& PivotCell, TArray<FCellIndex>& InAdjacentCellList)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return;

	InAdjacentCellList.Empty();

	auto DepthOneOffsets = RGameInstance->GridMapManager->GetDepthOneOffSets();
	for (auto DepthOffsetELem : DepthOneOffsets)
	{
		FCellIndex AdjacentGrid = GetAdjecentCellAroundTargetCell(PivotCell, DepthOffsetELem);

		if (AdjacentGrid.IsValidCellIndex() == false)
			continue;
		if (IsValidLocation(AdjacentGrid) == false)
			continue;

		InAdjacentCellList.Emplace(AdjacentGrid);
	}
}


void UPathFindingComponent::InvalidateDistance(UGridDistanceInfo* ParentCell)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->ObjectPoolManager) == false)
		return;

	TArray<FCellIndex> AdjacentCellList;
	SearchAdjacentCell(ParentCell->MyCellIndex, AdjacentCellList);

	for (auto& AdjacentCellElem : AdjacentCellList)
	{
		auto NewDistanceInfo = GetNewGridDistanceInfo(AdjacentCellElem);
		if (NewDistanceInfo == nullptr) 
			continue;

		switch (NewDistanceInfo->GridType)
		{
		case EGridType::VE_None:
			NewDistanceInfo->CalcCostFromParent(AdjacentCellElem, TargetCellIndex, ParentCell);
			AddCellOpenList(NewDistanceInfo);
			break;
		case EGridType::VE_OpenGrid:
			if (NewDistanceInfo->CurrentDistance > ParentCell->CurrentDistance + AdjacentCellElem.GetCellDistCost(ParentCell->MyCellIndex))
			{
				NewDistanceInfo->CalcCostFromParent(AdjacentCellElem, TargetCellIndex, ParentCell);
				AddCellOpenList(NewDistanceInfo);
			}
			break;
		case EGridType::VE_CloseGrid:
			break;
		default:
			break;
		}
	}
}

UGridDistanceInfo* UPathFindingComponent::FindMinimumDistanceCellInOpenList()
{
	if (PriorityQueue_OpenList.Num() > 0)
	{
		UGridDistanceInfo* MinimumDistanceGrid = nullptr;
		PriorityQueue_OpenList.HeapPop(MinimumDistanceGrid, GridInfoPredicate());
		return MinimumDistanceGrid;
	}
	return nullptr;
}

void UPathFindingComponent::SetMinimumDistancePath()
{
	Path.Empty();
	UGridDistanceInfo* ParentGridInfo = GetNewGridDistanceInfo(TargetCellIndex);
	if (ParentGridInfo == nullptr)
		return;

	while (!(ParentGridInfo->MyCellIndex == SourceCellIndex))
	{
		Path.Emplace(ParentGridInfo);
		ParentGridInfo->GridType = EGridType::VE_Path;
		ParentGridInfo = GetNewGridDistanceInfo(ParentGridInfo->ParentCellIndex);
		if (ParentGridInfo == nullptr)
			return;
	}

	ParentGridInfo = GetNewGridDistanceInfo(SourceCellIndex);
	if (ParentGridInfo == nullptr)
		return;

	ParentGridInfo->GridType = EGridType::VE_Path;
	Path.Emplace(ParentGridInfo);
}

void UPathFindingComponent::DrawGridBox()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->ObjectPoolManager) == false)
		return;

	auto MapNodeObjectPool = Cast<UMapNodeObjectPool>(RGameInstance->ObjectPoolManager->GetObjectPoolByType(EObjectPoolTypeEnum::VE_GridDistanceInfo));
	if (IsValid(MapNodeObjectPool) == false)
		return;

	FlushPersistentDebugLines(this->GetWorld());
	MapNodeObjectPool->DrawGridBox(SourceCellIndex, TargetCellIndex);
}

bool UPathFindingComponent::IsFindTargetCell(FCellIndex& CellIndex)
{
	if (TargetCellIndex == CellIndex)
		return true;

	return false;
}

FCellIndex UPathFindingComponent::GetAdjecentCellAroundTargetCell(FCellIndex& InTargetCellIndex, FCellIndex& InCellIndexOffset)
{
	return FCellIndex(InTargetCellIndex.XIdx + InCellIndexOffset.XIdx, InTargetCellIndex.YIdx + InCellIndexOffset.YIdx);
}

bool UPathFindingComponent::IsValidLocation(FCellIndex& CellIndex)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager)) return false;

	auto GridStateInfo = RGameInstance->GridMapManager->GetMapInfo(CellIndex.XIdx, CellIndex.YIdx);
	if (GridStateInfo.CellInfo != EMapCellStateEnum::VE_Movable)
		return false;

	return true;
}

void UPathFindingComponent::AddCellInCloseList(UGridDistanceInfo* GridInfo)
{
	if (IsValid(GridInfo))
		GridInfo->GridType = EGridType::VE_CloseGrid;
}

void UGridDistanceInfo::CalcCostFromParent(FCellIndex& InMyCellIndex, FCellIndex& DestCell, UGridDistanceInfo* InParent)
{
	if (InParent == nullptr)
		return;

	ParentCellIndex	= InParent->MyCellIndex;
	MyCellIndex		= InMyCellIndex;

	CurrentDistance = MyCellIndex.GetCellDistCost(ParentCellIndex) + InParent->CurrentDistance;
	FVector2D MyVector = FVector2D(MyCellIndex.XIdx, MyCellIndex.YIdx);
	FVector2D DestVector = FVector2D(DestCell.XIdx, DestCell.YIdx);
	RemainDistance = FVector2D::DistSquared(MyVector, DestVector) * WEIGHT_VALUE;
	Sum = CurrentDistance + RemainDistance;
}

bool UPathFindingComponent::GetNearestPositionArountPivotPosition(const FVector& Destination, FVector& NearestPosition)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	FCellIndex PivotCell;
	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(Destination, PivotCell);
	if (FindNearestPivotCellWithTargetCell(PivotCell, PivotCell) == false)
		return false;

	RGameInstance->GridMapManager->GetWorldPosFromCellIndex(PivotCell, NearestPosition);
	NearestPosition.Z = Destination.Z;
	
	return true;
}

bool UPathFindingComponent::GetNearestPositionAroundPivotPositionWithTarget(AActor* TargetOwner, const FVector& Destination, FVector& NearestPosition)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	FCellIndex PivotCell, TargetCell;
	if (RGameInstance->GridMapManager->GetCellIndexFromWorldPos(Destination, PivotCell) == false)
		return false;

	if (RGameInstance->GridMapManager->GetCellIndexFromWorldPos(TargetOwner->GetActorLocation(), TargetCell) == false)
		return false;

	if (FindNearestPivotCellWithTargetCell(PivotCell, TargetCell) == false)
		return false;

	RGameInstance->GridMapManager->GetWorldPosFromCellIndex(PivotCell, NearestPosition);
	NearestPosition.Z = Destination.Z;

	return true;
}