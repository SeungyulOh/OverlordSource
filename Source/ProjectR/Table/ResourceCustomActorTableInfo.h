#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ResourceCustomActorTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceCustomActorTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FResourceSequenceTableInfo)
	TAssetSubclassOf< AActor >		AssetClass = nullptr;

	UClass*		GetBlueprintClass();
};
