// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomStructures/GridMapDataInfo.h"
#include "MapCreatorActor.generated.h"


UCLASS()
class PROJECTR_API AMapCreatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapCreatorActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	// trick for rendering in PIE
	virtual bool ShouldTickIfViewportsOnly() const override;

	void InitMapData( int32 Width, int32 Height );
	void SetMapInfo(int32 XIdx, int32 YIdx, FMapCellData InMapCellData);
	FMapCellData GetMapInfo(int32 XIdx, int32 YIdx);

	UFUNCTION(BlueprintCallable, Category = MapCreator)
	bool SaveMapInfo(FString SaveFileName);

	UFUNCTION(BlueprintCallable, Category = MapCreator)
	bool SaveMapInfoWithBinary(FString SaveFileName);
	bool LoadMapInfoWithBinary(FString SaveFileName);

	void				MakeGridMapInfoData(FGridMapDataInfo& GridMapDataInfo);
	void				SetGridMapInfoData(FGridMapDataInfo& GridMapDataInfo);
	FVector				GetCellPos(int32 XIdx, int32 YIdx);
	FVector				GetCellWorldPos(int32 XIdx, int32 YIdx);

	UFUNCTION(BlueprintCallable, Category = MapCreator)
	void				BuildMapInfo();
	bool				CheckOnNavyMesh(FVector CellPivot);
	TArray< FVector>	GetSamplePoints(FVector CellPivot);
	
	void				DrawDebugInfo();
	void				OptimizeSize();
	EMapCellZoneType	RayCheckHitCellZoneVolume(FVector CheckPoint, FMapCellData& MapCellData);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	int32			CellSize = 150;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	int32			MapSizeX = 25;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	int32			MapSizeY = 25;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = MapInfo)
	FVector2D		MapHalfSize;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = MapInfo)
	FVector			StartPos;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	float			CheckPointGab = 30.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	int32			CellCheckSuccessCount = 3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	bool			CheckOnlyCenter = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	FVector			CheckExtent = FVector(10, 10, 5000);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	TArray< FMapCellDataInfo >			MapInfo;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapInfo)
	bool			bShowDebugInfo = false;

private:
	UPROPERTY()
	FVector2D		MapWidth;
	bool			bValidMap;
};
