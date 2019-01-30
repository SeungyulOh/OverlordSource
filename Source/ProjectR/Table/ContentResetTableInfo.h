#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ContentResetTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FContentResetTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FContentResetTableInfo)
	FName currencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FContentResetTableInfo)
	int32 currencyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FContentResetTableInfo)
	int32 resetExtraCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FContentResetTableInfo)
	int32 addCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FContentResetTableInfo)
	int32 resetLimitCountPerDay;
};
