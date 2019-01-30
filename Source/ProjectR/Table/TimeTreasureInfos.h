#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "TimeTreasureInfos.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FTimeTreasureTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TimeTreasure)
		int32					FloorInfoId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TimeTreasure)
		FName					RewardType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TimeTreasure)
		FName					RewardIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TimeTreasure)
		int32					RewardAmount_Rate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TimeTreasure)
		int32					RewardAmount_Max;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TimeTreasure)
		FName					RewardImageIndex;

	static TArray<FTimeTreasureTable*> LookUp(int32 InFloorIdx);
};