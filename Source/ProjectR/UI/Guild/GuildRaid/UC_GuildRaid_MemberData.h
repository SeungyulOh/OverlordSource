#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataEnum.h"
#include "UC_GuildRaid_MemberData.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API UUC_GuildRaid_MemberData : public URScrollItemData
{
	GENERATED_BODY()

public:
	void SetMemberData(FName InMemberName, int32 InChallengeCount, int32 InCreateCount, int32 InSinceTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	FName					MemberName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	int32					ChallengeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	int32					CreateCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaListScrollData)
	int32					SinceTime;
};