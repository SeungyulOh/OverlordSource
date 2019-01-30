// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "MapCreatorActor.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"



// Sets default values
AMapCreatorActor::AMapCreatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
#endif

	CellSize = 150;
	MapSizeX = 25;
	MapSizeY = 25;
	CheckPointGab = 30.0f;
	CellCheckSuccessCount = 3;
	CheckOnlyCenter = false;
	CheckExtent = FVector(10, 10, 5000);

	bValidMap = false;
}

// Called when the game starts or when spawned
void AMapCreatorActor::BeginPlay()
{
	Super::BeginPlay();


}

bool AMapCreatorActor::ShouldTickIfViewportsOnly() const
{
	return true;
}

// Called every frame
void AMapCreatorActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bShowDebugInfo)
		DrawDebugInfo();
	else
		SetActorTickEnabled(false);
}

void AMapCreatorActor::InitMapData(int32 Width, int32 Height)
{
	MapInfo.Empty();
	MapInfo.Reserve(Height);
	for (int32 i = 0; i < Height; ++i )
	{
		FMapCellDataInfo MapCellInfo;
		MapCellInfo.CellInfo.SetNum(Width);
		MapInfo.Emplace(MapCellInfo);
	}

	MapWidth = FVector2D(CellSize * Width, CellSize * Height);
	MapHalfSize = FVector2D(MapWidth.X * 0.5f, MapWidth.Y * 0.5f);
	StartPos = GetActorLocation() - FVector(MapHalfSize.X, MapHalfSize.Y, 0.0f);

	bValidMap = true;
}

void AMapCreatorActor::SetMapInfo(int32 XIdx, int32 YIdx, FMapCellData InMapCellData)
{
	if ( MapInfo.IsValidIndex(YIdx) && MapInfo[YIdx].CellInfo.IsValidIndex(XIdx))
		MapInfo[YIdx].CellInfo[XIdx] = InMapCellData;
}

FMapCellData AMapCreatorActor::GetMapInfo(int32 XIdx, int32 YIdx)
{
	if (MapInfo.IsValidIndex(YIdx) && MapInfo[YIdx].CellInfo.IsValidIndex(XIdx))
		return MapInfo[YIdx].CellInfo[XIdx];

	return FMapCellData();
}

bool AMapCreatorActor::SaveMapInfo( FString SaveFileName )
{
	FGridMapDataInfo	GridMapDataInfo;

	MakeGridMapInfoData(GridMapDataInfo);

	FString OutJsonString;
	FJsonHelper::UStructToJsonObjectString( FGridMapDataInfo::StaticStruct(), &GridMapDataInfo, OutJsonString, 0, 0 );
	FString FullPath = FPaths::ProjectSavedDir() + SaveFileName;
	if (FFileHelper::SaveStringToFile(OutJsonString, *FullPath))
		return true;
	return false;
}

bool AMapCreatorActor::SaveMapInfoWithBinary(FString SaveFileName)
{
	FGridMapDataInfo	GridMapDataInfo;

	MakeGridMapInfoData(GridMapDataInfo);

	FBufferArchive BuffArchive;
	BuffArchive << GridMapDataInfo;

	FString FullPath = FPaths::ProjectSavedDir() + SaveFileName + FString(".binary");
	if (FFileHelper::SaveArrayToFile(BuffArchive, *FullPath))
	{
		UE_LOG(LogUI, Log, TEXT("SaveMapInfoWithBinary Success"));
		return true;
	}
	UE_LOG(LogUI, Log, TEXT("SaveMapInfoWithBinary Failed"));
	return false;
}

bool AMapCreatorActor::LoadMapInfoWithBinary(FString SaveFileName)
{
	FString FullPath = FPaths::ProjectSavedDir() + SaveFileName + FString(".binary");
	TArray< uint8 > FileContents;
	if (!FFileHelper::LoadFileToArray(FileContents, *FullPath))
	{
		return false;
	}

	FBufferArchive ReceiveAr;
	ReceiveAr.Serialize((void*)FileContents.GetData(), FileContents.Num());

	FGridMapDataInfo	GridMapDataInfo;
	FMemoryReader Loader(ReceiveAr);
	Loader << GridMapDataInfo;

	SetGridMapInfoData(GridMapDataInfo);

	return true;
}

void AMapCreatorActor::MakeGridMapInfoData( FGridMapDataInfo& GridMapDataInfo )
{	
	GridMapDataInfo.MapSizeX = MapSizeX;
	GridMapDataInfo.MapSizeY = MapSizeY;
	GridMapDataInfo.StartPos = StartPos;
	GridMapDataInfo.CellSize = CellSize;

	GridMapDataInfo.MapInfo = MapInfo;
}

void AMapCreatorActor::SetGridMapInfoData(FGridMapDataInfo& GridMapDataInfo)
{
	MapSizeX = GridMapDataInfo.MapSizeX;
	MapSizeY = GridMapDataInfo.MapSizeY;
	StartPos = GridMapDataInfo.StartPos;
	CellSize = GridMapDataInfo.CellSize;

	MapInfo = GridMapDataInfo.MapInfo;
}

FVector	AMapCreatorActor::GetCellPos(int32 XIdx, int32 YIdx)
{
	float xPos = XIdx * CellSize;
	float yPos = YIdx * CellSize;

	return FVector(xPos, yPos, 0);
}

FVector AMapCreatorActor::GetCellWorldPos(int32 XIdx, int32 YIdx)
{
	return GetCellPos( XIdx, YIdx ) + StartPos;
}

void AMapCreatorActor::BuildMapInfo()
{
	FlushPersistentDebugLines(this->GetWorld());

	InitMapData(MapSizeX, MapSizeY);
	
	//FVector CellSizeVec = FVector(CellSize * 0.5f, CellSize * 0.5f, 0);
	for (int32 yIdx = 0; yIdx < MapSizeY; ++yIdx )
	{
		for (int32 xIdx = 0; xIdx < MapSizeX; ++xIdx )
		{
			FVector CellPivot = GetCellPos( xIdx, yIdx);
			
			if (CheckOnNavyMesh(CellPivot + StartPos))
			{
				FMapCellData MapCellData;
				MapCellData.CellInfo = EMapCellStateEnum::VE_Movable;

				FVector CenterPos = CellPivot + StartPos + FVector(CellSize / 2, CellSize / 2, 0);
				//FVector ProjectionToFloorPos;
				float Height;
				if (UMapFunctionLibrary::GetHeightFloor(this, CenterPos, Height))
				{
					MapCellData.Height = Height;

					CenterPos.Z = Height;
					MapCellData.CellZone = RayCheckHitCellZoneVolume(CenterPos, MapCellData);
				}
				else
				{
					MapCellData.CellInfo = EMapCellStateEnum::VE_Invalid;
				}
				
				SetMapInfo(xIdx, yIdx, MapCellData);
				UE_LOG(LogUI, Log, TEXT("map X,Y : %d, %d        Height = %f"), xIdx, yIdx, MapCellData.Height);
			}
		}
	}
}

bool AMapCreatorActor::CheckOnNavyMesh(FVector CellPivot)
{
	int32 SuccessCount = 0;

	TArray< FVector > SamplePoints = GetSamplePoints(CellPivot);

	for (FVector& point : SamplePoints)
	{
		if (UMapFunctionLibrary::IsEnableNavigationPos(this,point, CheckExtent))
			++SuccessCount;
	}

	if (SuccessCount > CellCheckSuccessCount)
	{
		return true;
	}

	return false;
}

TArray< FVector> AMapCreatorActor::GetSamplePoints(FVector CellPivot)
{
	TArray< FVector > Result;	
	FVector CenterPos = CellPivot + FVector(CellSize * 0.5f, CellSize * 0.5f, 0);

	if (!CheckOnlyCenter)
	{
		Result.Reserve(5);
		Result.Emplace(CenterPos + FVector(-CheckPointGab, -CheckPointGab, 0));
		Result.Emplace(CenterPos + FVector(CheckPointGab, -CheckPointGab, 0));
		Result.Emplace(CenterPos + FVector(-CheckPointGab, CheckPointGab, 0));
		Result.Emplace(CenterPos + FVector(CheckPointGab, CheckPointGab, 0));
	}
	Result.Emplace(CenterPos);

	//DrawDebugBox(GetWorld(), CenterPos, FVector(CheckPointGab, CheckPointGab, 0), FColor::Red );

	return Result;
}


void AMapCreatorActor::DrawDebugInfo()
{
	FVector MapCenterPos = FVector(MapSizeX / 2 * CellSize + StartPos.X, MapSizeY / 2 * CellSize + StartPos.Y, StartPos.Z);
	DrawDebugBox( GetWorld(), MapCenterPos, FVector(MapSizeX / 2 * CellSize, MapSizeY / 2 * CellSize, 1), FColor::Red , false , 2.f , 0 , 5.f);
	

	for (int32 y = 0; y < MapSizeY; ++y)
	{
		for (int32 x = 0; x < MapSizeX; ++x)
		{
			auto CellState = GetMapInfo(x, y);
			FVector CenterPos = FVector(x*CellSize + StartPos.X + CellSize / 2, y*CellSize + StartPos.Y + CellSize / 2, StartPos.Z);
			FVector Extent = FVector(CellSize / 2 - 3, CellSize / 2 - 3, 1);
			if (CellState.CellInfo == EMapCellStateEnum::VE_Movable)
			{
				if (EMapCellZoneType::VE_None == CellState.CellZone)
					DrawDebugBox( GetWorld(), CenterPos, Extent, FColor::Green, false, 2.f, 0, 5.f);
				else if(EMapCellZoneType::VE_PeaceZone == CellState.CellZone)
					DrawDebugBox(GetWorld(), CenterPos, Extent, FColor::Cyan, false, 2.f, 0, 5.f);
				else if (EMapCellZoneType::VE_ExchangeZone == CellState.CellZone)
					DrawDebugBox(GetWorld(), CenterPos, Extent, FColor::Magenta, false, 2.f, 0, 5.f);
			}
			else
			{
				DrawDebugBox(GetWorld(), CenterPos, Extent, FColor::Red, false, 2.f, 0, 5.f);
			}
		}
	}
}

void AMapCreatorActor::OptimizeSize()
{
	// Top line
	int32 TopStartIdx = 0;
	for (int32 y = 0; y < MapSizeY; ++y)
	{		
		bool bEmptyLine = true;
		for (int32 x = 0; x < MapSizeX; ++x)
		{
			if (GetMapInfo(x, y).CellInfo != EMapCellStateEnum::VE_None)
			{
				bEmptyLine = false;
				break;
			}
		}
		if (bEmptyLine)
			TopStartIdx++;
		else
			break;
	}

	// bottom line
	int32 BottomEndIdx = MapSizeY-1;
	for (int32 y = MapSizeY-1; y >= 0; --y)
	{
		bool bEmptyLine = true;
		for (int32 x = 0; x < MapSizeX; ++x)
		{
			if (GetMapInfo(x, y).CellInfo != EMapCellStateEnum::VE_None)
			{
				bEmptyLine = false;
				break;
			}
		}
		if (bEmptyLine)
			BottomEndIdx--;
		else
			break;
	}

	// left line
	int32 LeftStartIdx = 0;
	for (int32 x = 0; x < MapSizeX; ++x)
	{
		bool bEmptyLine = true;
		for (int32 y = 0; y < MapSizeY; ++y)
		{
			if (GetMapInfo(x, y).CellInfo != EMapCellStateEnum::VE_None)
			{
				bEmptyLine = false;
				break;
			}
		}
		if (bEmptyLine)
			LeftStartIdx++;
		else
			break;
	}

	// right line
	int32 RightEndIdx = MapSizeX-1;
	for (int32 x = MapSizeX - 1; x >= 0; --x)
	{
		bool bEmptyLine = true;
		for (int32 y = 0; y < MapSizeY; ++y)
		{
			if (GetMapInfo(x, y).CellInfo != EMapCellStateEnum::VE_None)
			{
				bEmptyLine = false;
				break;
			}
		}
		if (bEmptyLine)
			RightEndIdx--;
		else
			break;
	}
}

EMapCellZoneType AMapCreatorActor::RayCheckHitCellZoneVolume(FVector CheckPoint, FMapCellData& MapCellData)
{
	UWorld* InWorld = this->GetWorld();
	if (InWorld == nullptr)
		return EMapCellZoneType::VE_None;

	TArray<FHitResult> OutHitArray;

	FVector CheckStart = CheckPoint;
	FVector CheckEnd = CheckPoint;
	CheckStart.Z = CheckPoint.Z + 1000000.0f;
	CheckEnd.Z = CheckPoint.Z - 1000000.0f;
	ECollisionChannel ColChannel = ECollisionChannel::ECC_WorldStatic;

	FCollisionQueryParams CollisionParams;
	//CollisionParams.TraceTag = FName("PeaceZone");
	CollisionParams.bTraceComplex = false;

	if (InWorld->LineTraceMultiByChannel(OutHitArray, CheckStart, CheckEnd, ColChannel, CollisionParams))
	{
		for (FHitResult& OutHit : OutHitArray)
		{
			AActor* BlockActor = OutHit.GetActor();
			//UE_LOG(LogGamePlay, Log, TEXT("AMapCreatorActor::RayCheckHitCellZoneVolume() BlockActor(%s)"), *(AActor::GetDebugName(BlockActor)));

			if (BlockActor)
			{
				if (BlockActor->ActorHasTag(FName("PeaceZone")))
				{
					return EMapCellZoneType::VE_PeaceZone;
				}
				else if (BlockActor->ActorHasTag(FName("ExchangeZone")))
				{
					if (BlockActor->Tags.IsValidIndex(1))
					{
						MapCellData.ZoneParam = FCString::Atoi(*(BlockActor->Tags[1].ToString()));
					}

					return EMapCellZoneType::VE_ExchangeZone;
				}
			}
		}
	}
	return EMapCellZoneType::VE_None;
}