#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GuideResourceTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FGuideResourceTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGuideResourceTableInfo)
	FName						KOR;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGuideResourceTableInfo)
	FName						ENG;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGuideResourceTableInfo)
	FName						CHN;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGuideResourceTableInfo)
	FName						TWN;
};
