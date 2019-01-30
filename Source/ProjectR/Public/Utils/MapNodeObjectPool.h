// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/ObjectPool.h"
#include "Global/GridMapManager.h"
#include "Character/Components/FunctionalComponents/PathFindingComponent.h"
#include "MapNodeObjectPool.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UMapNodeObjectPool : public UObjectPool
{
	GENERATED_BODY()
	
public:
	void			Initialize(int32 Size = 50) override;
	void			ReturnAllObject();
	void			ReturnObject(UObject* TargetObject) override;
	UObject*		GetUnusedNode(const int& XIdx, const int& YIdx);
	void			DrawGridBox(const FCellIndex& SourceCellIndex, const FCellIndex& TargetCellIndex);
private:
	void			DrawDebugGrid(UGridDistanceInfo* DrawTarget, FColor DrawColor);
	void			DrawDebugGridByCell(const FCellIndex& CellIndex, FColor InDrawColor);
	int32			GetHashValue(const int& XIdx, const int& YIdx);

	UPROPERTY()
	TMap<int32, UObject*> UsedPoolMap;

	UPROPERTY()
	FCellIndex		MinimumDestinationCell;
};
