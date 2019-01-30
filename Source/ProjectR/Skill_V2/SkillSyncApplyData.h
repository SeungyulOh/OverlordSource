#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "SkillApplierBase.h"
#include "SkillSyncApplyData.generated.h"

USTRUCT()
struct FSkillSyncSlotApplierData
{
	GENERATED_USTRUCT_BODY()

	bool					Remove = false;
	FSkillApplierBase		SlotApplier;

	// Attack(VE_i_p_attack, VE_i_m_attack...)
	int32					SyncHP = 0;
	EBattleDamageResultEnum	DamageResult = EBattleDamageResultEnum::VE_None;
	float					Damage = 0.0f;

	// Use Teleport or Knock back
	FVector					TargetLoc = FVector::ZeroVector;
	FRotator				TargetRot = FRotator::ZeroRotator;

	void Clear()
	{
		Remove = false;

		SlotApplier.SkillApplierType = ESkillApplierTypeEnum::VE_Applier_Max;
		SlotApplier.TargetValue = 0;
		SlotApplier.TargetPercent = 0.0f;
		SlotApplier.CasterMagicPower = 0.0f;

		SyncHP = 0;
		DamageResult = EBattleDamageResultEnum::VE_None;
		Damage = 0.0f;

		TargetLoc = FVector::ZeroVector;
		TargetRot = FRotator::ZeroRotator;
	}

};

USTRUCT()
struct FSkillSyncApplyData
{
	GENERATED_USTRUCT_BODY()

	TWeakObjectPtr<AActor>			Target;

	TArray<EAbnormalSlotTypeEnum>	ApplySlots;
	TArray<bool>					RemoveSlots; // false == add

	TArray<FSkillSyncSlotApplierData>	Appliers;

	void Clear()
	{
		Target = nullptr;

		ApplySlots.Empty();
		RemoveSlots.Empty();

		Appliers.Empty();
	}

	bool GetSlotApplier(ESkillApplierTypeEnum ApplierType, FSkillSyncSlotApplierData& ApplierData)
	{
		for (auto SynctApplierData : Appliers)
		{
			if (false == SynctApplierData.Remove &&
				SynctApplierData.SlotApplier.SkillApplierType == ApplierType)
			{
				ApplierData = SynctApplierData;
				return true;
			}
		}
		return false;
	}

	int32 GetApplySlot(EAbnormalSlotTypeEnum SlotType)
	{
		int32 SlotCount = ApplySlots.Num();
		for (int32 n = 0; n < SlotCount; ++n)
		{
			if (false == RemoveSlots[n])
			{
				if (SlotType == ApplySlots[n])
				{
					return n;
				}
			}
		}
		return -1;
	}

	FSkillSyncApplyData()
	{
		Clear();
	}
};

USTRUCT()
struct FSkillSyncData
{
	GENERATED_USTRUCT_BODY()

	bool						ValidData = false;
	bool						isCooling = false;

	TArray<FSkillSyncApplyData>	TargetApplyDatas;

	FSkillSyncApplyData			CasterApplyData;

	bool IsValid() { return ValidData; }

	void Clear()
	{
		ValidData = false;

		TargetApplyDatas.Empty();
		CasterApplyData.Clear();
	}
};