#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "BattlePowerTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FBattlePowerTable: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBattlePowerTable)
	float AccuracyValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBattlePowerTable)
	float PierceValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBattlePowerTable)
	float AttackSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBattlePowerTable)
	float CriticalValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBattlePowerTable)
	float DodgeValue = 0.0f;
};
