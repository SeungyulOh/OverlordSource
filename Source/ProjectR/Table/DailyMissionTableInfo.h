#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "DailyMissionTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FDailyMissionTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	FName MissionTitle_StringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	FName MissionDocument_StringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	int32 Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	FName ContentsType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	FName ContentsLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	FName Condition1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	FName Condition2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	int32 ConditionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	FName CurrencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyMissionTableInfo)
	int32 CurrencyValue;
};

