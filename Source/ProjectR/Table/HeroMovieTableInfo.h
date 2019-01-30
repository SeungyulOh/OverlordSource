#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "HeroMovieTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FHeroMovieTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroMovieTableInfo)
// 	FName MovieType;
// 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroMovieTableInfo)
	FName IndividualType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroMovieTableInfo)
	FName MovieThumbnailRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroMovieTableInfo)
	FName MoviesRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroMovieTableInfo)
	FName NameString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroMovieTableInfo)
	int32 DisplayType;
};

