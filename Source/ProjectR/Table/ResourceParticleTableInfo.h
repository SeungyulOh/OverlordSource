#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ResourceParticleTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceParticleTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceParticleTableInfo)
	TAssetPtr< UParticleSystem >		Asset = nullptr;
};
