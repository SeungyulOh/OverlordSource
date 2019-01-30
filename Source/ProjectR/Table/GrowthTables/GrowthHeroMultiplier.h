#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GrowthHeroMultiplier.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FGrowthHeroMultiplier : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float MaxHP;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//float MaxMP;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float PhysicalAttackPower;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float MagicAttackPower;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float Armor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float MagicResistance;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//float ParryValue;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//float DodgeValue;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//float MPRegen;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//float CriticalValue;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//float PierceValue;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//float AccuracyValue;
};
