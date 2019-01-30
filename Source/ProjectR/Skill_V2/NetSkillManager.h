// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Network/RTSManager.h"
#include "NetSkillManager.generated.h"



class URGameInstance;
class AActor;
class USkillEntityComponent;
struct FWorkedSkillTableInfo;

const float RECEIVE_MAX_WAIT_TIME = 5.0f;


USTRUCT()
struct FRecvWaitData
{
	GENERATED_USTRUCT_BODY()

	TWeakObjectPtr<USkillEntityComponent>	SendSkillEntityComp;
	float									SendElapsedTime = 0.0f;
	float									RecvWaitTime = 0.0f;

	void Clear(int32& Count)
	{
		SendSkillEntityComp.Reset();
		SendElapsedTime = 0.0f;
		RecvWaitTime = 0.0f;

		if (1 < Count)
		{
			--Count;
		}
		else
		{
			Count = 0;
		}
	}
};


USTRUCT()
struct FNetSkillUseData
{
	GENERATED_USTRUCT_BODY()

	UISkill_V2*					UseSkill = nullptr;
	TWeakObjectPtr<AActor>		CasterPtr = nullptr;

	TWeakObjectPtr<AActor>		TargetPtr = nullptr;
	FVector						LocationTarget = FVector::ZeroVector;

	TArray< TWeakObjectPtr<AActor> >				ApplyTargetList;
	
	bool IsValid()
	{
		return (CasterPtr.IsValid() && nullptr != UseSkill);
	}

	void Clear()
	{
		UseSkill = nullptr;
		CasterPtr = nullptr;

		TargetPtr = nullptr;
		LocationTarget = FVector::ZeroVector;

		ApplyTargetList.Empty();
	}

	FNetSkillUseData& operator = (const FNetSkillUseData& other)
	{
		UseSkill = other.UseSkill;
		CasterPtr = other.CasterPtr;

		TargetPtr = other.TargetPtr;
		LocationTarget = other.LocationTarget;

		ApplyTargetList.SetNum(other.ApplyTargetList.Num());
		ApplyTargetList = other.ApplyTargetList;

		return *this;
	}
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UNetSkillManager : public UObject
{
	GENERATED_BODY()

public:
	void Init(URGameInstance* game_inst);
	void Update(float delta_time);

	UFUNCTION()
	bool TrySkillCasting(AActor* Caster, UISkill_V2* CastSkill, AActor* ActorTarget, FVector LocationTarget);

	void RecvRTS_SkillFailed(int32 CasterUK , uint8 errortype);

	void RecvRTS_CastSkill(OVERLORD::RTS::S2C::ACK_BT_SKILL_CASTEDT* RecvData);
	void RecvRTS_ApplySkill(OVERLORD::RTS::S2C::ACK_BT_SKILL_CASTEDT* RecvData);
	void RecvRTS_IntervalSkill(OVERLORD::RTS::S2C::ACK_BT_SKILL_CASTED_INTERVALT* RecvData);
	void RecvRTS_UpdateSkillSlot(OVERLORD::RTS::S2C::ACK_BT_SYNC_SKILL_UPDATET* RecvData);
	void RecvRTS_UpdateSkillCool(OVERLORD::RTS::S2C::ACK_BT_SKILL_ACTIVATEDT* RecvData);
	void RecvRTS_SkillBlocked(OVERLORD::RTS::S2C::ACK_BT_SKILL_BLOCKEDT* RecvData);

	// BattleField (CS Style)
protected:
	bool SendRTS_CastSkill(UISkill_V2* Skill, AActor* Caster, AActor* ActorTarget, FVector LocationTarget);

	void SetSkillApplyData(FSkillSyncApplyData& ApplyData, OVERLORD::RTS::STRUCT::ACTOR_SNA_UPDATET* TargetInfo);

	void SetLocation(FVector& Location, OVERLORD::RTS::STRUCT::RTS_VECTORT* RecvLocation);
	void SetRotation(FRotator& Rotation, OVERLORD::RTS::STRUCT::RTS_VECTORT* RecvDirection);

	// Common
	bool CheckCasterCondition(AActor* Caster);
	void CheckReceiveWaitTime(float DeltaTime);
	void SetReceiveWaitInfo(USkillEntityComponent* CasterSkillEntityComp);
	void ClearReceiveWaitInfo(USkillEntityComponent* CasterSkillEntityComp);

private:
	URGameInstance*			RGameInstance = nullptr;

	bool					InitNetSkillMgr = false;

	int32					RecvWaitCount = 0;
	TArray<FRecvWaitData>	RecvWaitDataList;
};
