// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnerTransformInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnerTransformInfo
{
	GENERATED_USTRUCT_BODY()

public:

	void				MakeTransform( int32 InID, FString strPosition, FString strRotation);

	FTransform			GetTransform();

	int32				ID;
	UPROPERTY()
	FVector				Position;
	UPROPERTY()
	FRotator			Rotation;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnerWallInfo
{
	GENERATED_USTRUCT_BODY()

public:

	void				MakeTransform(int32 InID, int32 InColorID, FString strPath, FString strPosition, FString strRotation, FString strScale);

	int32				ID;

	UPROPERTY()
	int32				ColorID;
	UPROPERTY()
	FString				Path;
	UPROPERTY()
	FVector				Position;
	UPROPERTY()
	FRotator			Rotation;
	UPROPERTY()
	FVector				Scale;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnerTriggerInfo
{
	GENERATED_USTRUCT_BODY()

public:

	void				MakeTransform(int32 InID, FString strPath, FString strPosition, FString strRotation, FString strBoxScale);

	int32				ID;
	UPROPERTY()
	FString				Path;
	UPROPERTY()
	FVector				Position;
	UPROPERTY()
	FRotator			Rotation;
	UPROPERTY()
	FVector				BoxScale;
};