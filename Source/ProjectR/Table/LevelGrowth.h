#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "LevelGrowth.generated.h"


/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FLevelGrowthTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		Level;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		AvatarMaxExp;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		StaminaMax;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		StaminaBonus;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		arenaDefaultExp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		colosseumDefaultExp;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		StaminaPVPMax;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = LevelGrowthTableInfo)
	int32		HeroMaxRune;
};