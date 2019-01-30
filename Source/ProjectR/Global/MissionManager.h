#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"

#include "MissionManager.generated.h"

// static void DAILYMISSION_STATUS_RQ(bool activate, int32 floor);
// static void DAILYMISSION_REWARD_RQ(FString id);
// static void DAILYMISSION_ADD_RQ();
// static void DAILYMISSION_RESET_RQ(TArray<FString> idList, bool isSpecial);

// static bool OnDAILYMISSION_STATUS_RP(FDAILYMISSION_STATUS_RP& dailymission_status_rp, URGameInstance* instance);
// static bool OnDAILYMISSION_REWARD_RP(FDAILYMISSION_REWARD_RP& dailymission_reward_rp, URGameInstance* instance);
// static bool OnDAILYMISSION_ADD_RP(FDAILYMISSION_ADD_RP& dailymission_add_rp, URGameInstance* instance);
// static bool OnDAILYMISSION_RESET_RP(FDAILYMISSION_RESET_RP& dailymission_reset_rp, URGameInstance* instance);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnMissionStatusRp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnFloorMissionRewardGetRp, TArray<FREWARD>&, InRewards, TArray<FREWARD>&, InExtraRewards);

#define INVALID_TARGET_FLOOR -1

UCLASS()
class PROJECTR_API UMissionManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize();

	FORCEINLINE void SetMissionStatus(FFLOOR_MISSION_STATUS updateData) { missionStatus = updateData; }
	FORCEINLINE FFLOOR_MISSION_STATUS GetMissionStatus() { return missionStatus; };

	TArray<FString> GetSelectedIDList() { return SelectedIDList; }

	/*
		@ return value : 추가되었으면 true
	*/
	bool CheckID(FString InId);
	bool IsContainsID(FString InId);
public:
	UPROPERTY()
	FDelegate_OnMissionStatusRp		OnMissionStatusRp;
	UPROPERTY()
	FDelegate_OnMissionStatusRp	ReceiveGetFloorMission;
	UPROPERTY()
	FDelegate_OnFloorMissionRewardGetRp		OnMissionRewardGetRp;
private:
	void ResetData();
	void AddID(FString InId);
	void RemoveID(FString InId);


private:
	UPROPERTY()
	TArray<FString>					SelectedIDList;
	FFLOOR_MISSION_STATUS missionStatus;

public:
	FName StageTargetFloor;
};

