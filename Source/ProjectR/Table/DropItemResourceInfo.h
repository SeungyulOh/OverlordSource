#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Engine/DataTable.h"
#include "DropItemResourceInfo.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FDropItemResourceInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropItemResourceInfo)
	TSoftObjectPtr< USkeletalMesh > SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropItemResourceInfo)
	TSoftObjectPtr< UAnimationAsset > Animation_Drop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropItemResourceInfo)
	TSoftObjectPtr< UAnimationAsset > Animation_Idle;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDropItemParticleInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropItemParticleInfo)
	TSoftObjectPtr< UParticleSystem > Particle;
};

