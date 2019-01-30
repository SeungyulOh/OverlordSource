#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GrowthHeroInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FGrowthHeroInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	TArray< float > MaxHP;
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//TArray< float > MaxMP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	TArray< float > PhysicalAttackPower;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	TArray< float > MagicAttackPower;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	TArray< float > Armor;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	TArray< float > MagicResistance;
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//TArray< float > ParryValue;
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//TArray< float > DodgeValue;
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//TArray< float > MPRegen;
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//TArray< float > CriticalValue;
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//TArray< float > PierceValue;
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	//TArray< float > AccuracyValue;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGrowthHeroInfo_V2 : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float MaxHP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float PhysicalAttackPower;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float MagicAttackPower;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float Armor;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroGrowthNumberType)
	float MagicResistance;
};