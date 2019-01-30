// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GridMapDataInfo.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMapCellStateEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Movable			UMETA(DisplayName = "Movable"),
	VE_Occupied			UMETA(DisplayName = "Occupied"),
	VE_MoveReserved		UMETA(DisplayName = "MoveReserved"),
	VE_Reserved2		UMETA(DisplayName = "Reserved2"),
	VE_Reserved3		UMETA(DisplayName = "Reserved3"),
	VE_Invalid			UMETA(DisplayName = "Invalid"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMapCellZoneType : uint8
{
	VE_None,
	VE_PeaceZone,
	VE_ExchangeZone,
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMapCellData
{
	GENERATED_USTRUCT_BODY()

public:
	FMapCellData()
	{}

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapCellDataInfo)
	EMapCellStateEnum		CellInfo = EMapCellStateEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapCellDataInfo)
	float					Height = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapCellDataInfo)
	EMapCellZoneType		CellZone = EMapCellZoneType::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapCellDataInfo)
	int32					ZoneParam = 0;

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FMapCellData& Data)
	{
		EMapCellStateEnum CellState = Data.CellInfo;
		Ar << CellState;
		Ar << Data.Height;
		EMapCellZoneType ZoneType = Data.CellZone;
		Ar << ZoneType;
		Ar << Data.ZoneParam;
		return Ar;
	}
};


enum EMapCellMoveDirectionFlag
{
	VE_UPPER_LEFT = 1 << 0,
	VE_UP = 1 << 1,
	VE_UPPER_RIGHT = 1 << 2,
	VE_LEFT = 1 << 3,
	VE_RIGHT = 1 << 4,
	VE_LOWER_LEFT = 1 << 5,
	VE_DOWN = 1 << 6,
	VE_LOWER_RIGHT = 1 << 7,
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMapCellDataInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapCellDataInfo)
	TArray< FMapCellData >			CellInfo;

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FMapCellDataInfo& Data)
	{
		Ar << Data.CellInfo;
		return Ar;
	}
};

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FGridMapDataInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GridMapDataInfo)
	int32			CellSize = 100;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GridMapDataInfo)
	int32			MapSizeX = 50;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GridMapDataInfo)
	int32			MapSizeY = 50;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GridMapDataInfo)
	FVector			StartPos;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GridMapDataInfo)
	TArray< FMapCellDataInfo >			MapInfo;

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGridMapDataInfo& Data)
	{
		Ar << Data.CellSize;
		Ar << Data.MapSizeX;
		Ar << Data.MapSizeY;
		Ar << Data.StartPos;
		Ar << Data.MapInfo;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGridCellStateInfo
{
	GENERATED_USTRUCT_BODY()

public:
	TWeakObjectPtr< AActor >		OpccuipingActor;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGridMapStateInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GridMapStateInfo)
	TArray< FGridCellStateInfo >			CellStateInfoList;
};
