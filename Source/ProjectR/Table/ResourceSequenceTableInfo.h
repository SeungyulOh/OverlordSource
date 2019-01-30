#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "ResourceSequenceTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceSequenceTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FResourceSequenceTableInfo)
	TAssetSubclassOf< ALevelSequenceActor >		SequenceClass = nullptr;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FResourceSequenceTableInfo)
	int32			StartSubtitle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FResourceSequenceTableInfo)
	int32			EndSubtitle;

	UClass*		GetBlueprintClass();
};
