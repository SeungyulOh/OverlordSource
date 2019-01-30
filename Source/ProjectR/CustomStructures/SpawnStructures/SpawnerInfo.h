// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "CustomStructures/SpawnStructures/SpawnerTransformInfo.h"
#include "SpawnerInfo.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnWaypointInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnWaypointInfo)
	int32			WaypointIdx;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnWaypointInfo)
	float			WaypointDelayTime = 0.0f;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnSpeechInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnSpeechInfo)
	FName			SpeechKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnSpeechInfo)
	float			SpeechDelayTime = 0.0f;
};

UENUM(BlueprintType)
enum class EStageIndicatorType : uint8
{
	VE_FixedText		UMETA(DisplayName = "FixedText"),
	VE_Attack			UMETA(DisplayName = "Attack"),
	VE_HP 				UMETA(DisplayName = "HP"),
	VE_INDICATORTYPEMAX	UMETA(DisplayName = "INDICATORTYPEMAX"),
};

USTRUCT(BlueprintType)
struct PROJECTR_API FStageIndicatorInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	FName							IndicatorStrKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	float							IndicatorDurationTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	float							IndicatorCoolTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	float							Param1;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	FName			CharacterID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	float			IgnoreMinDistance = 150.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	TArray<FSpawnWaypointInfo>	WaypointList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	TArray<FSpawnSpeechInfo>	SpeechList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	float						SpeechShowTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	int32						PlayMatineeID = -1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	int32						PlaySequenceID = -1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpawnCharacterInfo)
	float						SightRange = 0.0f;

public:
	TMap< EStageIndicatorType, TArray<FStageIndicatorInfo> >		IndicatorMap;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnerInfo
{
	GENERATED_USTRUCT_BODY()	
	 
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpawnerInfo)
	int32					SpawnerID = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpawnerInfo)
	int32					SpawnPositionID = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpawnerInfo)
	TArray< FSpawnCharacterInfo >			CharacterIDs;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpawnerInfo)
	float					SpawnDelay = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	ESpawnerTypeEnum		SpawnerType = ESpawnerTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	ESpawnerFormationTypeEnum	FormationType = ESpawnerFormationTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	float					SpawnPosGab = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	bool					BossSpawnTrigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	bool					BossCameraEvent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	TArray<int32>			BossArrayIndexList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	ETeamNO					TeamNO = ETeamNO::VE_NEUTRALITY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	float					SightRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	FName					BossName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerInfo)
	TArray< FSpawnerTransformInfo >					CustomFormationTransforms;

};
