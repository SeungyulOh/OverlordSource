#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ResourceStaticMeshTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceStaticMeshTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceStaticMeshTableInfo)
		TAssetPtr< UStaticMesh >		Asset = nullptr;
};
