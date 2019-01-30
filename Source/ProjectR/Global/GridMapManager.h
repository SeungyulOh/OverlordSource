// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "CustomStructures/GridMapDataInfo.h"
#include "CustomActors/MapCreatorActor.h"
#include "GridMapManager.generated.h"

enum class EGridType : uint8
{
	VE_None,
	VE_OpenGrid,
	VE_CloseGrid,
	VE_Path,
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCellIndex
{
	GENERATED_USTRUCT_BODY()
public:
	FCellIndex() {}
	FCellIndex(int32 InXIdx, int32 InYIdx)
	{
		XIdx = InXIdx;
		YIdx = InYIdx;
	}

	UPROPERTY()
	int32		XIdx;
	UPROPERTY()
	int32		YIdx;

	int32 GetCellDistCost(const FCellIndex& ToCell)
	{
		int32 xCost = FMath::Abs(this->XIdx - ToCell.XIdx);
		int32 yCost = FMath::Abs(this->YIdx - ToCell.YIdx);

		return xCost + yCost;
	}
	bool IsValidCellIndex()
	{
		if (XIdx < 0) return false;
		if (YIdx < 0) return false;
		return true;
	}

	int32 GetCustomHashValue()
	{
		return XIdx * 100000 + YIdx;
	}
	bool operator == (const FCellIndex& CompairTarget)
	{
		if (XIdx != CompairTarget.XIdx) return false;
		if (YIdx != CompairTarget.YIdx) return false;
		return true;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FBfs_Cell_Info
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FCellIndex	CellIndex;
	UPROPERTY()
	int32		Width = 0;
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMapCellOffsetDepthEnum : uint8
{
	VE_Depth0			UMETA(DisplayName = "Depth0"),
	VE_Depth1			UMETA(DisplayName = "Depth1"),
	VE_Depth2			UMETA(DisplayName = "Depth2"),	
	VE_Invalid			UMETA(DisplayName = "Invalid"),
};

/**
*
*/
UCLASS()
class PROJECTR_API UGridMapStatus : public UObject
{
	GENERATED_BODY()

public:

	void		InitFromMapCreatorActor(AMapCreatorActor* FromActor);
	FORCEINLINE AMapCreatorActor* GetMapCreatorActor() { return MapCreatorActor.Get(); }

	float		GetCellSize();

	int32		GetMapSizeX();
	int32		GetMapSizeY();
	FVector2D	GetMapSize();

	FVector		GetStartPos();
	FVector		GetCellWorldPos(int32 XIdx, int32 YIdx);

	bool		IsValidMapCell(int32 XIdx, int32 YIdx);
	bool		IsValidMapStatusCell(int32 XIdx, int32 YIdx);

	void				SetMapInfo(int32 XIdx, int32 YIdx, FMapCellData InMapCellData);
	FMapCellData		GetMapInfo(int32 XIdx, int32 YIdx);

	bool				ReserveMovingTarget(FCellIndex& InTargetIndics, AActor* TargetActor);
	void				ClearReserveMovingTarget(FCellIndex& InTargetIndics);
		

private:

	UPROPERTY()
	TWeakObjectPtr< AMapCreatorActor >		MapCreatorActor;
	UPROPERTY()
	TArray< FGridMapStateInfo >				MapStatusInfoList;					// for runtime data..

	UPROPERTY()
	bool									bValid = false;
};


/**
 * 
 */
UCLASS()
class PROJECTR_API UGridMapManager : public UObject
{
	GENERATED_BODY()

public:

	void				Initialize(/*class URGameInstance* RGameInstance*/);
	
	UFUNCTION(BlueprintCallable, Category = GridMapManager)
	bool				LoadMapInfoFromActor();
	bool				LoadMapInfoFromFile();

	void				LeaveFromPlayLevel();

//	void				InitMapData(int32 Width, int32 Height, FVector InStartPos, float InCellSize);
	void				SetMapInfo(int32 XIdx, int32 YIdx, FMapCellData MapCellData);
	FMapCellData		GetMapInfo(int32 XIdx, int32 YIdx);
	bool				SaveMapInfo(FString SaveFileName);
	
	bool				ReserveMovingTarget(FCellIndex& InTargetIndics, AActor* TargetActor );
	void				ClearReserveMovingTarget(FCellIndex& InTargetIndics );

	FVector				GetCellWorldPos( FCellIndex& InTargetIndics );
		
	//------------------------------------------------------------------------
	// Check
	//------------------------------------------------------------------------
	bool				UpdateCellInfo(FCellIndex PrevIdx, FVector InWorldPos, FCellIndex& OutNewIndics);
	UFUNCTION(BlueprintCallable, Category = GridMapManager)
	bool				GetCellIndexFromWorldPos(FVector InWorldPos, FCellIndex& OutIndics);
	void				GetWorldPosFromCellIndex(FCellIndex& InIndics, FVector& OutWorldPos);
	FMapCellData		GetMapInfoFromWorldPos(FVector InWorldPos);

	bool				AreTwoCellsAdjacent(FCellIndex& InSourceCell, FCellIndex& InTargetCell);
	bool				AreTwoCellsAdjacentFromWorldPos(FVector InSrcPos, FVector InTargetPos );
	//------------------------------------------------------------------------
	// AI
	//------------------------------------------------------------------------
	FCellIndex			GetNearestCellToTargetCellFromSource(FCellIndex& SrcCellIdx, FCellIndex& TargetCellIdx, int32 InTargetCellState = ENUM_TO_BITFIELD((int32)EMapCellStateEnum::VE_Movable));
	FVector				GetNearestLocationToTargetCellFromSource(FCellIndex& SrcCellIdx, FCellIndex& TargetCellIdx, int32 InTargetCellState = ENUM_TO_BITFIELD((int32)EMapCellStateEnum::VE_Movable));
	FVector				GetNearestPointToTargetCellFromSourceWorldPos(FVector SrcLocation, FVector TargetLocation, int32 InTargetCellState = ENUM_TO_BITFIELD((int32)EMapCellStateEnum::VE_Movable));
	void				GetAdjacentCells(TArray< FCellIndex > &OutCells, FCellIndex& TargetCellIdx, int32 InSearchDepth = ENUM_TO_BITFIELD((int32)EMapCellOffsetDepthEnum::VE_Depth1), int32 InTargetCellState = ENUM_TO_BITFIELD( (int32)EMapCellStateEnum::VE_Movable ), bool InbAddTargetCell = false );
	void				GetNearestCellIdx(FCellIndex& OutNearestCellIdx, TArray< FCellIndex > &TargetCellIdxs, FCellIndex& SrcCellIdx );
	int32				GetCellDistCost(FCellIndex FromCell, FCellIndex ToCell);

	FVector				GetCellSnapPos(FVector WorldPos);
	
	int32				GetCellDepthFromDistance(float InDistance);
	float				GetFarthestMovableDistance(const FVector& StartPos, const FVector& Direction, float Distance);


	//StartPos	: Actor Current Location
	//EndPos	: Actor Destination Location
	//ValidPos	: Valid Location that nearest with EndPos
	//bSweep	: true - Find Front Grid to End, false - Find End Grid to Front.
	//ex: teleport's bSweep value is false, than counter attack's bSweep value is true
	bool				GetValidLocationAroundPivotLocation(const FVector& StartPos, const FVector& EndPos, FVector& ValidPos, bool bSweep = true);

	
	bool				FindNearestPivotCellWithTargetCell(FCellIndex& InPivotCell, FCellIndex& InTargetCell);
	FCellIndex			GetAdjecentCellAroundTargetCell(FCellIndex& InTargetCellIndex, FCellIndex& InCellIndexOffset);

	FORCEINLINE const TArray< FCellIndex >& GetDepthOneOffSets() { return DepthOneOffsets; }
	FORCEINLINE FVector2D					GetMapSizeByMapStatus() { return MapStatus->GetMapSize(); }
	FORCEINLINE FVector						GetCellWorldPosByMapStatus(int32 XIdx, int32 YIdx) { return MapStatus->GetCellWorldPos(XIdx, YIdx); }
	float						GetCellSizeBymapStatus();
	bool						IsValidMapStatus();
	// for debugging
	void				DrawDebugInfo();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	bool				bShowDebugInfo = false;
	bool				IsTargetGridValidByWorldPos(const FVector& WorldPos);
private:
	void				InitDepthOffset();
	bool				GetValidHeight(const FVector& Pos, float& Height);
private:
// 	UPROPERTY()
// 	URGameInstance*				GameInstance = nullptr;						// Gamestate class can't access GetGameInstance.. because GetWorld is invalid..
	UPROPERTY()
	UGridMapStatus*				MapStatus = nullptr;
	
	UPROPERTY()
	bool						bInit = false;

	UPROPERTY()
	TArray< FCellIndex >		DepthOneOffsets;
	UPROPERTY()
	TArray< FCellIndex >		DepthTwoOffsets;


};