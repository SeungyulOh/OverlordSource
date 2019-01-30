// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "BattleRecordManager.generated.h"


//class URGameInstance;
class UISkill_V2;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnBattleRecord_UpdateHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnBattleRecord_UpdateKillCount, AActor*, DeadActor, AActor*, FromActor);

const int32 BATTLE_RECORD_MAX = 50;
const int32 DAMAGE_METER_RANK_MAX = 5;


USTRUCT(BlueprintType)
struct FBattleRecord
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<AActor>	ptrActor;

	int32	PlayerUK = PLAYER_UK_NONE;
	FName	CharacterID = "";
	int32	UserKID = 0;
	int32	GroupKey = 0;

	FString	NickName = "";

	int32	nAmountOfNormalAttack = 0;
	int32	nAmountOfSkillAttack = 0;
	int32	nAmountOfDamage = 0;
	int32	nAmountOfHeal = 0;
	int32	nKillCount = 0;
	int32	nSkillCount = 0;

	void Clear()
	{
		ptrActor.Reset();

		PlayerUK = PLAYER_UK_NONE;
		CharacterID = "";
		UserKID = 0;
		GroupKey = 0;

		NickName = "";

		nAmountOfNormalAttack = 0;
		nAmountOfSkillAttack = 0;
		nAmountOfDamage = 0;
		nAmountOfHeal = 0;
		nKillCount = 0;
		nSkillCount = 0;
	}

	FBattleRecord& operator = (const FBattleRecord& other)
	{
		ptrActor = other.ptrActor.Get();

		PlayerUK = other.PlayerUK;
		CharacterID = other.CharacterID;
		UserKID = other.UserKID;
		GroupKey = other.GroupKey;

		NickName = other.NickName;

		nAmountOfNormalAttack = other.nAmountOfNormalAttack;
		nAmountOfSkillAttack = other.nAmountOfSkillAttack;
		nAmountOfDamage = other.nAmountOfDamage;
		nAmountOfHeal = other.nAmountOfHeal;
		nKillCount = other.nKillCount;
		nSkillCount = other.nSkillCount;

		return *this;
	}

	int32 GetAmountOfAttack()
	{
		return (nAmountOfNormalAttack + nAmountOfSkillAttack);
	}
};


USTRUCT()
struct FUserBattleRecord // Key : UserKID, all of User's heroes record on basis
{
	GENERATED_USTRUCT_BODY()

	int32					UserKID = 0;

	FString					UserNickName = "";
	FString					UserSubNick = "";
	TArray<int32>			HeroPlayerUKs;

	FBattleRecord			TotalRecord;
	TArray<FBattleRecord*>	HeroBattleRecordList;

	int32					AccumulatedAttack = 0;

	FBattleRecord* GetTotalRecord();

	int32 GetSumOfNormalAttack();
	int32 GetSumOfSkillAttack();
	int32 GetSumOfAllAttack();
	int32 GetSumOfDamage();
	int32 GetSumOfHeal();
	int32 GetSumOfKillCount();
	int32 GetSumOfSkillCount();
};


USTRUCT()
struct FGroupBattleRecord // Key : UserKID, all of User's heroes record on basis
{
	GENERATED_USTRUCT_BODY()

	TArray<int32>				UserKIDs;
	TArray<FUserBattleRecord*>	UserBattleRecordList;
};


USTRUCT()
struct FDamageMeterRank
{
	GENERATED_USTRUCT_BODY()

	int32	RankerKID = 0;
	int32	AccumulatedAttack = 0;

	FDamageMeterRank& operator = (const FDamageMeterRank& other)
	{
		RankerKID = other.RankerKID;
		AccumulatedAttack = other.AccumulatedAttack;

		return *this;
	}
};

// For battle stats and also display health bars on HUD.
UCLASS()
class PROJECTR_API UBattleRecordManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(/*URGameInstance* InGameInstance*/);

	void ResetBattleRecordData();
	void SetHeroBattleInfoDelegate(AActor* BattleActor);
	void SetUserNickName(int32 UserKID, FString& NickName, FString& SubNick);
	void InitRealTimeBattleRecord();
	void RemoveBattleInfoDelegate();

	UFUNCTION()
	void CheckKillCount(AActor* DeadActor, AActor* FromActor);

	UFUNCTION()
	void CheckAmountDamage(AActor* Attacker, AActor* Victim, int32 damage, bool SkillAttack = false);

	UFUNCTION()
	void CheckAmountHeal(float Heal, AActor* FromActor);

	UFUNCTION()
	void CheckSkillCount(AActor* Caster, UISkill_V2* UseSkill);

	void UpdateforRTSActorHP(int32 GroupKey);
	//////////////////////////////////////////////////////////////////////////
	// Temp for formal compile -----
	//bool GetKillCount(int32 GroupKey, TArray<int32>& count_list);
	//bool GetAmountOfAttack(int32 GroupKey, TArray<int32>& amount_list);
	//bool GetAmountOfDamage(int32 GroupKey, TArray<int32>& amount_list);
	//bool GetAmountOfHeal(int32 GroupKey, TArray<int32>& amount_list);
	//bool GetSkillCount(int32 GroupKey, TArray<int32>& count_list);
	int32 GetTeamTotalHP(int32 GroupKey, bool bMaxHP = false);

	//FBattleRecord* GetBattleRecord(int32 GroupKey, FString FindHeroUD);
	//------------------------------

	FBattleRecord* GetBattleRecord(AActor* hero_actor);
	FBattleRecord* GetBattleRecord(int32 PlayerUK);
	FUserBattleRecord* GetUserBattleRecord(int32 UserKID);
	FGroupBattleRecord* GetGroupBattleRecord(int32 GroupKey);

	void GetDamageMeterRank(TArray<FBattleRecord*>& DamageRankList);

private:
	void AddBattleRecord(AActor* BattleActor, bool AddNew);
	void AddUserBattleRecord(int32 UserKID, int32 GroupKey, int32 PlayerUK);
	void AddGroupBattleRecord(int32 GroupKey, int32 UserKID);

	void AttachBattleInfoDelegate(AActor* TargetActor);
	void DetachBattleInfoDelegate(AActor* TargetActor);

	void ResetDamageMeterRank();
	void UpdateDamageMeterRank(FUserBattleRecord* UserRecord);

public:
	// [PlayerUK]
	TMap<int32, FBattleRecord>		BattleRecordMap;
	// [KID]
	TMap<int32, FUserBattleRecord>	BattleRecordUserKIDMap;
	// [GroupKey]
	TMap<int32, FGroupBattleRecord>	BattleRecordGroupMap;

	TArray<FDamageMeterRank>		DamageMeterRankerList;

	UPROPERTY()
	FDelegate_OnBattleRecord_UpdateHP			OnBattleRecord_UpdateHP;
	UPROPERTY()
	FDelegate_OnBattleRecord_UpdateKillCount	OnBattleRecord_UpdateKillCount;

private:
// 	UPROPERTY()
// 	URGameInstance*			RGameInstance = nullptr;
};
