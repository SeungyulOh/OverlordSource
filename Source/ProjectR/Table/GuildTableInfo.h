// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "SharedConstants/GlobalConstants.h"
#include "GuildTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FGuildLevelUpTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 point;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	FName attendanceDailyRewardKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 member_limit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 guildbuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 guildraid1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 guildraid2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 guildraid3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 guildraid4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 guildraid5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 guildmatch;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGuildContributeTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 consumeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 point;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildLevelUpTableInfo)
	int32 avatarExp;
};