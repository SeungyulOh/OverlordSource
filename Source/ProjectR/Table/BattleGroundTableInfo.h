#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "BattleGroundTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FBattleGroundTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGroundTableInfo)
	FName						MapNameUIStringKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGroundTableInfo)
	int32						EventGroupId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGroundTableInfo)
	FName						MapTableKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGroundTableInfo)
	int32						ExchangeTime = 0;
};
