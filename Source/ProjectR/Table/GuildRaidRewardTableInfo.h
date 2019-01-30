#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GuildRaidRewardTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FGuildRaidRewardTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGuildRaidRewardTableInfo)
	TArray<int32>					requiredGuildRaidPoint;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGuildRaidRewardTableInfo)
	TArray<FName>					guildRaidReward;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGuildRaidRewardTableInfo)
	TArray<FName>					SeasonStepRewardIcon;


};
