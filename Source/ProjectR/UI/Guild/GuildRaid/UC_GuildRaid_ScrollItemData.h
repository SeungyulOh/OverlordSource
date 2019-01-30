// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Network/PacketDataStructures.h"
#include "UI/Control/RScrollItemData.h"
#include "UC_GuildRaid_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaid_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
	
public:
	
	void SetRaidData(FGUILD_RAID& InData, bool bLock);

	UPROPERTY()
	FGUILD_RAID			RaidData;

	bool				IsLock = true;

	
	
};
