#pragma once

#include "Engine/DataTable.h"
#include "FloorQuestTableInfo.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FFloorQuestTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestTableInfo)
	int32		ConditionGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestTableInfo)
	FName		FloorInfoId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestTableInfo)
	FName		FloorStageId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestTableInfo)
	FName		RewardKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestTableInfo)
	FName		MissionTitleKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestTableInfo)
	FName		MissionDocumentKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestTableInfo)
	FName		TargetStringKey;
};