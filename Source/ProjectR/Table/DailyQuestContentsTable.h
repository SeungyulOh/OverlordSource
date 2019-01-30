#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "DailyQuestContentsTable.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FDailyQuestContents: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDailyQuestContents)
	EGamePlayModeTypeEnum		ContentsType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDailyQuestContents)
	int32						Score;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDailyQuestContents)
// 	FName						TriggerName;
};

