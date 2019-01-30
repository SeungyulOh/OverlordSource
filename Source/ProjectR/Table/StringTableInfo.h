#pragma once
#include "StringTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FStringTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StringTableInfo)
	FText KOR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StringTableInfo)
	FText ENG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StringTableInfo)
	FText CHN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StringTableInfo)
	FText TWN;
};