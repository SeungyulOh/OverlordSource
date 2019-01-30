#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "UI/UIDefs.h"
#include "GuildRuneRequestScrollItemData.generated.h"

/**
*
*/

UCLASS()
class PROJECTR_API UGuildRuneRequestScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FHERO_ICON_INFO				HeroIconInfo;

	UPROPERTY()
	int32						HaveCount;
};