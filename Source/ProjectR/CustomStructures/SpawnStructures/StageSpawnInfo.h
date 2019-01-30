// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "CustomStructures/SpawnStructures/SpawnerInfo.h"
#include "CustomStructures/SpawnStructures/SpawnerTransformInfo.h"
#include "StageSpawnInfo.generated.h"


UENUM(BlueprintType)
enum class EStageSpawnerEventEnum : uint8
{
	VE_Begin			UMETA(DisplayName = "Begin"),
	VE_Clear 			UMETA(DisplayName = "Clear"),	
};

UENUM(BlueprintType)
enum class EStageSpawnerCommandEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Spawn					UMETA(DisplayName = "Spawn"),
	VE_HeroFirstSpawn			UMETA(DisplayName = "HeroFirstSpawn"),
	VE_TriggerSpawn				UMETA(DisplayName = "TriggerSpawn"),
	VE_EndBossSpawnSquence		UMETA(DisplayName = "EndBossSpawnSquence"),
	VE_Wall						UMETA(DisplayName = "Wall"),
	VE_Dialog					UMETA(DisplayName = "Dialog"),
	VE_Matinee					UMETA(DisplayName = "Matinee"),
	VE_Sequence					UMETA(DisplayName = "Sequence"),
	VE_NextWaypoint				UMETA(DisplayName = "NextWaypoint"),
	VE_Movie					UMETA(DisplayName = "Movie"),
};


USTRUCT(BlueprintType)
struct PROJECTR_API FStageSpawnerEvent
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	EStageSpawnerCommandEnum		CommandType = EStageSpawnerCommandEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	int32							EventInvoker = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	int32							EventIntParam1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	int32							EventIntParam2 = 0;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FStageGroupEvent
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageGroupEvent)
	TArray<int32>							GroupList;	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageGroupEvent)
	TArray<FStageSpawnerEvent>				EventList;

	UPROPERTY()
	TMap<int32, int32>						ClearGroupList;

	void AddClearSpawn(int32 idx);
	bool IsClearEvent();
};


UENUM(BlueprintType)
enum class EDialogEventTypeEnum : uint8
{
	VE_EndStartSequence			UMETA(DisplayName = "EndStartSequence"),
	VE_EndBossSequence			UMETA(DisplayName = "EndBossSequence"),
	VE_EndClearSequence			UMETA(DisplayName = "EndClearSequence"),
	VE_TriggerEvent				UMETA(DisplayName = "TriggerEvent"),
};

USTRUCT(BlueprintType)
struct PROJECTR_API FStageDialogueEvent
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageDialogEvent)
	EDialogEventTypeEnum		eventType = EDialogEventTypeEnum::VE_EndStartSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageDialogEvent)
	FName						DialogID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageDialogEvent)
	FName						DialogGroupID;

};

USTRUCT(BlueprintType)
struct PROJECTR_API FStageDialogue
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageDialogue)
	FName						ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageDialogue)
	FName						DialogGroupID;

};

USTRUCT(BlueprintType)
struct PROJECTR_API FStageDialogues
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageDialogues)
	FName						TableID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageDialogues)
	TArray<FStageDialogue>		DialogList;

};


USTRUCT(BlueprintType)
struct PROJECTR_API FTriggerSpawnEvent
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FTriggerSpawnEvent)
	int32							ID;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTriggerWallEvent
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FTriggerWallEvent)
	int32							ID;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FStageTriggerEvent
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageTriggerEvent)
	int32							ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageTriggerEvent)
	TArray<FStageSpawnerEvent>		EventList;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FStageUsingWall
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageTriggerEvent)
	int32							ID;	
};


USTRUCT(BlueprintType)
struct PROJECTR_API FStageNextWayPoint
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageNextWayPoint)
	int32							ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageNextWayPoint)
	int32							PotionID = 0;
};




USTRUCT(BlueprintType)
struct PROJECTR_API FStageMovie
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
		 int32						ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
		FName						MovieFileName;

};

USTRUCT(BlueprintType)
struct PROJECTR_API FBossAlert
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	FName						AlertKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	float						AlertDurationTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	TArray<int32>				AlertSkills;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	bool						bAlertOn = false;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FIndicatorInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	FName						AlertKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	float						Param1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	float						AlertDurationTime = 0.0f;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	float						AlertCoolTime = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FStageDialog)
	bool						bPlayOnce = false;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FEventMatinee
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = EventMatinee)
	int32							MapID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = EventMatinee)
	FName							Path;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = EventMatinee)
	FString							MatineeName;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FEventSequence
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEventSequence)
	int32							ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEventSequence)
	FName							SequenceKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEventSequence)
	bool							IsEnableSkip;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FStageSpawnInfo
{
	GENERATED_USTRUCT_BODY()	
	 
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	FName							UniqueName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< int32 >					HeroSpawnPos;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FSpawnerInfo >			Spawners;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	float							BossSpawnConditionCount = 0;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	int32							BossCount= 0;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	FName							BossName;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageDialogueEvent >	DialogEvents;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageSpawnerEvent >	PreEvent;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageSpawnerEvent >	BeginEvent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageSpawnerEvent >	ClearEvent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageGroupEvent >		GroupEvent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageSpawnerEvent >	CustomEvent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FEventMatinee >			EventMatineeList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FEventSequence >		EventSequenceList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageTriggerEvent >	TriggerEventList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageUsingWall >		UsingWallList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageNextWayPoint >	NextWayPointList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	FName							DialogueTableID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)						
	TArray< FStageMovie >			MovieInfos;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	FBossAlert						BossAlertInfo;

	UPROPERTY()
	TMap< int32, FSpawnerTransformInfo >			SpawnTransforms;

	UPROPERTY()
	TMap< int32, FSpawnerWallInfo >					SpawnWalls;

	UPROPERTY()
	TMap< int32, FSpawnerTriggerInfo >				SpawnTriggers;

	/*
	UPROPERTY()
	TArray< FSpawnerTransformInfo >					CustomFormationTransforms;
	*/
	
	UPROPERTY()
	FName											CheckEventID;

public:
	void						Init();
	FSpawnerInfo*				GetSpanwerInfo(int32 spawnerID);
	FORCEINLINE FSpawnerInfo*	GetBossSpanwerInfo()	{ return &BossSpawnerInfo; }
	FORCEINLINE bool			HasBossSpawnerInfo()	{ return HasBossSpawner; }
	FORCEINLINE int32			GetSpawnerInfoNum()		{ return SpawnerInfoTable.Num(); }

private:
	UPROPERTY()
	TMap< int32, FSpawnerInfo >		SpawnerInfoTable;
	UPROPERTY()
	FSpawnerInfo					BossSpawnerInfo;
	UPROPERTY()
	bool							HasBossSpawner = false;

	UPROPERTY()
	bool							HasProtectedObject = false;	

public:
	bool							IsUsingBossAlert = false;
	TArray<FIndicatorInfo>			HPIndicatorInfo;
	FIndicatorInfo					IndicatorInfo;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FStageSpawnInfoList
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = StageSpawner)
	TArray< FStageSpawnInfo >			InfoList;
};


