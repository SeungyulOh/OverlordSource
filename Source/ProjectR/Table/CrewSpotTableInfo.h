#pragma once


#include "CrewSpotTableInfo.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FCrewSpotTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CrewSpotTableInfo)
	FVector2D Crew1Spot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CrewSpotTableInfo)
	FVector2D Crew2Spot;
};