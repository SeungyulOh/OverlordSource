#pragma once
#include "SubtitleTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSubtitleTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleTableInfo)
	FName MediaID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleTableInfo)
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleTableInfo)
	float EndTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleTableInfo)
	FName StringKey;

	
};