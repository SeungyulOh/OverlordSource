#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ResourceCurveTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceCurveTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceCurveTableInfo)
	TAssetPtr< UCurveBase >		Asset = nullptr;
};
