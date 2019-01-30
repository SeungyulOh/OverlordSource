#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "GameState/Stage/Spawner/LevelActor.h"

#include "MapTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FMapTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable )
	int32							Group;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MapTable|None")
	//int32							ProgressNo;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							StarCondition;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							StageGoalString;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	EGamePlayModeTypeEnum			GameMode;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							P_Map;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							Sub_Level_Art;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							Sub_Level_Design;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							Sub_Level_ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							LevelSpawnInfo;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							PlayTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	int32							BGSound;

	// Initial camera info
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	int32							StartPos;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							StartRot;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							CamLongDist;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							CamLongAngle;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							CamLongFOV;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							CamShortDist;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							CamShortAngle;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	float							CamShortFOV;


	//by cowspirit
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	TArray<FName>					LoadingTableKeys;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	FName							GameModeSequenceKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MapTable)
	TSoftClassPtr<class ALevelActor>		LevelActor;
};
