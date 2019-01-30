// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildInfo.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"

void UGuildInfo::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;

	ClearGuildData();
	ClearJoinUIData();
}

void UGuildInfo::ClearGuildData()
{
	MyGuildExist = false;
	MyGuildInfo.guild_id = 0;
	MyGuildInfo.name = "";
	MyMemberData.kId = 0;
	MyMemberData.guild_id = 0;
	GuildMasterData.kId = 0;
	GuildMasterData.guild_id = 0;

	MyGuildMemberList.Empty();
	AwaiterList.Empty();
}

bool UGuildInfo::CheckIsMyGuild(int32 guildID)
{
	if (MyGuildInfo.guild_id == guildID)
		return true;
	else
		return false;
}

void UGuildInfo::SetGuildMine(FGUILD_MINE& guildMine)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	MyGuildExist = guildMine.guild_exist;

	if (MyGuildExist)
	{
		SetGuildData(guildMine.guild_info);
		SetGuildDataExtend(guildMine.isActiveAttendance, guildMine.isActiveContribute/*, guildMine.openRaid*/);

		SetMyMemberData(guildMine.guild_member);
		SetGuildMasterData(guildMine.guild_master);
	}
	else
	{
		ClearGuildData();
	}
	RGameInstance->EventManager->OnRefreshGuildInfoUI.Broadcast();
	//RGameInstance->EventManager->OnRefreshGuildRaidInfo.Broadcast();
}

bool UGuildInfo::SetGuildMemberList(TArray<FGUILD_MEMBER>&	guildMemberList)
{
	if (0 >= guildMemberList.Num())
		return false;

	MyGuildMemberList = guildMemberList;

	return true;
}

void UGuildInfo::UpdateGuildMemberData(FGUILD_MEMBER& memberInfo)
{
	for (FGUILD_MEMBER& GuildMember : MyGuildMemberList)
	{
		if (GuildMember.kId == memberInfo.kId)
		{
			GuildMember = memberInfo;
			break;
		}
	}

	if (memberInfo.kId == MyMemberData.kId)
	{
		SetMyMemberData(memberInfo);
	}
	if (memberInfo.kId == GuildMasterData.kId)
	{
		SetGuildMasterData(memberInfo);
	}
}

void UGuildInfo::SetGuestGuildMemberList(TArray<FGUILD_MEMBER>& InMemberList)
{
	Guest_GuildMemberList.Reset();
	Guest_GuildMemberList = InMemberList;
}

bool UGuildInfo::GetGuildMasterInfo(FGUILD_MEMBER& GuildMasterInfo)
{
	for (FGUILD_MEMBER& GuildMember : MyGuildMemberList)
	{
		if (GuildMember.grade == EGUILD_GRADE::MASTER)
		{
			GuildMasterInfo = GuildMember;
			break;
		}
	}

	if (GuildMasterInfo.grade == EGUILD_GRADE::MASTER)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
// Guild Join UI Data

void UGuildInfo::ClearJoinUIData()
{
	FindGuildList.Empty();
	WaitingGuildIDs.Empty();
	GuildRankingList.Empty();
}

FGUILD_INFO* UGuildInfo::GetFindGuild(int32 InGuildID)
{
	FGUILD_INFO* FindGuildInfo = nullptr;

	if (0 < FindGuildList.Num())
	{
		FindGuildInfo = FindGuildList.FindByPredicate([&](const FGUILD_INFO& GuildInfo) {
			return GuildInfo.guild_id == InGuildID;
		});
	}
	
	return FindGuildInfo;
}

bool UGuildInfo::IsFullWaitingGuildCount()
{
	if (3 <= WaitingGuildIDs.Num())
		return true;

	return false;
}

bool UGuildInfo::IsWaitingGuild(int32 InGuildID)
{
	for (auto GuildID : WaitingGuildIDs)
	{
		if (GuildID == InGuildID)
			return true;
	}

	return false;
}

void UGuildInfo::AddWaitingGuildID(int32 InGuildID)
{
	if (WaitingGuildIDs.Contains(InGuildID))
		return;

	WaitingGuildIDs.Emplace(InGuildID);
}

void UGuildInfo::RemoveWaitingGuildID(int32 InGuildID)
{
	if (WaitingGuildIDs.Contains(InGuildID))
	{
		WaitingGuildIDs.Remove(InGuildID);
	}
}

bool UGuildInfo::SetGuildRankingList(TArray<FGUILD_INFO>& InGuildList)
{
	if (0 >= InGuildList.Num())
		return false;

	GuildRankingList = InGuildList;

	return true;
}


//////////////////////////////////////////////////////////////////////////


bool UGuildInfo::SetGuildAwaiterList(TArray<FGUILD_AWAITER>& guildAwaiterList)
{
	if (0 >= guildAwaiterList.Num())
		return false;

	AwaiterList = guildAwaiterList;

	return true;
}

void UGuildInfo::RemoveAwaiter(int32 InKid)
{
	int32 FindElementIndex = AwaiterList.IndexOfByPredicate([&](const FGUILD_AWAITER& AwaiterInfo) {
		return (AwaiterInfo.kId == InKid);
	});

	if (FindElementIndex != INDEX_NONE)
	{
		if (AwaiterList.IsValidIndex(FindElementIndex))
			AwaiterList.RemoveAt(FindElementIndex);
	}
}

void UGuildInfo::SetGuildHistory(TArray<FGUILD_HISTORY>& guildHistoryList)
{
	HistoryList = guildHistoryList;
}

void UGuildInfo::SetGuildRuneRequesterList(TArray<FGUILD_RUNE_REQUESTER>& guildRuneRequests)
{
	RuneRequests = guildRuneRequests;
}

void UGuildInfo::UpdateGuildRuneRequester(FGUILD_RUNE_REQUESTER& guildRuneRequest)
{
	int32 FindIndex = RuneRequests.IndexOfByPredicate([&](const FGUILD_RUNE_REQUESTER& RuneRequest) {
		return (RuneRequest.kId == guildRuneRequest.kId);
	});

	if (INDEX_NONE == FindIndex)
	{
		RuneRequests.Emplace(guildRuneRequest);
	}
	else
	{
		if (RuneRequests.IsValidIndex(FindIndex))
		{
			RuneRequests[FindIndex] = guildRuneRequest;
		}
	}
}

bool UGuildInfo::GetMyRuneRequest(FGUILD_RUNE_REQUESTER& outMyRuneRequest)
{
	int32 FindIndex = RuneRequests.IndexOfByPredicate([&](const FGUILD_RUNE_REQUESTER& RuneRequest) {
		return (RuneRequest.kId == MyMemberData.kId);
	});

	if (INDEX_NONE == FindIndex)
		return false;
	if (!RuneRequests.IsValidIndex(FindIndex))
		return false;

	outMyRuneRequest = RuneRequests[FindIndex];
	return true;
}

void UGuildInfo::SetGuildRaidStatusInfos(TArray<FGUILD_RAID> RaidInfos, int32 remainTime)
{
	RaidStatusInfos.Empty();
	RaidStatusInfos = RaidInfos;

	RemainTime = remainTime;
}

void UGuildInfo::SetGuildRaidStatusInfo(TArray<FGUILD_RAID_CHALLENGE>& Inhistorys, FGUILD_RAID& InRaidInfo)
{
	RaidHistorys = Inhistorys;
	int32 modId = FCString::Atoi(*InRaidInfo.modId);
	int32 BossIndex = static_cast<int32>((modId - 1) / 6);
	if (RaidStatusInfos.IsValidIndex(BossIndex))
	{
		RaidStatusInfos[BossIndex].captain = InRaidInfo.captain;
		RaidStatusInfos[BossIndex].members = InRaidInfo.members;
	}
}

void UGuildInfo::SetGuildDataExtend(bool bActiveAttendance, bool bActiveContribute/*, FGUILD_RAID& openRaid*/)
{ 
	IsActiveAttendance = bActiveAttendance; 
	IsActiveContribute = bActiveContribute; 
// 	if (openRaid.modId.IsEmpty())
// 		OpenRaid = FGUILD_RAID();
// 	else
// 		OpenRaid = openRaid; 

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->NotifyManager))
	{
		RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_GuildAttendance, IsActiveAttendance);
		RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_GuildDonate, IsActiveContribute);
	}
}