// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "Character/Components/FunctionalComponents/PathFindingComponent.h"
#include "MapNodeObjectPool.h"
#define XMultiplierValue 100000

void UMapNodeObjectPool::Initialize(int32 Size)
{
	PoolSize = 200;
	Super::Initialize(PoolSize);
}

UObject* UMapNodeObjectPool::GetUnusedNode(const int& XIdx, const int& YIdx)
{
	if (UnusedPool.Num() <= 0)
		return nullptr;

	int32 HashValue = GetHashValue(XIdx, YIdx);
	if (UsedPoolMap.Contains(HashValue) == true)
	{
		return UsedPoolMap[HashValue];
	}
	else
	{
		UObject* UnusedObject = UnusedPool.Last();

		if (IsValid(UnusedObject))
		{
			auto RemoveObjectNumber = UnusedPool.Remove(UnusedObject);
			//int32 HashValue = GetHashValue(XIdx, YIdx);
			UsedPoolMap.Emplace(HashValue, UnusedObject);

			return UnusedObject;
		}
	}

	return nullptr;
}

void UMapNodeObjectPool::ReturnObject(UObject* TargetObject)
{
	if (IsValid(TargetObject))
	{
		auto GridDistanceInfo = Cast<UGridDistanceInfo>(TargetObject);
		if (IsValid(GridDistanceInfo))
		{
			int32 HashValue = GetHashValue(GridDistanceInfo->MyCellIndex.XIdx, GridDistanceInfo->MyCellIndex.YIdx);
			auto RemoveObjectNumber = UsedPoolMap.Remove(HashValue);
			UnusedPool.Emplace(TargetObject);
		}
	}
}

void UMapNodeObjectPool::ReturnAllObject()
{
	for (auto& UsedElem : UsedPoolMap)
	{
		UnusedPool.Emplace(UsedElem.Value);
		auto GridDistanceInfo = Cast<UGridDistanceInfo>(UsedElem.Value);
		if (IsValid(GridDistanceInfo))
		{
			GridDistanceInfo->Clear();
		}
	}

	UsedPoolMap.Empty();
}

int32 UMapNodeObjectPool::GetHashValue(const int& XIdx, const int& YIdx)
{
	return XIdx * XMultiplierValue + YIdx;
}

void UMapNodeObjectPool::DrawGridBox(const FCellIndex& SourceCellIndex, const FCellIndex& TargetCellIndex)
{
	for (auto& UsedGridElem : UsedPoolMap)
	{
		auto GridDistanceInfo = Cast<UGridDistanceInfo>(UsedGridElem.Value);
		if (IsValid(GridDistanceInfo) == false)
			continue;

		if (GridDistanceInfo->MyCellIndex == SourceCellIndex || GridDistanceInfo->MyCellIndex == TargetCellIndex)
		{
			DrawDebugGrid(GridDistanceInfo, FColor::Cyan);
			continue;
		}

		switch (GridDistanceInfo->GridType)
		{
		case EGridType::VE_OpenGrid:
			DrawDebugGrid(GridDistanceInfo, FColor::Yellow);
			break;
		case EGridType::VE_CloseGrid:
			DrawDebugGrid(GridDistanceInfo, FColor::Red);
			break;
		case EGridType::VE_Path:
			DrawDebugGrid(GridDistanceInfo, FColor::Blue);
		default:
			break;
		}
	}
	DrawDebugGridByCell(SourceCellIndex, FColor::Black);
	DrawDebugGridByCell(TargetCellIndex, FColor::Cyan);
}

void UMapNodeObjectPool::DrawDebugGridByCell(const FCellIndex& CellIndex, FColor InDrawColor)
{
	int HashValue = GetHashValue(CellIndex.XIdx, CellIndex.YIdx);
	auto GridDistanceInfo = Cast<UGridDistanceInfo>(UsedPoolMap[HashValue]);

	if (IsValid(GridDistanceInfo))
	{
		DrawDebugGrid(GridDistanceInfo, InDrawColor);
	}
}

void UMapNodeObjectPool::DrawDebugGrid(UGridDistanceInfo* DrawTarget, FColor InDrawColor)
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	if (!IsValid(RGameInstance->GridMapManager)) return;

	if (IsValid(DrawTarget) == false)
		return;
	
	auto CellSize = RGameInstance->GridMapManager->GetCellSizeBymapStatus();
	CellSize *= 0.5f;
	auto CellWorldLocation = RGameInstance->GridMapManager->GetCellWorldPosByMapStatus(DrawTarget->MyCellIndex.XIdx, DrawTarget->MyCellIndex.YIdx);
	CellWorldLocation.X += CellSize;
	CellWorldLocation.Y += CellSize;
	DrawDebugBox(RGameInstance->GetWorld(), CellWorldLocation, FVector(CellSize, CellSize, 1), InDrawColor, true, 100000.0f, UINT8_MAX);
}

