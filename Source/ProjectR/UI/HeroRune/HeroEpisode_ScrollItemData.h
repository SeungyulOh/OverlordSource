#pragma once

#include "UI/Control/RScrollItemData.h"
#include "HeroEpisode_ScrollItemData.generated.h"

/**
*
*/

UCLASS()
class PROJECTR_API UHeroEpisode_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString		HeroUD;

	UPROPERTY()
	int32		nCurrentCount = 0;
	
	UPROPERTY()
	int32		nMaxCount = 2;

	UPROPERTY()
	bool		bEpisodeExist = false;
};