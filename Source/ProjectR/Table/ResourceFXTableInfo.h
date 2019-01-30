#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ResourceFXTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceFXTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceFXTableInfo)
	TAssetPtr< UParticleSystem >		Particle = nullptr;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceFXTableInfo)
	TAssetPtr< USoundCue >				Sound = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceFXTableInfo)
	bool								UseParentScale = true;
};
