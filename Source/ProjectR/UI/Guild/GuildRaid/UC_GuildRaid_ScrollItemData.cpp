// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_ScrollItemData.h"

void UUC_GuildRaid_ScrollItemData::SetRaidData(FGUILD_RAID& InData, bool bLock)
{
	RaidData = InData;
	IsLock = bLock;
}
