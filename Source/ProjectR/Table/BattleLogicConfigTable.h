#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "BattleLogicConfigTable.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FBattleLogicConfigTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBattleLogicConfigTable)
	float BattleConstant;
};

