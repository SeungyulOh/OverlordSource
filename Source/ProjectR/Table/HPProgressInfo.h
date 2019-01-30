#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "HPProgressInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FHPProgressInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HPProgressInfo)
	float					ReferenceValue;

	
};
