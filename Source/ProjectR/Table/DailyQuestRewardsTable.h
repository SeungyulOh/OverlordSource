
#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "DailyQuestRewardsTable.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FDailyQuestRewardsTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDailyQuestRewardsTable)
	int32		MinScore;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDailyQuestRewardsTable)
	FName		WalletId;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDailyQuestRewardsTable)
	int32		Amt;
};

