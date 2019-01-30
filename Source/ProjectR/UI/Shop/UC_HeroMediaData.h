#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataEnum.h"
#include "UC_HeroMediaData.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API UC_HeroMediaData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	FName					IndividualType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	FName					MovieThumbnailRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	FName					MoviesRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	FName					NameString;
};