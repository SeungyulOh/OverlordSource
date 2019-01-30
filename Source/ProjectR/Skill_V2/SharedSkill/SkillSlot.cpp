// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillSlot.h"
#include "GlobalIntegrated.h"



#include "Table/ItemTableInfo.h"
#include "Table/SkillTableInfo_V2.h"
#include "Table/AbnormalStateGroupTableInfo_V2.h"

// #include "Utils/AIFunctionLibrary.h"


#include "Character/AnimInstance/AnimInstanceDefs.h"
#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SkillApplierBase.h"
#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillPassiveSlotContainer.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/DamageFloater/SlateDamageFloaterComponentV2.h"
#include "Character/Components/CustomChangeMaterialComponent.h"
#include "Character/Components/SpecialMovingComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"

#include "Character/SharedBattle/BattleOperator.h"

#include "CustomStructures/SharedStructures/ProtectInfo.h"




USkillSlot::USkillSlot()
{
	bActivated = false;	
	SkillApplierList.Empty();
}

bool USkillSlot::IsInBlockCancelTime()
{
	return (ElapsedTime < BlockCancelTime);
}

bool USkillSlot::Update(float DeltaTime)
{
	if (bActivated && Duration > 0 )
	{
		CheckContinuousApplier(DeltaTime);
				
		ElapsedTime += DeltaTime;		
		if ( ElapsedTime >= Duration)
		{
			CloseSlot();
			return false;
		}
	}

	return true;
}

bool USkillSlot::OpenSlot(FName InSkillTableID, bool InIsCaster, AActor* InOwner, AActor* InCaster, UISkill_V2* AppliedSkill)
{
	checkf(InOwner != nullptr, TEXT("USkillPassiveSlotContainer::OpenSlot[InOwner Is Empty]"));
	checkf(InCaster != nullptr, TEXT("USkillPassiveSlotContainer::OpenSlot[InCaster Is Empty]"));
	checkf(AppliedSkill != nullptr, TEXT("USkillPassiveSlotContainer::OpenSlot[AppliedSkill Is Empty]"));

	SkillTableID = InSkillTableID;
	IEntityBaseProperty* CasterEntityBaseProperty = Cast< IEntityBaseProperty >(InCaster);
	if (CasterEntityBaseProperty == nullptr)
		return false;
	FSkillTableInfo_V2* SkillTableInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(CasterEntityBaseProperty->GetCharacterCategory(), SkillTableID);

	bActivated = true;
	OwnerActor = InOwner;
	CasterActor = InCaster;

	OwnerBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwner);
	if (false == OwnerBattleEntityComponent.IsValid())
		return false;

	if (false == IsValid(OwnerBattleEntityComponent->GetBattleInfo()))
		return false;

	CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InCaster);
	if (false == CasterBattleEntityComponent.IsValid())
		return false;

	CasterSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InCaster);
	if (false == CasterSkillEntityComponent.IsValid())
		return false;

	bForceDetach = false;

	SkillInst = AppliedSkill;
	SkillID = SkillInst->GetSkillTableID();

	if (InIsCaster)	GroupType = SkillTableInfo->CasterGroupType;
	else			GroupType = SkillTableInfo->TargetGroupType;

	IsCasterSlot = InIsCaster;

	SetApplierInfo();

	if (!IsPassiveSlot() && Duration <= 0.0f)
	{
		CloseSlot();
		return true;
	}

	ElapsedTime = 0.0f;
	IntervalElapsedTime = 0.0f;

	AttachSlotEffect();

	SetAbnormalState(true);

#if LOG_SKILLDEBUG
	IEntityBaseProperty* BattleBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (BattleBaseProperty)
	{
		UE_LOG(LogBattle, Log, TEXT(" $$$$$$$$$$$$$$$$$$$  Open : %d - %d \n"), (int32)SlotType, BattleBaseProperty->GetPlayerUK());
	}
#endif

	return true;
}


void USkillSlot::CloseSlot()
{
#if LOG_SKILLDEBUG
	IEntityBaseProperty* BattleBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (BattleBaseProperty)
	{
		UE_LOG(LogBattle, Log, TEXT(" $$$$$$$$$$$$$$$$$$$  Close : %d - notify : %d- %d\n"), (int32)SlotType, BattleBaseProperty->GetPlayerUK());
	}
#endif

	// Detach persistent applier
	for (FSkillApplierBase& SkillApplier : SkillApplierList)
	{
		bool isCApplier = FMath::IsWithin(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_c_BEGIN, ESkillApplierTypeEnum::VE_c_END);
		bool isPApplier = FMath::IsWithin(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_BEGIN, ESkillApplierTypeEnum::VE_p_END);
		bool isTApplier = FMath::IsWithin(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_t_BEGIN, ESkillApplierTypeEnum::VE_t_END);

		if (isCApplier)
			AttachContinousApplier(SkillApplier, false);
		else if (isPApplier)
			AttachPersistentApplier(SkillApplier, false);
		else if (isTApplier)
			AttachConditionalApplier(SkillApplier, false);
	}

	DetachSlotEffect();

	bActivated = false;
	SetAbnormalState(false);
	SkillApplierList.Empty();

	Duration = 0.0f;
	SlotLevel = 0;	
	
	SkillTableID = NAME_None;
	RuneSkillTableID = NAME_None;
	TowerSkillTableID = NAME_None;
	SpiritSkillTableID = NAME_None;

	OwnerActor = nullptr;
	CasterActor = nullptr;
	/*
	Cache component
	*/
	CasterBattleEntityComponent = nullptr;
	OwnerBattleEntityComponent = nullptr;
}

bool USkillSlot::IsPassiveSlot()
{
	if (SlotType >= EAbnormalSlotTypeEnum::VE_Slot_Passive )
		return true;

	return false;
}

void USkillSlot::SetApplierInfo()
{
	SkillApplierList.Empty();

	AActor* CasterActorPtr = CasterActor.Get();
	if (CasterActorPtr == nullptr)
		return;
	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(CasterActorPtr);
	if (!IsValid(SkillComponent))
		return;
	
	IEntityBaseProperty* CasterEntityBaseProperty = Cast< IEntityBaseProperty >(CasterActorPtr);
	if (CasterEntityBaseProperty == nullptr)
		return;
	FSkillTableInfo_V2* SkillTableInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(CasterEntityBaseProperty->GetCharacterCategory(), SkillTableID);
	
	checkf(SkillTableInfo != nullptr, TEXT("USkillSlot::SetApplierInfo[SkillTableInfo is empty]"));

	AActor* OwnerActorPtr = OwnerActor.Get();
	IntervalTime = SkillTableInfo->Interval;

	FSkillApplierBase SkillApplier;
	ESkillApplierTypeEnum ApplierTypes[6];
	float Percents[6];
	int32 Values[6];
	if (IsCasterSlot)
	{
		ApplierTypes[0] = SkillTableInfo->CasterEffect1;
		ApplierTypes[1] = SkillTableInfo->CasterEffect2;
		ApplierTypes[2] = SkillTableInfo->CasterEffect3;
		ApplierTypes[3] = SkillTableInfo->CasterEffect4;
		ApplierTypes[4] = SkillTableInfo->CasterEffect5;
		ApplierTypes[5] = SkillTableInfo->CasterEffect6;

		Percents[0] = SkillTableInfo->CasterPercent1;
		Percents[1] = SkillTableInfo->CasterPercent2;
		Percents[2] = SkillTableInfo->CasterPercent3;
		Percents[3] = SkillTableInfo->CasterPercent4;
		Percents[4] = SkillTableInfo->CasterPercent5;
		Percents[5] = SkillTableInfo->CasterPercent6;

		Values[0] = SkillTableInfo->CasterValue1;
		Values[1] = SkillTableInfo->CasterValue2;
		Values[2] = SkillTableInfo->CasterValue3;
		Values[3] = SkillTableInfo->CasterValue4;
		Values[4] = SkillTableInfo->CasterValue5;
		Values[5] = SkillTableInfo->CasterValue6;

		SlotLevel = SkillTableInfo->CasterSlotLevel;
		Duration = SkillTableInfo->CasterDuration;
	}
	else
	{
		ApplierTypes[0] = SkillTableInfo->TargetEffect1;
		ApplierTypes[1] = SkillTableInfo->TargetEffect2;
		ApplierTypes[2] = SkillTableInfo->TargetEffect3;
		ApplierTypes[3] = SkillTableInfo->TargetEffect4;
		ApplierTypes[4] = SkillTableInfo->TargetEffect5;
		ApplierTypes[5] = SkillTableInfo->TargetEffect6;

		Percents[0] = SkillTableInfo->TargetPercent1;
		Percents[1] = SkillTableInfo->TargetPercent2;
		Percents[2] = SkillTableInfo->TargetPercent3;
		Percents[3] = SkillTableInfo->TargetPercent4;
		Percents[4] = SkillTableInfo->TargetPercent5;
		Percents[5] = SkillTableInfo->TargetPercent6;

		Values[0] = SkillTableInfo->TargetValue1;
		Values[1] = SkillTableInfo->TargetValue2;
		Values[2] = SkillTableInfo->TargetValue3;
		Values[3] = SkillTableInfo->TargetValue4;
		Values[4] = SkillTableInfo->TargetValue5;
		Values[5] = SkillTableInfo->TargetValue6;

		SlotLevel = SkillTableInfo->TargetSlotLevel;
		Duration = SkillTableInfo->TargetDuration;
	}

	for (int32 n = 0; n < 6; ++n)
	{
		if (ApplierTypes[n] != ESkillApplierTypeEnum::VE_None)
		{
			SkillApplier.SkillApplierType = ApplierTypes[n];
			SkillApplier.TargetPercent = Percents[n];
			SkillApplier.TargetValue = Values[n];
			SkillApplier.CasterMagicPower = 0.0f;

			bool isIApplier = FMath::IsWithin(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_i_BEGIN, ESkillApplierTypeEnum::VE_i_END);
			bool isCApplier = FMath::IsWithin(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_c_BEGIN, ESkillApplierTypeEnum::VE_c_END);
			bool isPApplier = FMath::IsWithin(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_BEGIN, ESkillApplierTypeEnum::VE_p_END);
			bool isTApplier = FMath::IsWithin(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_t_BEGIN, ESkillApplierTypeEnum::VE_t_END);
			if (isIApplier)
			{
				SkillComponent->ApplyInstanceApplier(CasterActorPtr, OwnerActorPtr, SkillApplier, SkillInst);
			}
			else if (isCApplier)
			{
				if(AttachContinousApplier(SkillApplier, true))
					SkillApplierList.Emplace(SkillApplier);
			}
			else if (isPApplier)
			{
				if (AttachPersistentApplier(SkillApplier, true))
					SkillApplierList.Emplace(SkillApplier);
			}
			else if (isTApplier)
			{
				if (AttachConditionalApplier(SkillApplier, true))
					SkillApplierList.Emplace(SkillApplier);
			}
		}
	}
}

void USkillSlot::SetRuneApplierInfo()
{
	SkillApplierList.Empty();

	AActor* CasterActorPtr = CasterActor.Get();
	if (CasterActorPtr == nullptr)
		return;
	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(CasterActorPtr);
	if (!IsValid(SkillComponent))
		return;

	FRuneWordSkillTable* RuneSkillTableInfo = UTableManager::GetInstancePtr()->GetRuneWordSkillRow(RuneSkillTableID);

	checkf(RuneSkillTableInfo != nullptr, TEXT("USkillSlot::SetApplierInfo[RuneSkillTableInfo is empty]"));

	AActor* OwnerActorPtr = OwnerActor.Get();
	IntervalTime = RuneSkillTableInfo->Interval;

	FSkillApplierBase SkillApplier;
	ESkillApplierTypeEnum ApplierTypes[6];
	float Percents[6];
	int32 Values[6];
	if (IsCasterSlot)
	{
		ApplierTypes[0] = RuneSkillTableInfo->CasterEffect1;
		ApplierTypes[1] = RuneSkillTableInfo->CasterEffect2;
		ApplierTypes[2] = RuneSkillTableInfo->CasterEffect3;
		ApplierTypes[3] = RuneSkillTableInfo->CasterEffect4;
		ApplierTypes[4] = RuneSkillTableInfo->CasterEffect5;
		ApplierTypes[5] = RuneSkillTableInfo->CasterEffect6;

		Percents[0] = RuneSkillTableInfo->CasterPercent1;
		Percents[1] = RuneSkillTableInfo->CasterPercent2;
		Percents[2] = RuneSkillTableInfo->CasterPercent3;
		Percents[3] = RuneSkillTableInfo->CasterPercent4;
		Percents[4] = RuneSkillTableInfo->CasterPercent5;
		Percents[5] = RuneSkillTableInfo->CasterPercent6;

		Values[0] = RuneSkillTableInfo->CasterValue1;
		Values[1] = RuneSkillTableInfo->CasterValue2;
		Values[2] = RuneSkillTableInfo->CasterValue3;
		Values[3] = RuneSkillTableInfo->CasterValue4;
		Values[4] = RuneSkillTableInfo->CasterValue5;
		Values[5] = RuneSkillTableInfo->CasterValue6;

		SlotLevel = RuneSkillTableInfo->CasterSlotLevel;
		Duration = RuneSkillTableInfo->CasterDuration;
	}
	else
	{
		ApplierTypes[0] = RuneSkillTableInfo->TargetEffect1;
		ApplierTypes[1] = RuneSkillTableInfo->TargetEffect2;
		ApplierTypes[2] = RuneSkillTableInfo->TargetEffect3;
		ApplierTypes[3] = RuneSkillTableInfo->TargetEffect4;
		ApplierTypes[4] = RuneSkillTableInfo->TargetEffect5;
		ApplierTypes[5] = RuneSkillTableInfo->TargetEffect6;

		Percents[0] = RuneSkillTableInfo->TargetPercent1;
		Percents[1] = RuneSkillTableInfo->TargetPercent2;
		Percents[2] = RuneSkillTableInfo->TargetPercent3;
		Percents[3] = RuneSkillTableInfo->TargetPercent4;
		Percents[4] = RuneSkillTableInfo->TargetPercent5;
		Percents[5] = RuneSkillTableInfo->TargetPercent6;

		Values[0] = RuneSkillTableInfo->TargetValue1;
		Values[1] = RuneSkillTableInfo->TargetValue2;
		Values[2] = RuneSkillTableInfo->TargetValue3;
		Values[3] = RuneSkillTableInfo->TargetValue4;
		Values[4] = RuneSkillTableInfo->TargetValue5;
		Values[5] = RuneSkillTableInfo->TargetValue6;

		SlotLevel = RuneSkillTableInfo->TargetSlotLevel;
		Duration = RuneSkillTableInfo->TargetDuration;
	}

	for (int32 n = 0; n < 6; ++n)
	{
		if (ApplierTypes[n] != ESkillApplierTypeEnum::VE_None)
		{
			SkillApplier.SkillApplierType = ApplierTypes[n];
			SkillApplier.TargetPercent = Percents[n];
			SkillApplier.TargetValue = Values[n];
			SkillApplier.CasterMagicPower = 0.0f;

			if (false == SkillComponent->ApplyInstanceApplier(CasterActorPtr, OwnerActorPtr, SkillApplier, SkillInst))
			{
				if (AttachPersistentApplier(SkillApplier, true))
				{
					SkillApplierList.Emplace(SkillApplier);
				}
			}
		}
	}
}

void USkillSlot::SetTowerApplierInfo()
{
	SkillApplierList.Empty();

	AActor* CasterActorPtr = CasterActor.Get();
	if (CasterActorPtr == nullptr)
		return;
	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(CasterActorPtr);
	if (!IsValid(SkillComponent))
		return;

	FTowerSkillTable* TowerSKillTableInfo = UTableManager::GetInstancePtr()->GetTowerSkillRow(TowerSkillTableID);//GetRuneWordSkillRow(RuneSkillTableID);

	checkf(TowerSKillTableInfo != nullptr, TEXT("USkillSlot::SetTowerApplierInfo[TowerSKillTableInfo is empty]"));

	AActor* OwnerActorPtr = OwnerActor.Get();
	IntervalTime = TowerSKillTableInfo->Interval;

	FSkillApplierBase SkillApplier;
	ESkillApplierTypeEnum ApplierTypes[6];
	float Percents[6];
	int32 Values[6];
	if (IsCasterSlot)
	{
		ApplierTypes[0] = TowerSKillTableInfo->CasterEffect1;
		ApplierTypes[1] = TowerSKillTableInfo->CasterEffect2;
		ApplierTypes[2] = TowerSKillTableInfo->CasterEffect3;
		ApplierTypes[3] = TowerSKillTableInfo->CasterEffect4;
		ApplierTypes[4] = TowerSKillTableInfo->CasterEffect5;
		ApplierTypes[5] = TowerSKillTableInfo->CasterEffect6;

		Percents[0] = TowerSKillTableInfo->CasterPercent1;
		Percents[1] = TowerSKillTableInfo->CasterPercent2;
		Percents[2] = TowerSKillTableInfo->CasterPercent3;
		Percents[3] = TowerSKillTableInfo->CasterPercent4;
		Percents[4] = TowerSKillTableInfo->CasterPercent5;
		Percents[5] = TowerSKillTableInfo->CasterPercent6;

		Values[0] = TowerSKillTableInfo->CasterValue1;
		Values[1] = TowerSKillTableInfo->CasterValue2;
		Values[2] = TowerSKillTableInfo->CasterValue3;
		Values[3] = TowerSKillTableInfo->CasterValue4;
		Values[4] = TowerSKillTableInfo->CasterValue5;
		Values[5] = TowerSKillTableInfo->CasterValue6;

		SlotLevel = TowerSKillTableInfo->CasterSlotLevel;
		Duration = TowerSKillTableInfo->CasterDuration;
	}
	else
	{
		ApplierTypes[0] = TowerSKillTableInfo->TargetEffect1;
		ApplierTypes[1] = TowerSKillTableInfo->TargetEffect2;
		ApplierTypes[2] = TowerSKillTableInfo->TargetEffect3;
		ApplierTypes[3] = TowerSKillTableInfo->TargetEffect4;
		ApplierTypes[4] = TowerSKillTableInfo->TargetEffect5;
		ApplierTypes[5] = TowerSKillTableInfo->TargetEffect6;

		Percents[0] = TowerSKillTableInfo->TargetPercent1;
		Percents[1] = TowerSKillTableInfo->TargetPercent2;
		Percents[2] = TowerSKillTableInfo->TargetPercent3;
		Percents[3] = TowerSKillTableInfo->TargetPercent4;
		Percents[4] = TowerSKillTableInfo->TargetPercent5;
		Percents[5] = TowerSKillTableInfo->TargetPercent6;

		Values[0] = TowerSKillTableInfo->TargetValue1;
		Values[1] = TowerSKillTableInfo->TargetValue2;
		Values[2] = TowerSKillTableInfo->TargetValue3;
		Values[3] = TowerSKillTableInfo->TargetValue4;
		Values[4] = TowerSKillTableInfo->TargetValue5;
		Values[5] = TowerSKillTableInfo->TargetValue6;

		SlotLevel = TowerSKillTableInfo->TargetSlotLevel;
		Duration = TowerSKillTableInfo->TargetDuration;
	}

	for (int32 n = 0; n < 6; ++n)
	{
		if (ApplierTypes[n] != ESkillApplierTypeEnum::VE_None)
		{
			SkillApplier.SkillApplierType = ApplierTypes[n];
			SkillApplier.TargetPercent = Percents[n];
			SkillApplier.TargetValue = Values[n];
			SkillApplier.CasterMagicPower = 0.0f;

			if (false == SkillComponent->ApplyInstanceApplier(CasterActorPtr, OwnerActorPtr, SkillApplier, SkillInst))
			{
				if (AttachPersistentApplier(SkillApplier, true))
				{
					SkillApplierList.Emplace(SkillApplier);
				}
			}
		}
	}
}

void USkillSlot::SetSpiritApplierInfo()
{
	SkillApplierList.Empty();

	AActor* CasterActorPtr = CasterActor.Get();
	if (CasterActorPtr == nullptr)
		return;
	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(CasterActorPtr);
	if (!IsValid(SkillComponent))
		return;
	FSkillTableInfo_V2* SpiritSkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SpiritSkillTableID);

	checkf(SpiritSkillTableInfo != nullptr, TEXT("USkillSlot::SetSpiritApplierInfo[SpiritSkillTableInfo is empty]"));

	AActor* OwnerActorPtr = OwnerActor.Get();
	IntervalTime = SpiritSkillTableInfo->Interval;

	FSkillApplierBase SkillApplier;
	ESkillApplierTypeEnum ApplierTypes[6];
	float Percents[6];
	int32 Values[6];

	ApplierTypes[0] = SpiritSkillTableInfo->CasterEffect1;
	ApplierTypes[1] = SpiritSkillTableInfo->CasterEffect2;
	ApplierTypes[2] = SpiritSkillTableInfo->CasterEffect3;
	ApplierTypes[3] = SpiritSkillTableInfo->CasterEffect4;
	ApplierTypes[4] = SpiritSkillTableInfo->CasterEffect5;
	ApplierTypes[5] = SpiritSkillTableInfo->CasterEffect6;

	Percents[0] = SpiritSkillTableInfo->CasterPercent1;
	Percents[1] = SpiritSkillTableInfo->CasterPercent2;
	Percents[2] = SpiritSkillTableInfo->CasterPercent3;
	Percents[3] = SpiritSkillTableInfo->CasterPercent4;
	Percents[4] = SpiritSkillTableInfo->CasterPercent5;
	Percents[5] = SpiritSkillTableInfo->CasterPercent6;

	Values[0] = SpiritSkillTableInfo->CasterValue1;
	Values[1] = SpiritSkillTableInfo->CasterValue2;
	Values[2] = SpiritSkillTableInfo->CasterValue3;
	Values[3] = SpiritSkillTableInfo->CasterValue4;
	Values[4] = SpiritSkillTableInfo->CasterValue5;
	Values[5] = SpiritSkillTableInfo->CasterValue6;

	SlotLevel = SpiritSkillTableInfo->CasterSlotLevel;
	Duration = SpiritSkillTableInfo->CasterDuration;

	for (int32 n = 0; n < 6; ++n)
	{
		if (ApplierTypes[n] != ESkillApplierTypeEnum::VE_None)
		{
			SkillApplier.SkillApplierType = ApplierTypes[n];
			SkillApplier.TargetPercent = Percents[n];
			SkillApplier.TargetValue = Values[n];
			SkillApplier.CasterMagicPower = 0.0f;

			if (AttachPersistentApplier(SkillApplier, true))
			{
				SkillApplierList.Emplace(SkillApplier);
			}
		}
	}

}

bool USkillSlot::AttachPersistentApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach )
{
	bool isStatUpApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_stat_up_BEGIN, ESkillApplierTypeEnum::VE_p_stat_up_END);
	if (isStatUpApplier)
	{
		AttachPersistentStatUpDownApplier(InSkillApplier, InIsAttach);
		return true;
	}

	bool isBlockApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_block_BEGIN, ESkillApplierTypeEnum::VE_p_block_END);
	if (isBlockApplier)
	{
		//Todo :
		return true;
	}

	bool isAbnormalApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Abnormal_BEGIN, ESkillApplierTypeEnum::VE_p_Abnormal_END);
	if (isAbnormalApplier)
	{
		AttachPersistentAbnormalApplier(InSkillApplier, InIsAttach);
		return true;
	}

	bool isStatusApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Status_BEGIN, ESkillApplierTypeEnum::VE_p_Status_END);
	if (isStatusApplier)
	{
		AttachPersistentStateApplier(InSkillApplier, InIsAttach);
		return true;
	}

	bool isCounterApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Counter_BEGIN, ESkillApplierTypeEnum::VE_p_Counter_END);
	if (isCounterApplier)
	{
		AttachAdditionalDamageProperty(InSkillApplier, InIsAttach);
		return true;
	}

	bool isSpiritApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Spirit_BEGIN, ESkillApplierTypeEnum::VE_p_Spirit_END);
	if (isSpiritApplier)
	{
		AttachSpiritProperty(InSkillApplier, InIsAttach);
		return true;
	}

	return false;
}

void USkillSlot::AttachPersistentStatUpDownApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach)
{
	ECharacterPropertyTypeEnum TargetPropertyType = ECharacterPropertyTypeEnum::VE_None;
	switch (InSkillApplier.SkillApplierType)
	{
		case ESkillApplierTypeEnum::VE_p_stat_up_PhysicalAttackPower:	TargetPropertyType = ECharacterPropertyTypeEnum::VE_PhysicalAttackPower; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_MagicAttackPower:		TargetPropertyType = ECharacterPropertyTypeEnum::VE_MagicAttackPower; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_AccuracyValue:			TargetPropertyType = ECharacterPropertyTypeEnum::VE_AccuracyValue; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_PierceValue:			TargetPropertyType = ECharacterPropertyTypeEnum::VE_PierceValue; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_AttackSpeed:			TargetPropertyType = ECharacterPropertyTypeEnum::VE_AttackSpeed; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_CriticalValue:			TargetPropertyType = ECharacterPropertyTypeEnum::VE_CriticalValue; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_CriticalDamageValue:	TargetPropertyType = ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_PhysicalDefenceValue:	TargetPropertyType = ECharacterPropertyTypeEnum::VE_Armor; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_MagicDefenceValue:		TargetPropertyType = ECharacterPropertyTypeEnum::VE_MagicResistance; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_AvoidValue:			TargetPropertyType = ECharacterPropertyTypeEnum::VE_DodgeValue; break;
// 		case ESkillApplierTypeEnum::VE_p_stat_up_ParryValue:			TargetPropertyType = ECharacterPropertyTypeEnum::VE_ParryValue; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_Maxhp:					TargetPropertyType = ECharacterPropertyTypeEnum::VE_MaxHP; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_MoveSpeed:				TargetPropertyType = ECharacterPropertyTypeEnum::VE_MoveSpeed; break;
		case ESkillApplierTypeEnum::VE_p_stat_up_AttackRange:			TargetPropertyType = ECharacterPropertyTypeEnum::VE_AttackRange; break;
		case ESkillApplierTypeEnum::VE_p_blindness:						TargetPropertyType = ECharacterPropertyTypeEnum::VE_AttackRange; break;
// 		case ESkillApplierTypeEnum::VE_p_stat_up_CriticalResistValue:	TargetPropertyType = ECharacterPropertyTypeEnum::VE_CriticalResistValue; break;
// 		case ESkillApplierTypeEnum::VE_p_stat_up_CriticalResistDamage:	TargetPropertyType = ECharacterPropertyTypeEnum::VE_CriticalResistDamage; break;
// 		case ESkillApplierTypeEnum::VE_p_stat_up_MagicCriticalRate:		TargetPropertyType = ECharacterPropertyTypeEnum::VE_MagicCriticalRate; break;
			break;

		case ESkillApplierTypeEnum::VE_p_stat_up_ModelScale:
		{
			if (InIsAttach)
			{
				//scale up
				if (OwnerActor.IsValid())
				{
					auto ScaleRate = 1.0f + static_cast<float>(InSkillApplier.TargetPercent) / 100.0f;
					OwnerActor.Get()->SetActorScale3D(FVector(ScaleRate, ScaleRate, ScaleRate));
					FVector FloorPos;
					if (UMapFunctionLibrary::GetValidPlacableProjectionPos(OwnerActor.Get(), OwnerActor->GetActorLocation(), FloorPos, OwnerActor.Get()))
					{
						OwnerActor->SetActorLocation(FloorPos);
					}

				}
			}
			else
			{
				//scale down
				if (OwnerActor.IsValid())
				{
					IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
					if (BaseProperty)
					{
						FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(BaseProperty->GetCharacterCategory(), BaseProperty->GetCharacterID());
						if (CharacterTableInfo)
							OwnerActor.Get()->SetActorScale3D(FVector(CharacterTableInfo->ModelScale, CharacterTableInfo->ModelScale, CharacterTableInfo->ModelScale));

						FVector FloorPos;
						if (UMapFunctionLibrary::GetValidPlacableProjectionPos(OwnerActor.Get(), OwnerActor->GetActorLocation(), FloorPos, OwnerActor.Get()))
						{
							OwnerActor->SetActorLocation(FloorPos);
						}
					}
				}
			}
			return;
		}
	}

	if (false == OwnerBattleEntityComponent.IsValid())
		return;

	float OwnerPropertyValue = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterNativeTotalProperty<float>(TargetPropertyType);
	float BonusValue = (float)OwnerPropertyValue * ((float)InSkillApplier.TargetPercent / 100.0f) + InSkillApplier.TargetValue;
	if (!InIsAttach)
		BonusValue *= -1.0f;
	OwnerBattleEntityComponent->GetBattleInfo()->AddCharacterAdditionalProperty<float>(TargetPropertyType, BonusValue);

	switch (TargetPropertyType)
	{
		case ECharacterPropertyTypeEnum::VE_MaxHP:
		{
			// 최대체력 대비 현재체력도 증가.
			OwnerBattleEntityComponent->AddHP(BonusValue, nullptr);
		}
		break;
		case ECharacterPropertyTypeEnum::VE_MoveSpeed:
		{
			UCharacterMovementComponent* moveComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerActor.Get());
			if (IsValid(moveComponent))
			{
				moveComponent->MaxWalkSpeed = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
			}
		}
		break;
		case ECharacterPropertyTypeEnum::VE_AttackSpeed:
		{
			UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
			if (EntityAIComponent)
			{
				EntityAIComponent->AddModifiedAnimSpeed(BonusValue);
			}
		}
		break;
	}	
}

void USkillSlot::AttachPersistentAbnormalApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach)
{
	int32 ResultCount = 0;
	if (OwnerBattleEntityComponent.IsValid())
		ResultCount = OwnerBattleEntityComponent->AttachAbnormalStateApplier(InSkillApplier.SkillApplierType, InIsAttach);

	switch (InSkillApplier.SkillApplierType)
	{
		case ESkillApplierTypeEnum::VE_p_captivate:
		{
			USpecialMovingComponent* SpecialMovingComponent = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(OwnerActor.Get());
			if (SpecialMovingComponent)
			{
				if (InIsAttach)
					SpecialMovingComponent->StartCaptivate(CasterActor.Get(), Duration);
				else
					SpecialMovingComponent->StopCaptivate();
			}
		}
		break;


		case ESkillApplierTypeEnum::VE_p_fear:
		{
			USpecialMovingComponent* SpecialMovingComponent = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(OwnerActor.Get());
			if (SpecialMovingComponent)
			{
				if (InIsAttach)
					SpecialMovingComponent->StartFear(CasterActor.Get(), Duration);
				else
					SpecialMovingComponent->StopFear();
			}
		}
		break;

		case ESkillApplierTypeEnum::VE_p_push:
		{
			if (!OwnerActor.IsValid() || !CasterActor.IsValid())
				return;

			USpecialMovingComponent* SpecialMovingComponent = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(OwnerActor.Get());
			if (SpecialMovingComponent)
			{
				FVector Dir = OwnerActor->GetActorLocation() - CasterActor->GetActorLocation();
				Dir.Z = 0;
				Dir = Dir.GetSafeNormal();

				UBattleBaseAnimInstance* BattleBaseAnim = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(OwnerActor.Get());
				if (BattleBaseAnim)
				{
					UAnimMontage* AbnormalMontage = BattleBaseAnim->GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
					if (AbnormalMontage)
					{
						int32 SectionIndex = AbnormalMontage->GetSectionIndex(ANIM_MONTAGE_ABNORMAL_PUSH);
						float Length = AbnormalMontage->GetSectionLength(SectionIndex);

						if (InIsAttach)
							SpecialMovingComponent->StartPush(Dir, Length);
						else
							SpecialMovingComponent->StopPush();
					}
				}
			}
		}
		break;

		case ESkillApplierTypeEnum::VE_p_knockback:
		{
			if (!OwnerActor.IsValid() || !CasterActor.IsValid())
				break;;

			USpecialMovingComponent* SpecialMovingComponent = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(OwnerActor.Get());
			if (SpecialMovingComponent)
			{
				FVector Dir = OwnerActor->GetActorLocation() - CasterActor->GetActorLocation();
				Dir.Z = 0;
				Dir = Dir.GetSafeNormal();

				UBattleBaseAnimInstance* BattleBaseAnim = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(OwnerActor.Get());
				if (BattleBaseAnim)
				{
					UAnimMontage* AbnormalMontage = BattleBaseAnim->GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
					if (AbnormalMontage)
					{
						int32 SectionIndex = AbnormalMontage->GetSectionIndex(ANIM_MONTAGE_ABNORMAL_KNOCKBACK);
						float Length = AbnormalMontage->GetSectionLength(SectionIndex);

						if (InIsAttach)
							SpecialMovingComponent->StartKnockBack(Dir, Length);
						else
							SpecialMovingComponent->StopKnockBack();
					}
				}
			}
		}
		break;

		case ESkillApplierTypeEnum::VE_p_hide:
		{
			// use slot
			if (ResultCount > 0)
			{
				// VE_Slot_Hide is set SetAbnormalState(true) in OpenSlot()
				if (EAbnormalSlotTypeEnum::VE_Slot_Hide != SlotType)
				{
					OwnerBattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_Hide, true);
				}
			}
			else
				OwnerBattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_Hide, false);
		}
		break;
	}
}

void USkillSlot::AttachAdditionalDamageProperty(FSkillApplierBase& InSkillApplier, bool InIsAttach)
{
	bool isBlessApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Counter_BEGIN, ESkillApplierTypeEnum::VE_p_Counter_MID1);
	bool isWeakApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Counter_MID1, ESkillApplierTypeEnum::VE_p_Counter_MID2);
	bool isProtectApplier = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Counter_MID2, ESkillApplierTypeEnum::VE_p_Counter_END);

	UBattleInfoBlackboard* BattleBB = OwnerBattleEntityComponent->GetBattleInfoBlackboard();
	if (IsValid(BattleBB))
	{
		if (isBlessApplier)
			BattleBB->SetBlessApplier(InIsAttach == true ? InSkillApplier : FSkillApplierBase());
		else if(isProtectApplier)
			BattleBB->SetProtectApplier(InIsAttach == true ? InSkillApplier : FSkillApplierBase());
		else if (isWeakApplier)
		{
			BattleBB->SetWeakApplier(InSkillApplier, InIsAttach);
		}
	}
}

void USkillSlot::AttachSpiritProperty(FSkillApplierBase& InSkillApplier, bool InIsAttach)
{
	bool isSpiritPlus = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Spirit_BEGIN, ESkillApplierTypeEnum::VE_p_Spirit_MID);
	bool isSpiritMinus = FMath::IsWithin(InSkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Spirit_MID, ESkillApplierTypeEnum::VE_p_Spirit_END);

	UBattleInfoBlackboard* BattleBB = OwnerBattleEntityComponent->GetBattleInfoBlackboard();
	if (isSpiritPlus)
	{
		BattleBB->SetSpiritPlusApplier(InSkillApplier, InIsAttach);
	}
	else if (isSpiritMinus)
	{
		BattleBB->SetSpiritMinusApplier(InSkillApplier, InIsAttach);
	}
}

void USkillSlot::AttachPersistentStateApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach)
{
	int32 ResultCount = 0;
	if (OwnerBattleEntityComponent.IsValid())
	{
		ResultCount = OwnerBattleEntityComponent->AttachAbnormalStateApplier(InSkillApplier.SkillApplierType, InIsAttach);
	}

	switch (InSkillApplier.SkillApplierType)
	{
		case ESkillApplierTypeEnum::VE_p_provoke:
		{
			UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
			if (EntityAIComponent)
			{
				if (InIsAttach)
					EntityAIComponent->SetBlackboardForceTarget(CasterActor.Get());
				else
					EntityAIComponent->SetBlackboardForceTarget(nullptr);
			}
		}
		break;

		case ESkillApplierTypeEnum::VE_p_modification:
		{
			UObjectFunctionLibrary::ModifyActor(OwnerActor.Get(), InIsAttach);
		}
		break;

		case ESkillApplierTypeEnum::VE_p_fear_self:
		{
			USpecialMovingComponent* SpecialMovingComponent = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(OwnerActor.Get());
			if (SpecialMovingComponent)
			{
				if (InIsAttach)
				{
					AActor* TargetActor = OwnerBattleEntityComponent->GetCurrentAttackTarget();
					if (TargetActor == nullptr)
					{
						UEntityAIComponent* OwnerAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
						if (OwnerAIComponent)
						{
							TargetActor = OwnerAIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
						}
					}
					if (TargetActor == OwnerActor.Get())
						return;

					if (TargetActor)
						SpecialMovingComponent->StartFear(TargetActor, Duration);
				}
				else
				{
					if (SpecialMovingComponent->GetSpecialMoveType() == ESpecialMoveTypeEnum::VE_Fear)
					{
						SpecialMovingComponent->StopFear();
					}
				}
			}
		}
		break;

			/*
			Protect
			*/
		case ESkillApplierTypeEnum::VE_p_damage_ice_shield:
		case ESkillApplierTypeEnum::VE_p_damage_shield:
		case ESkillApplierTypeEnum::VE_p_damage_shield_recovery:		
			{
				if (InIsAttach)
				{
					FProtectInfo ProtectInfo;
					ProtectInfo.ProtectType = EProtectType::VE_HP;
					ProtectInfo.SkillApplierType = InSkillApplier.SkillApplierType;

					int32 OwnerMaxHP = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
					int32 ShieldPower = ((float)InSkillApplier.TargetPercent * 0.01f) * (float)OwnerMaxHP;
					ProtectInfo.ProtectPower = InSkillApplier.TargetValue + ShieldPower;
					OwnerBattleEntityComponent->GetBattleOperator()->AttachProtect(ProtectInfo);
				}
				else
				{
					bool bIsTimeOut = false;
					if (InSkillApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_damage_shield_recovery)
					{
						if (ElapsedTime >= Duration)
						{
							float RemainedPower = OwnerBattleEntityComponent->GetBattleOperator()->GetProtectPower(EProtectType::VE_HP);
							//if (CasterSkillEntityComponent.IsValid())
							//{
							//	CasterSkillEntityComponent->CheckAccumulationSkillCost(SkillInst);
							//}
							OwnerBattleEntityComponent->ApplyFinalDamage(-RemainedPower, OwnerActor.Get(), EBattleDamageResultEnum::VE_Heal);
							bIsTimeOut = true;
						}
					}
					
					OwnerBattleEntityComponent->GetBattleOperator()->DetachProtect(EProtectType::VE_HP, bIsTimeOut);
				}
			}
		break;

		case ESkillApplierTypeEnum::VE_p_blindness: 
		{
			float OwnerPropertyValue = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterNativeTotalProperty<float>(ECharacterPropertyTypeEnum::VE_AttackRange);
			float BonusValue = (float)OwnerPropertyValue * ((float)InSkillApplier.TargetPercent / 100.0f) + InSkillApplier.TargetValue;
			if (!InIsAttach)
				BonusValue *= -1.0f;
			OwnerBattleEntityComponent->GetBattleInfo()->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_AttackRange, BonusValue);
		}
		break;

		case ESkillApplierTypeEnum::VE_p_timebomb:
		{
			if( !InIsAttach )
			{
				int32 OwnerMaxHP = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
				int32 PercentBonus = (float)OwnerMaxHP * (30.0f / 100.0f);
				PercentBonus = FMath::Min(PercentBonus, InSkillApplier.TargetValue);

				//if (CasterSkillEntityComponent.IsValid())
				//{
				//	CasterSkillEntityComponent->CheckAccumulationSkillCost(SkillInst);
				//}
				OwnerBattleEntityComponent->ApplyFinalDamage(PercentBonus, CasterActor.Get(), EBattleDamageResultEnum::VE_NoneHitDamage);
			}
		}
		break;

		case ESkillApplierTypeEnum::VE_p_timeHeal:
			if (!InIsAttach)
			{
				int32 OwnerMaxHP = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
				int32 PercentBonus = (float)OwnerMaxHP * (30.0f / 100.0f);
				PercentBonus = FMath::Min(PercentBonus, InSkillApplier.TargetValue);

				//if (CasterSkillEntityComponent.IsValid())
				//{
				//	CasterSkillEntityComponent->CheckAccumulationSkillCost(SkillInst);
				//}
				OwnerBattleEntityComponent->ApplyFinalDamage(-PercentBonus, CasterActor.Get(), EBattleDamageResultEnum::VE_Heal);
			}
			break;

		case ESkillApplierTypeEnum::VE_p_vamp_attack:
			if( InIsAttach )
				OwnerBattleEntityComponent->GetBattleInfoBlackboard()->VampAttackApplier += InSkillApplier;
			else
				OwnerBattleEntityComponent->GetBattleInfoBlackboard()->VampAttackApplier -= InSkillApplier;
			break;


		case ESkillApplierTypeEnum::VE_p_reflect_damage:
		case ESkillApplierTypeEnum::VE_p_reflect_damage_All:
			if( InIsAttach )
				OwnerBattleEntityComponent->GetBattleInfoBlackboard()->ReflectionDamageApplier += InSkillApplier;
			else
				OwnerBattleEntityComponent->GetBattleInfoBlackboard()->ReflectionDamageApplier -= InSkillApplier;
			break;

		case ESkillApplierTypeEnum::VE_p_self_despawn:
			{
				if (!InIsAttach)
					OwnerBattleEntityComponent->ForceDead(nullptr);
			}
			break;
	}	
}

bool USkillSlot::AttachContinousApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach )
{
	if (InIsAttach)
	{
		switch (InSkillApplier.SkillApplierType)
		{
			case ESkillApplierTypeEnum::VE_c_hp_group_fire:
			{
				USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
				if (SkillEntityComponent)
				{
					if (SkillEntityComponent->GetSkillSlotContainer()->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Fire))
					{
						SkillEntityComponent->ClearSlotsByGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Fire, 100);
					}
				}
			}
			// *****
			// Skil break;
			// continue case... group fire should save caster magic attack power
			// *****			
			case ESkillApplierTypeEnum::VE_c_hp_dot:
				// save magic attack power
				if (CasterBattleEntityComponent.IsValid())
				{
					InSkillApplier.CasterMagicPower = CasterBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MagicAttackPower)
						+ CasterBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower);
					InSkillApplier.CasterMagicPower = InSkillApplier.CasterMagicPower * (InSkillApplier.TargetPercent / 100.0f);
				}
				break;

			case ESkillApplierTypeEnum::VE_c_hp_hot:
				// save magic attack power
				if (CasterBattleEntityComponent.IsValid())
				{
					InSkillApplier.CasterMagicPower = CasterBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MagicAttackPower);
					InSkillApplier.CasterMagicPower = InSkillApplier.CasterMagicPower * (InSkillApplier.TargetPercent / 100.0f);
				}
				break;
		}
			
		ApplyContinousApplier(InSkillApplier, false);
	}
	
	return true;
}

bool USkillSlot::AttachConditionalApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach)
{
	int32 ResultCount = 0;
	if (OwnerBattleEntityComponent.IsValid())
	{
		ResultCount = OwnerBattleEntityComponent->AttachAbnormalStateApplier(InSkillApplier.SkillApplierType, InIsAttach);
	}

	switch (InSkillApplier.SkillApplierType)
	{
		case ESkillApplierTypeEnum::VE_t_attack_pile_up:
		{
			OwnerBattleEntityComponent->GetBattleInfoBlackboard()->SetAttachedPileupSlot(true, InSkillApplier.TargetValue, InSkillApplier.TargetPercent);
		}
		break;

		case ESkillApplierTypeEnum::VE_t_guts:
		{
			USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
			if (SkillEntityComponent)
			{
				auto PassiveSkillSlotCon = SkillEntityComponent->GetPassiveSkillSlotContainer();
				if (PassiveSkillSlotCon)
				{
					PassiveSkillSlotCon->AddOneTimeValidApplier(ESkillApplierTypeEnum::VE_t_guts, this);
				}
			}
		}
		break;

		default:
			return false;
	}

	return true;
}

void USkillSlot::ApplyContinousApplier(FSkillApplierBase& InSkillApplier, bool Updated)
{
	switch (InSkillApplier.SkillApplierType)
	{		
		case ESkillApplierTypeEnum::VE_c_hp_hot:
			{
				int32 TotalValue = InSkillApplier.CasterMagicPower + InSkillApplier.TargetValue;
				if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_heal_bonus))
				{
					USkillEntityComponent* OwnerSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
					if (OwnerSkillEntityComponent)
					{
						bool ApplyBonus = false;
						float ApplierPercent = 0.0f;
						float ApplierValue = 0.0f;
						FSkillApplierBase OutSkillApplier;
						if (OwnerSkillEntityComponent->GetSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
						{
							ApplierPercent = OutSkillApplier.TargetPercent;
							ApplierValue = OutSkillApplier.TargetValue;
							ApplyBonus = true;
						}

						if (OwnerSkillEntityComponent->GetPassiveSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
						{
							ApplierPercent += OutSkillApplier.TargetPercent;
							ApplierValue += OutSkillApplier.TargetValue;
							ApplyBonus = true;
						}
						
						if (ApplyBonus)
						{
							TotalValue = (TotalValue + (TotalValue * (ApplierPercent / 100.0f))) + ApplierValue;
						}
					}
				}

				TotalValue = TotalValue + (int32)((float)TotalValue * OwnerBattleEntityComponent->GetBattleInfo()->GetAvatarAbilityProperty(EAvatarAbilityType::VE_ReceivedHeal) / 100.0f);
				TotalValue = FMath::Max(TotalValue, 0);
				//if (CasterSkillEntityComponent.IsValid())
				//{
				//	CasterSkillEntityComponent->CheckAccumulationSkillCost(SkillInst);
				//}
				OwnerBattleEntityComponent->ApplyFinalDamage(-TotalValue, CasterActor.Get(), EBattleDamageResultEnum::VE_Heal);
				OwnerBattleEntityComponent->NotifyReceiveHeal(TotalValue, CasterActor.Get());
			}
			break;
		case ESkillApplierTypeEnum::VE_c_hp_dot:
		case ESkillApplierTypeEnum::VE_c_hp_group_fire:
			{	
				int32 TotalValue = InSkillApplier.CasterMagicPower + InSkillApplier.TargetValue;
				//if (CasterSkillEntityComponent.IsValid())
				//{
				//	CasterSkillEntityComponent->CheckAccumulationSkillCost(SkillInst);
				//}
				OwnerBattleEntityComponent->ApplyFinalDamage(TotalValue, CasterActor.Get(), EBattleDamageResultEnum::VE_NoneHitDamage);					
				OwnerBattleEntityComponent->OnNotifyDamageDelegate.Broadcast(CasterActor.Get(), OwnerActor.Get(), TotalValue, true);
			}
			break;		
		case ESkillApplierTypeEnum::VE_c_hp_percent:
			{
				int32 OwnerMaxHP = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
				int32 PercentBonus = (float)OwnerMaxHP * ((float)InSkillApplier.TargetPercent / 100.0f);			
				if (PercentBonus > 0)
				{
					int32 TotalValue = PercentBonus;
					if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_heal_bonus))
					{
						USkillEntityComponent* OwnerSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
						if (OwnerSkillEntityComponent)
						{
							bool ApplyBonus = false;
							float ApplierPercent = 0.0f;
							float ApplierValue = 0.0f;
							FSkillApplierBase OutSkillApplier;
							if (OwnerSkillEntityComponent->GetSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
							{
								ApplierPercent = OutSkillApplier.TargetPercent;
								ApplierValue = OutSkillApplier.TargetValue;
								ApplyBonus = true;
							}

							if (OwnerSkillEntityComponent->GetPassiveSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
							{
								ApplierPercent += OutSkillApplier.TargetPercent;
								ApplierValue += OutSkillApplier.TargetValue;
								ApplyBonus = true;
							}

							if (ApplyBonus)
							{
								TotalValue = TotalValue + TotalValue * ((1.0f + ApplierPercent) / 100.0f) + ApplierValue;
							}
						}
					}

					TotalValue = TotalValue + (int32)((float)TotalValue * OwnerBattleEntityComponent->GetBattleInfo()->GetAvatarAbilityProperty(EAvatarAbilityType::VE_ReceivedHeal) / 100.0f);
					TotalValue = FMath::Min(TotalValue, InSkillApplier.TargetValue);
					//if (CasterSkillEntityComponent.IsValid())
					//{
					//	CasterSkillEntityComponent->CheckAccumulationSkillCost(SkillInst);
					//}
					OwnerBattleEntityComponent->ApplyFinalDamage(-TotalValue, CasterActor.Get(), EBattleDamageResultEnum::VE_Heal);					
					OwnerBattleEntityComponent->NotifyReceiveHeal(TotalValue, CasterActor.Get());
				}
				else
				{
					int32 TotalValue = FMath::Min(-PercentBonus, InSkillApplier.TargetValue);
					//if (CasterSkillEntityComponent.IsValid())
					//{
					//	CasterSkillEntityComponent->CheckAccumulationSkillCost(SkillInst);
					//}
					OwnerBattleEntityComponent->ApplyFinalDamage(TotalValue, CasterActor.Get(), EBattleDamageResultEnum::VE_NoneHitDamage);					
					OwnerBattleEntityComponent->OnNotifyDamageDelegate.Broadcast(CasterActor.Get(), OwnerActor.Get(), TotalValue, true);
				}
			}

			break;
	}	
}


FSkillApplierBase& USkillSlot::GetPersistentApplier(ESkillApplierTypeEnum InApplierType)
{
	for (FSkillApplierBase& SkillApplier : SkillApplierList)
	{
		if (SkillApplier.SkillApplierType == InApplierType)
			return SkillApplier;
	}	

	static FSkillApplierBase ZeroInfo;
	return ZeroInfo;
}

bool USkillSlot::AccumulateApplier(ESkillApplierTypeEnum InApplierType, FSkillApplierBase& OutSkillApplied)
{
	bool bResult = false;
	for (FSkillApplierBase& SkillApplier : SkillApplierList)
	{
		if (SkillApplier.SkillApplierType == InApplierType)
		{
			OutSkillApplied += SkillApplier;
			bResult = true;
		}
	}	

	return bResult;
}

void USkillSlot::CheckContinuousApplier( float DeltaTime )
{
	IntervalElapsedTime += DeltaTime;
	if (IntervalElapsedTime >= IntervalTime)
	{
		IntervalElapsedTime = 0.0f;

		for (FSkillApplierBase& SkillAppilerInfo : SkillApplierList)
		{
			ApplyContinousApplier(SkillAppilerInfo);
		}
	}
}


FSkillApplierBase* USkillSlot::ContainApplier(ESkillApplierTypeEnum InApplierType)
{
// 	if (SkillApplierList.Num() <= 0)
// 		return nullptr;
// 	for (int32 i = 0; i < SkillApplierList.Num(); ++i)
// 	{
// 		if (SkillApplierList.IsValidIndex(i))
// 		{
// 			if (SkillApplierList[i].SkillApplierType == InApplierType)
// 				return &SkillApplierList[i];
// 		}
// 	}
	if (SkillApplierList.GetAllocatedSize() <= 0)
		return nullptr;

	for (FSkillApplierBase& SkillApplier : SkillApplierList)
	{
		if (SkillApplier.SkillApplierType == InApplierType)
			return (&SkillApplier);
	}

	return nullptr;
}


void USkillSlot::AttachSlotEffect()
{	
	FName SlotID = UUtilFunctionLibrary::GetEnumToName(TEXT("EAbnormalSlotTypeEnum"), (int32)SlotType);
	FAbnormalStateGroupTableInfo_V2* SlotTableInfo = UTableManager::GetInstancePtr()->GetAbnormalStateGroupRow_V2(SlotID);
	if (SlotTableInfo)
	{
		if (SlotEffect.IsValid())
			DetachSlotEffect();
		SlotEffect = UUtilFunctionLibrary::SpawnEmitterAttached( UUtilFunctionLibrary::GetParticleSystemFromAssetID(SlotTableInfo->Effect), OwnerActor.Get(), SlotTableInfo->Socket);
		if (SlotEffect.IsValid() && OwnerBattleEntityComponent.IsValid())
		{
			float Radius = OwnerBattleEntityComponent->GetBoundingRadius() / RADIUSSIZE_DEFAULT;
			if (SlotTableInfo->Socket.Compare(SOCKET_STATE) == 0)
			{
				if (Radius > 2)
					Radius = 2;
			}
			FVector EffectScale = FVector(Radius, Radius, Radius);
			SlotEffect->SetWorldScale3D(EffectScale);
		}

		// Custom material change
		if (SlotTableInfo->EffectMaterial.GetLongPackageName().IsEmpty() == false )
		{
			UCustomChangeMaterialComponent* Comp = UCharacterHelpFunctionLibrary::GetCustomMaterialComponent(OwnerActor.Get());
			if (IsValid(Comp))
			{
				Comp->Active(true, UUtilFunctionLibrary::GetMaterialFromAssetID(SlotTableInfo->EffectMaterial) );
			}
		}

		// Status color
		if (SlotTableInfo->StatusColor.DWColor() != FColor::Black.DWColor())
		{
			UEntityRenderComponent* RenderEntityComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get() );
			if (RenderEntityComponent)
				RenderEntityComponent->SetMaterialColor(SlotTableInfo->StatusColor);
		}
	}	
}

void USkillSlot::DetachSlotEffect()
{	
	if (SlotEffect.IsValid())
	{
#if LOG_SKILL
		UE_LOG(LogBattle, Log, TEXT("<<< Detach Slot Effect - %d"), (int32)SlotType);
#endif
		if (false == bForceDetach)
		{
			if (OwnerBattleEntityComponent.IsValid() && OwnerBattleEntityComponent->IsDead())
			{
				SlotEffect->DestroyComponent();
			}
			else
			{
				SlotEffect->Deactivate();
			}
		}
		else
		{
			SlotEffect->ActivateSystem(false);
			SlotEffect->DeactivateSystem();
			SlotEffect->KillParticlesForced();

			SlotEffect->DestroyComponent();
		}

		SlotEffect = nullptr;
	}

	SlotEffect = nullptr;

	FName SlotID = UUtilFunctionLibrary::GetEnumToName(TEXT("EAbnormalSlotTypeEnum"), (int32)SlotType);
	FAbnormalStateGroupTableInfo_V2* SlotTableInfo = UTableManager::GetInstancePtr()->GetAbnormalStateGroupRow_V2(SlotID);
	if (SlotTableInfo)
	{
		if (SlotTableInfo->EffectMaterial.GetLongPackageName().IsEmpty() == false)
		{
			UCustomChangeMaterialComponent* Comp = UCharacterHelpFunctionLibrary::GetCustomMaterialComponent(OwnerActor.Get());
			if (IsValid(Comp))
			{
				Comp->Active(false, nullptr );
			}
		}

		// Status color
		if (SlotTableInfo->StatusColor != FColor::Black)
		{
			UEntityRenderComponent* RenderEntityComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
			if (RenderEntityComponent)
				RenderEntityComponent->SetMaterialColor(FColor::Black);
		}
	}
}

void USkillSlot::SetAbnormalState(bool bAttach)
{
	if (OwnerBattleEntityComponent.IsValid())
	{
		/*2018-08-22*/
		/*AbnormalState is decided by Applier not Slot! */
		//EAbonormalStateFlagEnum StateFlag = URenewal_BattleFunctionLibrary::GetStateFlagBySlotType(SlotType);
		EAbonormalStateFlagEnum StateFlag = EAbonormalStateFlagEnum::BM_None;
		if (!OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_state_change))
		{
			for (auto& Element : SkillApplierList)
			{
				switch (Element.SkillApplierType)
				{
				case ESkillApplierTypeEnum::VE_p_stun: StateFlag = EAbonormalStateFlagEnum::BM_Stun; break;
				case ESkillApplierTypeEnum::VE_p_down: StateFlag = EAbonormalStateFlagEnum::BM_Down; break;
				case ESkillApplierTypeEnum::VE_p_frozen: StateFlag = EAbonormalStateFlagEnum::BM_Frozen; break;
				case ESkillApplierTypeEnum::VE_p_silence: StateFlag = EAbonormalStateFlagEnum::BM_Silence; break;
				case ESkillApplierTypeEnum::VE_p_hold: StateFlag = EAbonormalStateFlagEnum::BM_Hold; break;
				case ESkillApplierTypeEnum::VE_p_hide: StateFlag = EAbonormalStateFlagEnum::BM_Hide; break;
				case ESkillApplierTypeEnum::VE_p_fear: StateFlag = EAbonormalStateFlagEnum::BM_Fear; break;
				case ESkillApplierTypeEnum::VE_p_airborne: StateFlag = EAbonormalStateFlagEnum::BM_Airborne; break;
				case ESkillApplierTypeEnum::VE_p_captivate: StateFlag = EAbonormalStateFlagEnum::BM_Captivate; break;
				case ESkillApplierTypeEnum::VE_p_push: StateFlag = EAbonormalStateFlagEnum::BM_Push; break;
				case ESkillApplierTypeEnum::VE_p_knockback: StateFlag = EAbonormalStateFlagEnum::BM_KnockBack; break;
				}
			}
		}

		OwnerBattleEntityComponent->SetAbnormalState(StateFlag, bAttach);

		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (SkillEntityComponent)
		{
			SkillEntityComponent->OnSetAbnormalIcon.Broadcast(SlotType, bAttach);

			// Check PassiveSkillCondition setting by Group
			ESkillApplierTypeEnum ConditionType = ESkillApplierTypeEnum::VE_None;
			switch (GroupType)
			{
			case EAbnormalGroupTypeEnum::VE_Group_DOT_Poison:	ConditionType = ESkillApplierTypeEnum::VE_t_caster_group_poison;	break;
			case EAbnormalGroupTypeEnum::VE_Group_DOT_Bleeding:	ConditionType = ESkillApplierTypeEnum::VE_t_caster_group_bleeding;break;
			case EAbnormalGroupTypeEnum::VE_Group_DOT_Fire:		ConditionType = ESkillApplierTypeEnum::VE_t_caster_group_fire;	break;
			case EAbnormalGroupTypeEnum::VE_Group_DOT_Ignite:	ConditionType = ESkillApplierTypeEnum::VE_t_caster_group_ignite;	break;
			//case EAbnormalGroupTypeEnum::VE_Group_Mark_Weapon:	ConditionType = ESkillApplierTypeEnum::VE_p_condition_caster_group_mark;	break;
			}
			if (ESkillApplierTypeEnum::VE_None != ConditionType)
			{
				SkillEntityComponent->ReservePassiveSkillCondition(ConditionType);
			}

			// Check PassiveSkillCondition setting by Slot
			ConditionType = ESkillApplierTypeEnum::VE_None;
			switch (SlotType)
			{
			case EAbnormalSlotTypeEnum::VE_Slot_Stun:			ConditionType = ESkillApplierTypeEnum::VE_t_caster_slot_stun;		break;
			case EAbnormalSlotTypeEnum::VE_Slot_Down:			ConditionType = ESkillApplierTypeEnum::VE_t_caster_slot_down;		break;
			case EAbnormalSlotTypeEnum::VE_Slot_Fear:			ConditionType = ESkillApplierTypeEnum::VE_t_caster_slot_fear;		break;
			case EAbnormalSlotTypeEnum::VE_Slot_Freezing:		ConditionType = ESkillApplierTypeEnum::VE_t_caster_slot_freezing;	break;
			case EAbnormalSlotTypeEnum::VE_Slot_Hold:			ConditionType = ESkillApplierTypeEnum::VE_t_caster_slot_hold;		break;
			case EAbnormalSlotTypeEnum::VE_Slot_Ice:			ConditionType = ESkillApplierTypeEnum::VE_t_caster_slot_ice;		break;
			case EAbnormalSlotTypeEnum::VE_Slot_Blindness:		ConditionType = ESkillApplierTypeEnum::VE_p_blindness;						break;
			}
			if (ESkillApplierTypeEnum::VE_None != ConditionType)
			{
				SkillEntityComponent->ReservePassiveSkillCondition(ConditionType);
			}
		}
	}
}


/*
For debugging
*/
float USkillSlot::GetDuration()
{
	return Duration;
}

float USkillSlot::GetDurationElapsedTime()
{
	return ElapsedTime;
}
