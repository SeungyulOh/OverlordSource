#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "GuildRuneDonationScrollItemData.generated.h"

/**
*
*/

UCLASS()
class PROJECTR_API UGuildRuneDonationScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGUILD_RUNE_REQUESTER		RuneRequester;
};