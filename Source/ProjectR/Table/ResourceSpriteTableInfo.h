#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "PaperSprite.h"
#include "ResourceSpriteTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceSpriteTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceSpriteTableInfo)
	TAssetPtr< UPaperSprite >		Asset = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceSpriteTableInfo)
	int32							Cache = 0;
};
