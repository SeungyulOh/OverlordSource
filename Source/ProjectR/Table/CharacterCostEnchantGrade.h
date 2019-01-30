#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "CharacterCostEnchantGrade.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FCharacterCostEnchantGrade : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FCharacterCostEnchantGrade)
		FName MaxEnchantPoint;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FCharacterCostEnchantGrade)
		FName StatMaxPoint;
};