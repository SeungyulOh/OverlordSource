#pragma once
#include "MediaTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FMediaTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaTableInfo)
	FString MediaUrl;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MediaTableInfo)
	int32			StartSubtitle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = MediaTableInfo)
	int32			EndSubtitle;
};