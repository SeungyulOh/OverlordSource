#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "GuildManagerListScrollItemData.generated.h"

/**
*
*/

UCLASS()
class PROJECTR_API UGuildManagerListScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGUILD_MEMBER	GuildMemberData;
	UPROPERTY()
	EGUILD_GRADE	MyGuildGrade;
	UPROPERTY()
	int32			MyAccountKid;
	UPROPERTY()
		bool		IsBtnOn;
};