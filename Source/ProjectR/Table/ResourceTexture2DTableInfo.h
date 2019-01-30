#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ResourceTexture2DTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceTexture2DTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceTexture2DTableInfo)
	TAssetPtr< UTexture2D >		Asset = nullptr;
};
