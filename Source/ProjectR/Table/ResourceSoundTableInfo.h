#pragma once
#include "ResourceSoundTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceSoundTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StringTableInfo)
	TSoftObjectPtr< USoundCue >		Sound;
};