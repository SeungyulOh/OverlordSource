// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "GuildInfo.generated.h"

//class URGameInstance;



USTRUCT(BlueprintType)
struct PROJECTR_API FGuildRaidSeasonStatus
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	FName		LastReward;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	int32		LastLevel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	bool		IsAlreadyReward;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	FName		Reward;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	int32		MaxPoint;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	int32		Point;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	int32		Level;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FGuildRaidSeasonStatus")
	int32		RemainTime;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildInfo : public UObject
{
	GENERATED_BODY()

public:
	void	Initialize(/*URGameInstance* InRGameInstance*/);

	//////////////////////////////////////////////////////////////////////////
	// Guild Management Data

	void	ClearGuildData();
	bool CheckIsMyGuild(int32 guildID);
	void	SetGuildMine(FGUILD_MINE& guildMine);

	FORCEINLINE void	SetGuildData(FGUILD_INFO& guildInfo) { MyGuildInfo = guildInfo; }
	void	SetGuildDataExtend(bool bActiveAttendance, bool bActiveContribute/*, FGUILD_RAID& openRaid*/);
	FORCEINLINE void	SetMyMemberData(FGUILD_MEMBER& memberInfo)		{ MyMemberData = memberInfo; }
	FORCEINLINE void	SetGuildMasterData(FGUILD_MEMBER& memberInfo)	{ GuildMasterData = memberInfo; }
	bool	SetGuildMemberList(TArray<FGUILD_MEMBER>& guildMemberList);
	void	UpdateGuildMemberData(FGUILD_MEMBER& memberInfo);

	FORCEINLINE void	SetGuestGuildData(FGUILD_INFO& guildInfo) { Guest_GuildInfo = guildInfo; }
	void    SetGuestGuildMemberList(TArray<FGUILD_MEMBER>& InMemberList);
	
	FORCEINLINE bool	IsGuildExist() { return MyGuildExist; }
	bool	GetGuildMasterInfo(FGUILD_MEMBER& GuildMasterInfo);

	UPROPERTY()
	bool					MyGuildExist;
	UPROPERTY()
	FGUILD_INFO				MyGuildInfo;
	UPROPERTY()
	FGUILD_MEMBER			MyMemberData;
	UPROPERTY()
	FGUILD_MEMBER			GuildMasterData;
	UPROPERTY()
	TArray<FGUILD_MEMBER>	MyGuildMemberList;

	bool					IsActiveAttendance;
	bool					IsActiveContribute;
// 	FGUILD_RAID				OpenRaid;

	FGUILD_INFO				Guest_GuildInfo;
	TArray<FGUILD_MEMBER>   Guest_GuildMemberList;

public:
	//////////////////////////////////////////////////////////////////////////
	// Guild Join UI Data
	void ClearJoinUIData();
	
	void SetFindGuildList(TArray<FGUILD_INFO>& InGuildList) { FindGuildList = InGuildList; }
	FGUILD_INFO* GetFindGuild(int32 InGuildID);

	void SetWaitingGuildIDs(TArray<int32>& InGuildIDs) { WaitingGuildIDs = InGuildIDs; }
	bool IsFullWaitingGuildCount();
	bool IsWaitingGuild(int32 InGuildID);
	void AddWaitingGuildID(int32 InGuildID);
	void RemoveWaitingGuildID(int32 InGuildID);

	bool SetGuildRankingList(TArray<FGUILD_INFO>& InGuildList);

	UPROPERTY()
	TArray<FGUILD_INFO>		FindGuildList;
	UPROPERTY()
	TArray<int32>			WaitingGuildIDs;
	UPROPERTY()
	TArray<FGUILD_INFO>		GuildRankingList;


public:
	//////////////////////////////////////////////////////////////////////////
	// Guild Management UI Data

	// Attendance
	EREWARDABLE_STATUS		Attendance_dailyRewardable;
	EREWARDABLE_STATUS		Attendance_weeklyRewardable;
	int32					Attendance_weeklyCount;
	int32					Attendance_weeklyTimeLeft;

	// Donation
	EREWARDABLE_STATUS			Donation_contributable;
	EGUILD_CONTRIBUTION_TYPE	Donation_contributionType;

	// Awaiter
	bool	SetGuildAwaiterList(TArray<FGUILD_AWAITER>& guildAwaiterList);
	void	RemoveAwaiter(int32 InKid);
	UPROPERTY()
	TArray<FGUILD_AWAITER>			AwaiterList;

	void	SetGuildHistory(TArray<FGUILD_HISTORY>& guildHistoryList);
	UPROPERTY()
	TArray<FGUILD_HISTORY>		HistoryList;

	// Rune
	void SetGuildRuneRequesterList(TArray<FGUILD_RUNE_REQUESTER>& guildRuneRequests);
	void UpdateGuildRuneRequester(FGUILD_RUNE_REQUESTER& guildRuneRequest);
	bool GetMyRuneRequest(FGUILD_RUNE_REQUESTER& outMyRuneRequest);
	UPROPERTY()
	TArray<FGUILD_RUNE_REQUESTER>	RuneRequests;


public:
	//////////////////////////////////////////////////////////////////////////
	// Guild Raid
	void SetGuildRaidStatusInfos(TArray<FGUILD_RAID> RaidInfos, int32 remainTime );
	void SetGuildRaidStatusInfo(TArray<FGUILD_RAID_CHALLENGE>& Inhistorys, FGUILD_RAID& InRaidInfo);

	UPROPERTY()
	TArray<FGUILD_RAID>		RaidStatusInfos;
	UPROPERTY()
	TArray<FGUILD_RAID_CHALLENGE>		RaidHistorys;
	UPROPERTY()
	FGuildRaidSeasonStatus	RaidSeasonStatus;
	//////////////////////////////////////////////////////////////////////////
	UPROPERTY()
	int32					RemainTime;
	UPROPERTY()
	int32					RaidBossIndex = 0;

private:

// 	UPROPERTY()
// 	URGameInstance* RGameInstance = nullptr;

};
 