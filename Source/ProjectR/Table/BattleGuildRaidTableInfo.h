#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "BattleGuildRaidTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FBattleGuildRaidTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					modGroup;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					Grade;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					PlayLevel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					GradeCost;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					OpenCost;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					MapNameUIStringKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					MapDescUIStringKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					LockSprite;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					UnLockSprite;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					BFNpcLocationGroupId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					BFEventGroupId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					OpenEventType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					MapName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					MapTableKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					MemberMax;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					MemberMin;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					PartyMax;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	int32					PlayTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	TArray<FName>			MonsterIDList;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					RewardKey1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					RewardKey2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					RewardKey3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	FName					ClearRewardKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FBattleGuildRaidTableInfo)
	float					DiceDelay;
};
