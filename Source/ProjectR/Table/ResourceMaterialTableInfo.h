#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ResourceMaterialTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceMaterialTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceMaterialTableInfo)
	TAssetPtr< UMaterialInstance >		Asset = nullptr;
};
