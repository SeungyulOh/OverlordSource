#pragma once

//#include "SharedConstants/GlobalConstants.h"
#include "Engine/DataTable.h"
#include "Network/PacketDataEnum.h"
#include "FloorMissionTable.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FFloorMissionTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
	FName MissionTitle_StringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		FName MissionDocument_StringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		int32 FloorInfoId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		FName PlayType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		int32 ClearGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		FName ClassCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		EFLOOR_MISSION_CLASS_CONDITION_TYPE ConditionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		FName CurrencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FFloorMissionTable)
		int32 CurrencyValue;
};

