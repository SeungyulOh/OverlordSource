// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_MemberData.h"

void UUC_GuildRaid_MemberData::SetMemberData(FName InMemberName, int32 InChallengeCount, int32 InCreateCount, int32 InSinceTime)
{
	MemberName = InMemberName;

	ChallengeCount = InChallengeCount;

	CreateCount = InCreateCount;

	SinceTime = InSinceTime;
}
