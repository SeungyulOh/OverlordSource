// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GridMapManager.h"
#include "RGameInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "UtilFunctionIntegrated.h"

//#define DRAW_DEBUG

void UGridMapManager::Initialize(/*URGameInstance* RGameInstance*/)
{ 
//	GameInstance = RGameInstance; 

	bInit = false;
	
	InitDepthOffset();
}

void UGridMapManager::InitDepthOffset()
{
	// 1- depth
	DepthOneOffsets.Empty(8);
	DepthOneOffsets.Emplace(FCellIndex(-1, -1));
	DepthOneOffsets.Emplace(FCellIndex( 0, -1));
	DepthOneOffsets.Emplace(FCellIndex( 1, -1));
	DepthOneOffsets.Emplace(FCellIndex(-1,  0));
	DepthOneOffsets.Emplace(FCellIndex( 1,  0));
	DepthOneOffsets.Emplace(FCellIndex(-1,  1));
	DepthOneOffsets.Emplace(FCellIndex( 0,  1));
	DepthOneOffsets.Emplace(FCellIndex( 1,  1));

	// 2- depth
	DepthTwoOffsets.Empty(16);
	DepthTwoOffsets.Emplace(FCellIndex(-2, -2 ));
	DepthTwoOffsets.Emplace(FCellIndex(-1, -2));
	DepthTwoOffsets.Emplace(FCellIndex( 0, -2));
	DepthTwoOffsets.Emplace(FCellIndex( 1, -2));
	DepthTwoOffsets.Emplace(FCellIndex( 2, -2));

	DepthTwoOffsets.Emplace(FCellIndex(-2, -1));
	DepthTwoOffsets.Emplace(FCellIndex( 2, -1));

	DepthTwoOffsets.Emplace(FCellIndex(-2,  0));
	DepthTwoOffsets.Emplace(FCellIndex( 2,  0));

	DepthTwoOffsets.Emplace(FCellIndex(-2,  1));
	DepthTwoOffsets.Emplace(FCellIndex( 2,  1));

	DepthTwoOffsets.Emplace(FCellIndex(-2,  2));
	DepthTwoOffsets.Emplace(FCellIndex(-1,  2));
	DepthTwoOffsets.Emplace(FCellIndex( 0,   2));
	DepthTwoOffsets.Emplace(FCellIndex( 1,   2));
	DepthTwoOffsets.Emplace(FCellIndex( 2,   2));

}

bool UGridMapManager::LoadMapInfoFromActor()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if (GameInstance == nullptr)
		return false;
#if LOG_TIME
	double StartMainTime = FPlatformTime::Seconds();
#endif
	bool bIsExistMapCreator = false;
	for (TActorIterator<AMapCreatorActor> ActorItr(GameInstance->GetWorld()); ActorItr; ++ActorItr)
	{
		AMapCreatorActor* MapCreator = *ActorItr;		
		if (IsValid(MapCreator))
		{
			MapStatus = NewObject<UGridMapStatus>(this);
			MapStatus->InitFromMapCreatorActor(MapCreator);

			bInit = true;

#if LOG_TIME
			double EndTime = FPlatformTime::Seconds();
			double PrintTime = EndTime - StartMainTime;
			UE_LOG(LogGamePlay, Log, TEXT("%f ____UGridMapManager::LoadMapInfoFromActor()"), PrintTime);
#endif
			bIsExistMapCreator = true;
			return true;
		}
	}
	checkf(bIsExistMapCreator, TEXT("Not Exist MapCreator!!!!!!"));
	
#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartMainTime;
	UE_LOG(LogGamePlay, Log, TEXT("%f ____UGridMapManager::LoadMapInfoFromActor()"), PrintTime);
#endif

	return false;
}

void UGridMapManager::LeaveFromPlayLevel()
{
	if (MapStatus)
	{
		MapStatus->ConditionalBeginDestroy();
	}
	MapStatus = nullptr;
}

bool UGridMapManager::LoadMapInfoFromFile()
{
	return true;
}

void UGridMapManager::SetMapInfo(int32 XIdx, int32 YIdx, FMapCellData InMapCellData)
{
	if ( IsValid( MapStatus ) )
		MapStatus->SetMapInfo(XIdx, YIdx, InMapCellData);
}

FMapCellData UGridMapManager::GetMapInfo(int32 XIdx, int32 YIdx)
{
	if (IsValid(MapStatus))
		return MapStatus->GetMapInfo(XIdx, YIdx );

	return FMapCellData();
}

bool UGridMapManager::SaveMapInfo(FString SaveFileName)
{
	/*
	if (MapStatus.IsValid() == false)
		return false;

	FGridMapDataInfo GridMapInfo;
	MapCreatorActor->MakeGridMapInfoData(GridMapInfo);

	FString OutJsonString;
	FJsonHelper::UStructToJsonObjectString(FGridMapDataInfo::StaticStruct(), &GridMapInfo, OutJsonString, 0, 0);
	FString FullPath = FPaths::GameSavedDir() + SaveFileName;
	if (FFileHelper::SaveStringToFile(OutJsonString, *FullPath))
		return true;
	*/
	return false;
}

bool UGridMapManager::ReserveMovingTarget(FCellIndex& InTargetIndics, AActor* TargetActor)
{
	if (IsValid(MapStatus))
	{
		return MapStatus->ReserveMovingTarget(InTargetIndics, TargetActor);
	}
	return false;
}

void UGridMapManager::ClearReserveMovingTarget(FCellIndex& InTargetIndics)
{
	if (IsValid(MapStatus))
		MapStatus->ClearReserveMovingTarget(InTargetIndics);
}

FVector	UGridMapManager::GetCellWorldPos(FCellIndex& InTargetIndics)
{
	if (IsValid(MapStatus))
		return MapStatus->GetCellWorldPos(InTargetIndics.XIdx, InTargetIndics.YIdx);
	return FVector::ZeroVector;
}

bool UGridMapManager::UpdateCellInfo( FCellIndex PrevIdx, FVector InWorldPos, FCellIndex& OutNewIndics )
{
	if (IsValid(MapStatus) == false)
		return false;

	FCellIndex CellIndics;
	GetCellIndexFromWorldPos(InWorldPos, CellIndics);

	if (CellIndics.XIdx == OutNewIndics.XIdx && CellIndics.YIdx == OutNewIndics.YIdx)
		return false;

	if(MapStatus->IsValidMapCell( CellIndics.XIdx, CellIndics.YIdx ) == false )	
		return false;

	FMapCellData PrevCellDataState = MapStatus->GetMapInfo( CellIndics.XIdx, CellIndics.YIdx);
	if (PrevCellDataState.CellInfo == EMapCellStateEnum::VE_None ||
		PrevCellDataState.CellInfo == EMapCellStateEnum::VE_Occupied)
	{
		OutNewIndics = CellIndics;
		return false;
	}

	if (MapStatus->IsValidMapCell(PrevIdx.XIdx, PrevIdx.YIdx))
	{
		auto CellData		= GetMapInfo(PrevIdx.XIdx, PrevIdx.YIdx);
		CellData.CellInfo	= EMapCellStateEnum::VE_Movable;
		MapStatus->SetMapInfo(PrevIdx.XIdx, PrevIdx.YIdx, CellData);
	}

	auto CellData = GetMapInfo(CellIndics.XIdx, CellIndics.YIdx);
	CellData.CellInfo = EMapCellStateEnum::VE_Occupied;
	MapStatus->SetMapInfo(CellIndics.XIdx, CellIndics.YIdx, CellData);
	OutNewIndics = CellIndics;

	return true;
}

bool UGridMapManager::GetCellIndexFromWorldPos(FVector InWorldPos, FCellIndex& OutIndics )
{
	if ( IsValid( MapStatus ) )
	{		
		FVector RelativePos = InWorldPos - MapStatus->GetStartPos();
		float CellSize = MapStatus->GetCellSize();
		OutIndics.XIdx = RelativePos.X / CellSize;
		OutIndics.YIdx = RelativePos.Y / CellSize;

		if (MapStatus->IsValidMapCell(OutIndics.XIdx, OutIndics.YIdx))
			return true;
	}	

	return false;
}

bool UGridMapManager::AreTwoCellsAdjacent(FCellIndex& InSourceCell, FCellIndex& InTargetCell)
{
	for (FCellIndex& cellIdxOffset : DepthOneOffsets)
	{
		FCellIndex cellIdx;
		cellIdx.XIdx = cellIdxOffset.XIdx + InSourceCell.XIdx;
		cellIdx.YIdx = cellIdxOffset.YIdx + InSourceCell.YIdx;

		if (cellIdx == InTargetCell)
		{
			return true;
		}
	}

	return false;
}

bool UGridMapManager::AreTwoCellsAdjacentFromWorldPos(FVector InSrcPos, FVector InTargetPos)
{
	FCellIndex SrcIdx, TargetIdx;
	GetCellIndexFromWorldPos(InSrcPos, SrcIdx);
	GetCellIndexFromWorldPos(InTargetPos, TargetIdx);

	return AreTwoCellsAdjacent(SrcIdx, TargetIdx);
}

void UGridMapManager::GetWorldPosFromCellIndex(FCellIndex& InIndics, FVector& OutWorldPos)
{
	if (IsValid(MapStatus))
	{
		OutWorldPos = MapStatus->GetCellWorldPos(InIndics.XIdx, InIndics.YIdx);
	}
}

FMapCellData UGridMapManager::GetMapInfoFromWorldPos(FVector InWorldPos)
{
	FCellIndex CellIndics;
	if (GetCellIndexFromWorldPos(InWorldPos, CellIndics))
	{
		//UE_LOG(LogBattle, Log, TEXT("Get map info %s, %d:%d "), *(InWorldPos.ToString()), CellIndics.XIdx, CellIndics.YIdx);
		return GetMapInfo(CellIndics.XIdx, CellIndics.YIdx);
	}
	return FMapCellData();
}

// for debugging
void UGridMapManager::DrawDebugInfo()
{	
	if (bShowDebugInfo == false)
		return;

	if (!bInit)
		return;

	if (IsValid(MapStatus) && IsValid(MapStatus->GetMapCreatorActor() ) )
		MapStatus->GetMapCreatorActor()->DrawDebugInfo();
}


//------------------------------------------------------------------------
// AI
//------------------------------------------------------------------------
FCellIndex UGridMapManager::GetNearestCellToTargetCellFromSource( FCellIndex& SrcCellIdx, FCellIndex& TargetCellIdx, int32 InTargetCellState )
{
	TArray< FCellIndex > AdjacentCells;
	GetAdjacentCells(AdjacentCells, TargetCellIdx, (int32)EMapCellOffsetDepthEnum::VE_Depth0, InTargetCellState);
	if (AdjacentCells.Num() == 0)
	{
		GetAdjacentCells(AdjacentCells, TargetCellIdx, (int32)EMapCellOffsetDepthEnum::VE_Depth1, InTargetCellState);
		if (AdjacentCells.Num() == 0)
			GetAdjacentCells(AdjacentCells, TargetCellIdx, (int32)EMapCellOffsetDepthEnum::VE_Depth2, InTargetCellState);
	}

	if( AdjacentCells.Num() == 0 )
		return SrcCellIdx;

	FCellIndex NearestCellIdx = SrcCellIdx;
	GetNearestCellIdx(NearestCellIdx, AdjacentCells, SrcCellIdx);
	return NearestCellIdx;
	
}

FVector UGridMapManager::GetNearestLocationToTargetCellFromSource(FCellIndex& SrcCellIdx, FCellIndex& TargetCellIdx, int32 InTargetCellState )
{
	if( IsValid(MapStatus ) == false )
		return FVector::ZeroVector;

	float CellSize = MapStatus->GetCellSize();
	FCellIndex NearestCellIdx = GetNearestCellToTargetCellFromSource(SrcCellIdx, TargetCellIdx, InTargetCellState);
	return MapStatus->GetCellWorldPos(NearestCellIdx.XIdx, NearestCellIdx.YIdx) +FVector( CellSize / 2, CellSize / 2, 0);
}

FVector	UGridMapManager::GetNearestPointToTargetCellFromSourceWorldPos(FVector SrcLocation, FVector TargetLocation, int32 InTargetCellState )
{
	if (IsValid(MapStatus) == false)
		return TargetLocation;

	FCellIndex SrcCellIdx;
	GetCellIndexFromWorldPos(SrcLocation, SrcCellIdx );
	FCellIndex TargetCellIdx;
	GetCellIndexFromWorldPos(TargetLocation, TargetCellIdx );

	return GetNearestLocationToTargetCellFromSource(SrcCellIdx, TargetCellIdx);
}

void UGridMapManager::GetNearestCellIdx( FCellIndex& OutNearestCellIdx, TArray< FCellIndex > &TargetCellIdxs, FCellIndex& SrcCellIdx )
{
	int32 minDist = 10000;	
	for (FCellIndex& targetCellIdx : TargetCellIdxs)
	{		
		int32 curCost = GetCellDistCost(SrcCellIdx, targetCellIdx);
		if (curCost < minDist)
		{
			minDist = curCost;
			OutNearestCellIdx = targetCellIdx;
		}
	}	
}

int32 UGridMapManager::GetCellDistCost(FCellIndex FromCell, FCellIndex ToCell)
{
	int32 xCost = FMath::Abs(FromCell.XIdx - ToCell.XIdx);
	int32 yCost = FMath::Abs(FromCell.YIdx - ToCell.YIdx);

	return ( xCost*xCost + yCost*yCost);
}

FVector UGridMapManager::GetCellSnapPos(FVector WorldPos)
{
	if (IsValid(MapStatus) == false)
		return WorldPos;

	FCellIndex OutIndics;
	GetCellIndexFromWorldPos(WorldPos, OutIndics);

	return MapStatus->GetCellWorldPos(OutIndics.XIdx, OutIndics.YIdx);
}

void UGridMapManager::GetAdjacentCells(TArray< FCellIndex > &OutCells, FCellIndex& TargetCellIdx, int32 InCellDepth, int32 InTargetCellState, bool InbAddTargetCell )
{
	OutCells.Empty();
	
	// depth 0	
	if ( InbAddTargetCell )
	{
		auto CellState = GetMapInfo(TargetCellIdx.XIdx, TargetCellIdx.YIdx);
		if (ENUM_TO_BITFIELD((int32)CellState.CellInfo) & InTargetCellState)
			OutCells.Emplace(TargetCellIdx);
	}
	// depth 1
	if ( ENUM_TO_BITFIELD( (int32)(EMapCellOffsetDepthEnum::VE_Depth1) ) & InCellDepth )
	{
		for ( FCellIndex& cellIdxOffset : DepthOneOffsets)
		{
			FCellIndex cellIdx;
			cellIdx.XIdx = cellIdxOffset.XIdx + TargetCellIdx.XIdx;
			cellIdx.YIdx = cellIdxOffset.YIdx + TargetCellIdx.YIdx;

			auto CellState = GetMapInfo(cellIdx.XIdx, cellIdx.YIdx);
			if (ENUM_TO_BITFIELD( (int32)CellState.CellInfo ) & InTargetCellState)
				OutCells.Emplace(cellIdx);
		}
	}

	// depth 2
	if (ENUM_TO_BITFIELD( (int32)(EMapCellOffsetDepthEnum::VE_Depth2)) & InCellDepth)
	{
		for ( FCellIndex& cellIdxOffset : DepthTwoOffsets)
		{
			FCellIndex cellIdx;
			cellIdx.XIdx = cellIdxOffset.XIdx + TargetCellIdx.XIdx;
			cellIdx.YIdx = cellIdxOffset.YIdx + TargetCellIdx.YIdx;

			auto CellState = GetMapInfo(cellIdx.XIdx, cellIdx.YIdx);
			if (ENUM_TO_BITFIELD((int32)CellState.CellInfo) & InTargetCellState)
				OutCells.Emplace(cellIdx);
		}
	}
}

int32 UGridMapManager::GetCellDepthFromDistance(float InDistance)
{
	if (IsValid(MapStatus) == false)
		return 0;

	int32 DepthResult = FPlatformMath::RoundToInt( InDistance / MapStatus->GetCellSize() );
	if (DepthResult <= 0)
		DepthResult = 1;

	return DepthResult;
}

const float DEFAULT_CHECK_SIZE = 0.707106f;	// SQRT(2) / 2
const float CHECK_SIZE_RATE = 0.006508f;	// (1 - SQRT(2) / 2) / 45

float UGridMapManager::GetFarthestMovableDistance(const FVector& StartPos, const FVector& Direction, float Distance)
{
	if (IsValid(MapStatus) == false)
	{
		return 0.0f;
	}

	FVector end_pos = StartPos + (Direction * Distance);

#ifdef DRAW_DEBUG
	DrawDebugLine(RGAMEINSTANCE(this)->GetWorld(), StartPos, end_pos, FColor(255, 255, 0, 255), false, 30.0f, 20, 2.0f);
#endif

	float cos_value = Direction.X;
	if (0.0f > cos_value)
	{
		cos_value = cos_value * -1.0f;
	}

	float degree = FMath::RadiansToDegrees(FMath::Acos(cos_value));
	//UE_LOG(LogTemp, Warning, TEXT("GetFarthestMovableDistance : org_degree(%.02f)"), degree);

	if (0.0f > degree)
	{
		degree = degree * -1.0f;
	}
	if (45.0f > degree)
	{
		degree = 90.0f - degree;
	}

	float check_size_rate = (degree - 45.0f) * CHECK_SIZE_RATE + DEFAULT_CHECK_SIZE;
	float check_size = MapStatus->GetCellSize() * check_size_rate;
	int32 check_count = int32(Distance / check_size) + 1;
	//UE_LOG(LogTemp, Warning, TEXT("GetFarthestMovableDistance : degree(%.02f), size_rate(%.04f), size(%.04f), count(%d)"),
	//	degree, check_size_rate, check_size, check_count);

	float far_dist = 0.0f;
	float check_dist = 0.0f;
	FMapCellData cell_info;
	cell_info.CellInfo = EMapCellStateEnum::VE_None;
	for (int n = 0; n < check_count; ++n)
	{
		if (n < (check_count - 1))
		{
			check_dist = check_size * n;
		}
		else
		{
			check_dist = Distance;
		}

		cell_info = GetMapInfoFromWorldPos(StartPos + (Direction * check_dist));
		if (EMapCellStateEnum::VE_None == cell_info.CellInfo || EMapCellStateEnum::VE_Invalid == cell_info.CellInfo)
		{
			break;
		}
		else
		{
			far_dist = check_dist;
		}
		//end_pos = StartPos + (Direction * check_dist);
		//DrawDebugLine(GameInstance->GetWorld(), StartPos, end_pos, FColor(0, 255, 0, 255), false, 5.0f, 0, n * 2.0f);
		//UE_LOG(LogTemp, Warning, TEXT("GetFarthestMovableDistance : cheked_count(%d)"), n);
	}

	return far_dist;
}

/*
Find Valid Location : Line Trace
*/

bool UGridMapManager::GetValidLocationAroundPivotLocation(const FVector& StartPos, const FVector& EndPos, FVector& ValidPos, bool bSweep)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
#ifdef DRAW_DEBUG
	DrawDebugLine(GameInstance->GetWorld(), StartPos, EndPos, FColor(255, 0, 0, 1), false, 10.0f, 0.0f, 2.0f);
#endif
	FCellIndex StartCellIndex, EndCellIndex;
	GetCellIndexFromWorldPos(StartPos, StartCellIndex);
	GetCellIndexFromWorldPos(EndPos, EndCellIndex);

	auto DistXCell = static_cast<int32>(FMath::Abs(StartCellIndex.XIdx - EndCellIndex.XIdx));
	auto DistYCell = static_cast<int32>(FMath::Abs(StartCellIndex.YIdx - EndCellIndex.YIdx));
	float IntervalPos = 0.0f;
	if (DistXCell < DistYCell)
	{
		IntervalPos = FMath::Abs(StartPos.X - EndPos.X) / static_cast<float>(DistYCell);
	}
	else
	{
		IntervalPos = FMath::Abs(StartPos.Y - EndPos.Y) / static_cast<float>(DistXCell);
	}

	bool		bIsFindValidGrid = false;

	if (FMath::IsNearlyZero(StartPos.X - EndPos.X))
	{
		if (bSweep)
		{
			if (StartCellIndex.YIdx > EndCellIndex.YIdx)
			{
				for (float YWorldPos = StartPos.Y; YWorldPos >= EndPos.Y; YWorldPos -= IntervalPos)
				{
					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);

					bIsFindValidGrid = true;
					auto bIsGridValid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (!bIsGridValid)
						break;

					bIsFindValidGrid = true;
					ValidPos = TargetWorldPos;
				}
			}
			else
			{
				for (float YWorldPos = StartPos.Y; YWorldPos <= EndPos.Y; YWorldPos += IntervalPos)
				{
					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);
					auto bIsGridValid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsGridValid == true)
					{
						bIsFindValidGrid = true;
						ValidPos = TargetWorldPos;
					}

					if (!bIsGridValid)
						break;
				}
			}
		}
		else
		{
			if (EndCellIndex.YIdx < StartCellIndex.YIdx)
			{
				for (float YWorldPos = EndPos.Y; YWorldPos <= StartPos.Y; YWorldPos += IntervalPos)
				{
					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);
					bIsFindValidGrid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsFindValidGrid)
					{
						ValidPos = TargetWorldPos;
						break;
					}
				}
			}
			else
			{
				for (float YWorldPos = EndPos.Y; YWorldPos >= StartPos.Y; YWorldPos -= IntervalPos)
				{
					FVector TargetWorldPos = FVector(StartPos.X, YWorldPos, 0.0f);
					bIsFindValidGrid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsFindValidGrid)
					{
						ValidPos = TargetWorldPos;
						break;
					}
				}
			}
		}
	}
	else if (FMath::IsNearlyZero(StartPos.Y - EndPos.Y))
	{
		if (bSweep)
		{
			if (StartCellIndex.XIdx > EndCellIndex.XIdx)
			{
				for (float XWorldPos = StartPos.X; XWorldPos >= EndPos.X; XWorldPos -= IntervalPos)
				{
					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
					auto bIsGridValid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsGridValid == true)
					{
						bIsFindValidGrid = true;
						ValidPos = TargetWorldPos;
					}

					if (!bIsGridValid)
						break;
				}
			}
			else
			{
				for (float XWorldPos = StartPos.X; XWorldPos <= EndPos.X; XWorldPos += IntervalPos)
				{
					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
					auto bIsGridValid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsGridValid == true)
					{
						bIsFindValidGrid = true;
						ValidPos = TargetWorldPos;
					}

					if (!bIsGridValid)
						break;
				}
			}
		}
		else
		{
			if (EndCellIndex.XIdx < StartCellIndex.XIdx)
			{
				for (float XWorldPos = EndPos.X; XWorldPos <= StartPos.X; XWorldPos += IntervalPos)
				{
					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
					bIsFindValidGrid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsFindValidGrid)
					{
						ValidPos = TargetWorldPos;
						break;
					}
				}
			}
			else
			{
				for (float XWorldPos = EndPos.X; XWorldPos >= StartPos.X; XWorldPos -= IntervalPos)
				{
					FVector TargetWorldPos = FVector(XWorldPos, StartPos.Y, 0.0f);
					bIsFindValidGrid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if(bIsFindValidGrid)
					{
						ValidPos = TargetWorldPos;
						break;
					}
				}
			}
		}
	}
	else //y =ax + b
	{
		float	M = static_cast<float>(EndPos.Y - StartPos.Y) / static_cast<float>(EndPos.X - StartPos.X);
		auto	Y = [&M, &StartPos](float XPos)->float { return M * (XPos - StartPos.X) + StartPos.Y; };
		if (bSweep)
		{
			if (StartCellIndex.XIdx > EndCellIndex.XIdx)
			{
				for (float XWorldPos = StartPos.X; XWorldPos >= EndPos.X; XWorldPos -= IntervalPos)
				{
					float YWorldPos = Y(XWorldPos);
					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
					auto bIsGridValid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsGridValid == true)
					{
						bIsFindValidGrid = true;
						ValidPos = TargetWorldPos;
					}

					if (!bIsGridValid)
						break;
				}
			}
			else
			{
				for (float XWorldPos = StartPos.X; XWorldPos <= EndPos.X; XWorldPos += IntervalPos)
				{
					float YWorldPos = Y(XWorldPos);
					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
					auto bIsGridValid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsGridValid == true)
					{
						bIsFindValidGrid = true;
						ValidPos = TargetWorldPos;
					}

					if (!bIsGridValid)
						break;
				}
		}
	}
		else
		{
			if (EndCellIndex.XIdx < StartCellIndex.XIdx)
			{
				for (float XWorldPos = EndPos.X; XWorldPos <= StartPos.X; XWorldPos += IntervalPos)
				{
					float YWorldPos = Y(XWorldPos);
					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
					bIsFindValidGrid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsFindValidGrid)
					{
						ValidPos = TargetWorldPos;
						break;
					}
				}
			}
			else
			{
				for (float XWorldPos = EndPos.X; XWorldPos >= StartPos.X; XWorldPos -= IntervalPos)
				{
					float YWorldPos = Y(XWorldPos);
					FVector TargetWorldPos = FVector(XWorldPos, YWorldPos, 0.0f);
					bIsFindValidGrid = IsTargetGridValidByWorldPos(TargetWorldPos);
					if (bIsFindValidGrid)
					{
						ValidPos = TargetWorldPos;
						break;
					}
				}
			}
		}
}

	if (bIsFindValidGrid)
	{
		float Height = 0.0f;
		if (GetValidHeight(ValidPos, Height) == false)
			return false;

		ValidPos.Z = Height;
#ifdef DRAW_DEBUG
		DrawDebugPoint(GameInstance->GetWorld(), ValidPos, 10, FColor(0, 255, 0, 1), false, 10.0f);
#endif
	}

	return bIsFindValidGrid;
}

bool UGridMapManager::GetValidHeight(const FVector& Pos, float& Height)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	auto World = GameInstance->GetWorld();
	if (World == nullptr)
		return false;


	TArray<FHitResult> OutHitArray;

	FVector StartLocation = Pos, EndLocation = Pos;
	StartLocation.Z = Pos.Z + 1000000.0f;
	EndLocation.Z = Pos.Z - 1000000.0f;

	FCollisionQueryParams Param(TEXT("Find_Floor_BlockingVolume"), false, nullptr);
	//World->LineTraceMultiByChannel(OutHitArray, StartLocation, EndLocation, ECC_WorldStatic, CollisionParams); 
	World->LineTraceMultiByObjectType(OutHitArray, StartLocation, EndLocation, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects), Param);

#ifdef DRAW_DEBUG
	DrawDebugLine(World, StartLocation, EndLocation, FColor(255, 0, 0, 1), false, 30.0f, 0, 3);
#endif

	if (OutHitArray.Num() <= 0)
		return false;

	bool bFindHitLocation = false;

	for (auto& HitElem : OutHitArray)
	{
		if (HitElem.bBlockingHit == false)
			continue;

		if (!HitElem.Actor.IsValid())
			continue;

		auto HitBlockingVolume = Cast<ABlockingVolume>(HitElem.Actor.Get());
		if (HitBlockingVolume == nullptr)
			continue;

		Height = HitElem.Location.Z;
		bFindHitLocation = true;
		break;
	}

	return bFindHitLocation;
}

bool UGridMapManager::IsTargetGridValidByWorldPos(const FVector& WorldPos)
{
	auto MapState = GetMapInfoFromWorldPos(WorldPos);
	if (MapState.CellInfo == EMapCellStateEnum::VE_Invalid || MapState.CellInfo == EMapCellStateEnum::VE_None)
		return false;

	return true;
}

/*
Find Valid Location Nearest TargetCell
*/
bool UGridMapManager::FindNearestPivotCellWithTargetCell(FCellIndex& InPivotCell, FCellIndex& InTargetCell)
{
	TQueue<FBfs_Cell_Info>	BFS_Queue;
	TSet<int32>				CheckCells;
	FBfs_Cell_Info			CellElem;
	FBfs_Cell_Info			FindCellInfo;
	bool					bFindDestination = false;
	int32					MinDistBetweenTargetAndFindCell = MAX_int32;

	CellElem.CellIndex = InPivotCell;
	CellElem.Width = 0;

	BFS_Queue.Enqueue(CellElem);
	CheckCells.Emplace(InPivotCell.GetCustomHashValue());
	while (BFS_Queue.IsEmpty() == false)
	{
		if (BFS_Queue.Dequeue(CellElem) == false)
			break;

		auto CellElemInfo = GetMapInfo(CellElem.CellIndex.XIdx, CellElem.CellIndex.YIdx);
		if (CellElem.CellIndex.IsValidCellIndex() &&
			(CellElemInfo.CellInfo != EMapCellStateEnum::VE_Invalid) &&
			(CellElemInfo.CellInfo != EMapCellStateEnum::VE_None)
			)
		{
			if (!bFindDestination || (MinDistBetweenTargetAndFindCell > InTargetCell.GetCellDistCost(CellElem.CellIndex)))
			{
				bFindDestination = true;
				FindCellInfo = CellElem;
				MinDistBetweenTargetAndFindCell = InTargetCell.GetCellDistCost(CellElem.CellIndex);
			}
		}

		for (auto& DepthOffsetELem : DepthOneOffsets)
		{
			FCellIndex AdjacentGrid = GetAdjecentCellAroundTargetCell(CellElem.CellIndex, DepthOffsetELem);
			//auto AdjacentGridInfo = GetMapInfo(AdjacentGrid.XIdx, AdjacentGrid.YIdx);

			if (AdjacentGrid.IsValidCellIndex() == false)
				continue;

			if (CheckCells.Contains(AdjacentGrid.GetCustomHashValue()))
				continue;

			FBfs_Cell_Info				AdjacentCellInfo;    
			AdjacentCellInfo.CellIndex = AdjacentGrid;
			AdjacentCellInfo.Width = CellElem.Width + 1;

			if (!bFindDestination || (MinDistBetweenTargetAndFindCell > InTargetCell.GetCellDistCost(AdjacentGrid)))
			{
				BFS_Queue.Enqueue(AdjacentCellInfo);
				CheckCells.Emplace(AdjacentGrid.GetCustomHashValue());
			}
		}
	}

	if (bFindDestination == false)
	{
		InPivotCell = FCellIndex(-1, -1);
	}
	else
	{
		InPivotCell = FindCellInfo.CellIndex;
	}

	return bFindDestination;
}

FCellIndex UGridMapManager::GetAdjecentCellAroundTargetCell(FCellIndex& InTargetCellIndex, FCellIndex& InCellIndexOffset)
{
	return FCellIndex(InTargetCellIndex.XIdx + InCellIndexOffset.XIdx, InTargetCellIndex.YIdx + InCellIndexOffset.YIdx);
}

float UGridMapManager::GetCellSizeBymapStatus()
{
	if (MapStatus)
	{
		return MapStatus->GetCellSize();
	}

	return 150.0f;
}

bool UGridMapManager::IsValidMapStatus()
{
	return IsValid(MapStatus);
}

/**
	Grid map status class
*/

void UGridMapStatus::InitFromMapCreatorActor(AMapCreatorActor* FromActor)
{
	if (IsValid(FromActor) == false)
	{		
		bValid = false;
		return;
	}
		
	MapCreatorActor = FromActor;

#ifndef CHECK_NEXT_MOVE_VALIDATION
	return;
#endif

	MapStatusInfoList.Empty();
	MapStatusInfoList.SetNum(MapCreatorActor->MapSizeY);
	for (FGridMapStateInfo& HorMap : MapStatusInfoList)
	{
		HorMap.CellStateInfoList.SetNum(MapCreatorActor->MapSizeX);
	}
}

float UGridMapStatus::GetCellSize()
{
	if (MapCreatorActor.IsValid())
		return MapCreatorActor->CellSize;

	return 1;
}

int32 UGridMapStatus::GetMapSizeX()
{
	if (MapCreatorActor.IsValid())
		return MapCreatorActor->MapSizeX;

	return 1;
}

int32 UGridMapStatus::GetMapSizeY()
{
	if (MapCreatorActor.IsValid())
		return MapCreatorActor->MapSizeY;

	return 1;
}

FVector2D UGridMapStatus::GetMapSize()
{
	if (MapCreatorActor.IsValid())
		return FVector2D( MapCreatorActor->MapSizeX, MapCreatorActor->MapSizeY );

	return FVector2D(1,1);
}

FVector UGridMapStatus::GetStartPos()
{
	if (MapCreatorActor.IsValid())
		return MapCreatorActor->StartPos;
	return FVector::ZeroVector;
}

FVector UGridMapStatus::GetCellWorldPos(int32 XIdx, int32 YIdx)
{
	if (MapCreatorActor.IsValid())
		return MapCreatorActor->GetCellWorldPos( XIdx, YIdx );
	return FVector::ZeroVector;
}

void UGridMapStatus::SetMapInfo(int32 XIdx, int32 YIdx, FMapCellData InMapCellData)
{
	if (MapCreatorActor.IsValid())
		MapCreatorActor->SetMapInfo(XIdx, YIdx, InMapCellData);
}

FMapCellData UGridMapStatus::GetMapInfo(int32 XIdx, int32 YIdx)
{
	if (MapCreatorActor.IsValid())
		return MapCreatorActor->GetMapInfo(XIdx, YIdx);
	return FMapCellData();
}

bool UGridMapStatus::IsValidMapCell(int32 XIdx, int32 YIdx)
{
	if (MapCreatorActor.IsValid() == false)
		return false;

	if (MapCreatorActor->MapInfo.IsValidIndex(YIdx) == false)
		return false;

	if (MapCreatorActor->MapInfo[YIdx].CellInfo.IsValidIndex(XIdx) == false)
		return false;

	return true;
}

bool UGridMapStatus::IsValidMapStatusCell(int32 XIdx, int32 YIdx)
{
	if (MapStatusInfoList.IsValidIndex(YIdx) == false)
		return false;

	if (MapStatusInfoList[YIdx].CellStateInfoList.IsValidIndex(XIdx) == false)
		return false;

	return true;
}

bool UGridMapStatus::ReserveMovingTarget(FCellIndex& InTargetIndics, AActor* TargetActor)
{
	if (IsValidMapStatusCell(InTargetIndics.XIdx, InTargetIndics.YIdx) == false)
		return false;

	if (MapStatusInfoList[InTargetIndics.YIdx].CellStateInfoList[InTargetIndics.XIdx].OpccuipingActor.IsValid())
		return false;
	else
		MapStatusInfoList[InTargetIndics.YIdx].CellStateInfoList[InTargetIndics.XIdx].OpccuipingActor = nullptr;		// check invalid target actor.

	MapStatusInfoList[InTargetIndics.YIdx].CellStateInfoList[InTargetIndics.XIdx].OpccuipingActor = TargetActor;

	return true;
}

void UGridMapStatus::ClearReserveMovingTarget(FCellIndex& InTargetIndics)
{
	if (IsValidMapStatusCell(InTargetIndics.XIdx, InTargetIndics.YIdx) == false)
		return;

	MapStatusInfoList[InTargetIndics.YIdx].CellStateInfoList[InTargetIndics.XIdx].OpccuipingActor = nullptr;
}

