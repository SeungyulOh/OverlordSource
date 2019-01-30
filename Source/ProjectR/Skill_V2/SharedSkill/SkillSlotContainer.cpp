// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillSlotContainer.h"
#include "GlobalIntegrated.h"	


#include "Table/SkillTableInfo_V2.h"
#include "ISkill_V2.h"


#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SharedSkill/Skill_Passive_V2.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "../RemoteSkillSlot.h"


USkillSlotContainer::USkillSlotContainer()
{
}

void USkillSlotContainer::Init(AActor* InOwner)
{
	OwnerActor = InOwner;

	bRealTimeMode = URealTimeModManager::IsRealTimeGameMode();

	PrepareSlots();
}

void USkillSlotContainer::PrepareSlots()
{
	int32 MaxIndex = (int32)EAbnormalSlotTypeEnum::VE_Slot_Active_Max;
	SkillSlotList.Init(nullptr, MaxIndex);

	for (int32 n = 0; n < MaxIndex; ++n)
	{
		if (false == bRealTimeMode)
		{
			SkillSlotList[n] = NewObject<USkillSlot>();
		}
		else
		{
			SkillSlotList[n] = NewObject<URemoteSkillSlot>();
		}

		SkillSlotList[n]->SetSlotType((EAbnormalSlotTypeEnum)n);
	}
}

void USkillSlotContainer::Update(float DeltaTime)
{
	for (auto& SkillSlot : SkillSlotList)
	{
		if ( SkillSlot && SkillSlot->IsActivated())
		{
			SkillSlot->Update(DeltaTime);
		}
	}
}


/*
Passive skill don't have AppliedSkill instance.
*/
bool USkillSlotContainer::CanAddSkillSlot(FSkillTableInfo_V2* SkillInfo, bool bCasterSlot)
{
	checkf(SkillInfo != nullptr, TEXT("USkillSlotContainer::CanAddSkillSlot[SkillInfo Is Empty]"));
// 	if (nullptr == SkillInfo)
// 		return false;

	int32 slotIndex = (int32)SkillInfo->TargetSlotType;
	int32 AddSlotLevel = SkillInfo->TargetSlotLevel;
	if (bCasterSlot)
	{
		slotIndex = (int32)SkillInfo->CasterSlotType;
		AddSlotLevel = SkillInfo->CasterSlotLevel;
	}

	if (SkillSlotList.IsValidIndex(slotIndex))
	{
		if (SkillSlotList[slotIndex]->IsActivated())
		{
			if (SkillSlotList[slotIndex]->GetSlotLevel() > AddSlotLevel)
			{
				return false;
			}
			else
			{
				SkillSlotList[slotIndex]->CloseSlot();
			}
		}
		return true;
	}
	return false;
}

void USkillSlotContainer::AddSkillSlot(UISkill_V2* AppliedSkill, bool InIsCasterSlot, AActor* InCaster)
{
	checkf(InCaster != nullptr, TEXT("USkillSlotContainer::AddSkillSlot[InCaster Is Empty]"));
	checkf(AppliedSkill != nullptr, TEXT("USkillSlotContainer::AddSkillSlot[AppliedSkill Is Empty]"));
// 	if (false == IsValid(InCaster))
// 		return;
// 
// 	if (nullptr == AppliedSkill)
// 		return;

	if (false == bRealTimeMode)
	{
		switch (AppliedSkill->eSkillType)
		{
		case ESkillType::eSkillType_Normal:
		{
			FSkillTableInfo_V2 * SkillTableInfo = AppliedSkill->GetSkillInfo();
			SkillTable(SkillTableInfo, AppliedSkill, InIsCasterSlot, InCaster);
		}
		break;
		}
	}
	else
	{
		AddSkillSlotForRealTime(AppliedSkill, InIsCasterSlot, InCaster);
	}
}


void USkillSlotContainer::SkillTable(FSkillTableInfo_V2* SkillTableInfo, UISkill_V2* AppliedSkill, bool InIsCasterSlot, AActor* InCaster)
{
	checkf(InCaster != nullptr, TEXT("USkillSlotContainer::SkillTable[InCaster Is Empty]"));
	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InCaster);
	if (!IsValid(SkillComponent))
		return;

	int32 slotIndex = (int32)SkillTableInfo->TargetSlotType;
	if (InIsCasterSlot)
		slotIndex = (int32)SkillTableInfo->CasterSlotType;

	// none slot instance skill
	if (slotIndex == 0)
	{
		SkillComponent->ApplyInstanceApplierGroup(InCaster, OwnerActor.Get(), SkillTableInfo, InIsCasterSlot, AppliedSkill);
	}
	else
	{
		bool bSuccessApply = false;

		if (CanAddSkillSlot(SkillTableInfo, InIsCasterSlot))
		{
			if (CanOpenSlot(SkillTableInfo, InIsCasterSlot))
			{
				SkillSlotList[slotIndex]->OpenSlot(AppliedSkill->GetSkillTableID(), InIsCasterSlot, OwnerActor.Get(), InCaster, AppliedSkill);
				bSuccessApply = true;
			}
		}

 		if (false == bSuccessApply)
 			SkillComponent->ApplyInstanceApplierGroup(InCaster, OwnerActor.Get(), SkillTableInfo, InIsCasterSlot, AppliedSkill);
	}
}

bool USkillSlotContainer::CanOpenSlot(FSkillTableInfo_V2* InSkillTableInfo, bool InIsCaster)
{
	checkf(InSkillTableInfo != nullptr, TEXT("USkillSlotContainer::CanOpenSlot[InSkillTableInfo Is Empty]"));
// 	if (nullptr == InSkillTableInfo)
// 		return false;

	EAbnormalSlotTypeEnum SlotType = InSkillTableInfo->TargetSlotType;

	if (EAbnormalSlotTypeEnum::VE_None == SlotType)
		return true;

	if (IsInBlockSlot(SlotType))
		return false;

	if (false == CanAttachApplier(InSkillTableInfo, InIsCaster))
		return false;

	/*
	check group VE_Group_Helplessness
	*/
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return false;

	if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_Helplessness, SlotType))
	{
		RemoveAllSlotsByGroup( EAbnormalGroupTypeEnum::VE_Group_Helplessness, 10000 );
	}

	return true;
}

bool USkillSlotContainer::IsInBlockSlot(EAbnormalSlotTypeEnum SlotType)
{
	if (EAbnormalSlotTypeEnum::VE_Slot_Passive <= SlotType)
		return false;

	auto OwnerBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());

	if (OwnerBattleEntityComponent->IsInBlockBuffType(SlotType))
		return true;

	if (OwnerBattleEntityComponent->IsInBlockGroupType(SlotType))
		return true;

	if (OwnerBattleEntityComponent->IsInBlockSlotType(SlotType))
		return true;

	if (OwnerBattleEntityComponent->IsInExceptBlockGroupType(SlotType))
		return true;

	if (OwnerBattleEntityComponent->IsInExceptBlockSlotType(SlotType))
		return true;

	if (OwnerActor.IsValid())
	{
		UBossBaseComponent* BossBaseComponent = UCharacterHelpFunctionLibrary::GetCharacterBossBaseComponent(OwnerActor.Get());
		if (IsValid(BossBaseComponent))
		{
			if (false == BossBaseComponent->CanAttachSlot(SlotType))
				return true;
		}
	}
	return false;
}

bool USkillSlotContainer::CanAttachApplier(FSkillTableInfo_V2* SkillTableInfo, bool IsCasterSlot)
{
	checkf(SkillTableInfo != nullptr, TEXT("USkillSlotContainer::CanAttachApplier[SkillTableInfo Is Empty]"));
// 	if (nullptr == SkillTableInfo)
// 		return false;

	auto OwnerBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (nullptr == OwnerBattleEntityComponent)
		return false;

	ESkillApplierTypeEnum ApplierTypes[6];
	if (IsCasterSlot)
	{
		ApplierTypes[0] = SkillTableInfo->CasterEffect1;
		ApplierTypes[1] = SkillTableInfo->CasterEffect2;
		ApplierTypes[2] = SkillTableInfo->CasterEffect3;
		ApplierTypes[3] = SkillTableInfo->CasterEffect4;
		ApplierTypes[4] = SkillTableInfo->CasterEffect5;
		ApplierTypes[5] = SkillTableInfo->CasterEffect6;
	}
	else
	{
		ApplierTypes[0] = SkillTableInfo->TargetEffect1;
		ApplierTypes[1] = SkillTableInfo->TargetEffect2;
		ApplierTypes[2] = SkillTableInfo->TargetEffect3;
		ApplierTypes[3] = SkillTableInfo->TargetEffect4;
		ApplierTypes[4] = SkillTableInfo->TargetEffect5;
		ApplierTypes[5] = SkillTableInfo->TargetEffect6;
	}

	for (int32 n = 0; n < 6; ++n)
	{
		if (ESkillApplierTypeEnum::VE_None == ApplierTypes[n])
			continue;

		switch (ApplierTypes[n])
		{
		case ESkillApplierTypeEnum::VE_c_hp_group_fire:
		{
			if (false == IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Fire) )
			{
				return false;
			}
		}
		break;

		/*case ESkillApplierTypeEnum::VE_p_block_action_slot_ice:
		{
			if (false == IsInSlot(EAbnormalSlotTypeEnum::VE_Slot_Ice))
			{
				return false;
			}
		}
		break;*/

		case ESkillApplierTypeEnum::VE_p_provoke:
		{
			if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_provoke))
			{
				OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_provoke);
				return false;
			}
		}
		break;
		case ESkillApplierTypeEnum::VE_p_captivate:
		{
			if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_captivate))
			{
				OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_captivate);
				return false;
			}
		}
		break;
		case ESkillApplierTypeEnum::VE_p_stat_up_ModelScale:
		{
			if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_stat_up_ModelScale))
			{
				OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_stat_up_ModelScale);
				return false;
			}
		}
		break;
		}
	}

	return true;
}

USkillSlot* USkillSlotContainer::GetSlotThatContainApplier(ESkillApplierTypeEnum InApplierType)
{
	for (auto SkillSlot : SkillSlotList)
	{
		if (SkillSlot->IsActivated())
		{
			FSkillApplierBase* Applier = SkillSlot->ContainApplier(InApplierType);
			if (Applier)
				return SkillSlot;
		}
	}

	return nullptr;
}


TArray< USkillSlot* >& USkillSlotContainer::GetAllSlots()
{
	return SkillSlotList;
}

bool USkillSlotContainer::GatherPersistentAppilerValue(ESkillApplierTypeEnum InSkillApplierType, FSkillApplierBase& OutSkillApplier )
{	
	bool bResult = false;

	for (auto SkilSlot : SkillSlotList)
	{		
		if (SkilSlot->IsActivated())
		{
			if (SkilSlot->AccumulateApplier(InSkillApplierType, OutSkillApplier))
				bResult = true;
		}
	}

	return bResult;
}

bool USkillSlotContainer::IsInSlot(EAbnormalSlotTypeEnum InSlotType)
{
	int32 slotIndex = (int32)InSlotType;
	if (SkillSlotList.IsValidIndex(slotIndex))
		return SkillSlotList[slotIndex]->IsActivated();

	return false;
}

bool USkillSlotContainer::IsInGroup(EAbnormalGroupTypeEnum InGroupType)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("USkillSlotContainer::IsInGroup[RGameInstance Is Empty]"));
// 	if (RGameInstance == nullptr)
// 		return false;

	for (auto SkilSlot : SkillSlotList)
	{
		if (SkilSlot->IsActivated())
		{
			if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(InGroupType, SkilSlot->GetSlotType()))
				return true;
		}
	}

	return false;
}

bool USkillSlotContainer::IsInBuff(EAbnormalApplyTypeEnum InBuffType)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("USkillSlotContainer::IsInBuff[RGameInstance Is Empty]"));
// 	if (RGameInstance == nullptr)
// 		return false;

	for (auto SkilSlot : SkillSlotList)
	{
		if (SkilSlot->IsActivated())
		{
			if (InBuffType == EAbnormalApplyTypeEnum::VE_Buff)
			{
				if (RGameInstance->AbnormalStateGroupTable_V2->IsInBuff(SkilSlot->GetSlotType()))
					return true;
			}
			else
			{				
				if (!RGameInstance->AbnormalStateGroupTable_V2->IsInBuff(SkilSlot->GetSlotType()))
					return true;
			}

		}
	}
	return false;
}

void USkillSlotContainer::RemoveAllSlotsByBuffType(EAbnormalApplyTypeEnum InBuffType, int32 InSlotLevel )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("USkillSlotContainer::RemoveAllSlotsByBuffType[RGameInstance Is Empty]"));
// 	if (RGameInstance == nullptr)
// 		return;

	for (auto SkillSlot : SkillSlotList)
	{
		if (SkillSlot->IsActivated())
		{
			if (InBuffType == EAbnormalApplyTypeEnum::VE_Buff)
			{
				if (RGameInstance->AbnormalStateGroupTable_V2->IsInBuff(SkillSlot->GetSlotType()))
				{
					if(SkillSlot->GetSlotLevel() <= InSlotLevel )
						SkillSlot->CloseSlot();
				}
			}
			else
			{
				if (!RGameInstance->AbnormalStateGroupTable_V2->IsInBuff(SkillSlot->GetSlotType()))
				{
					if (SkillSlot->GetSlotLevel() <= InSlotLevel)
						SkillSlot->CloseSlot();
				}
			}
		}
	}
}

void USkillSlotContainer::RemoveAllSlotsByGroup(EAbnormalGroupTypeEnum InGroupType, int32 InSlotLevel)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("USkillSlotContainer::RemoveAllSlotsByGroup[RGameInstance Is Empty]"));
// 	if (RGameInstance == nullptr)
// 		return;

	for (auto SkillSlot : SkillSlotList)
	{
		if (SkillSlot->IsActivated())
		{
			if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup( InGroupType, SkillSlot->GetSlotType()))
			{
				if (SkillSlot->GetSlotLevel() <= InSlotLevel)
					SkillSlot->CloseSlot();
			}
		}
	}
}

void USkillSlotContainer::RemoveSlot(EAbnormalSlotTypeEnum InSlotType)
{
	for (auto SkilSlot : SkillSlotList)
	{
		if (SkilSlot->IsActivated())
		{
			if (SkilSlot->GetSlotType() == InSlotType)
			{
				SkilSlot->CloseSlot();
			}
		}
	}
}

void USkillSlotContainer::RemoveSlotbyResourceID(FName InID)
{
	for (auto SkilSlot : SkillSlotList)
	{
		if (SkilSlot->IsActivated())
		{
			if (SkilSlot->GetSkillResourceID() == InID)
			{
				SkilSlot->CloseSlot();
			}
		}
	}
}

void USkillSlotContainer::RemoveAllSlots()
{
	for (auto SkilSlot : SkillSlotList)
	{
		if (SkilSlot->IsActivated())
		{
			SkilSlot->SetForceDetach();
			SkilSlot->CloseSlot();
		}
	}
}

void USkillSlotContainer::AddSkillSlotForRealTime(UISkill_V2* AppliedSkill, bool InIsCasterSlot, AActor* InCaster)
{
	checkf(AppliedSkill != nullptr, TEXT("USkillSlotContainer::AddSkillSlotForRealTime[AppliedSkill Is Empty]"));

	if (false == AppliedSkill->SkillSyncData.IsValid())
		return;

	FSkillSyncApplyData* SyncData = nullptr;
	if (InIsCasterSlot)	SyncData = &(AppliedSkill->SkillSyncData.CasterApplyData);
	else				SyncData = AppliedSkill->GetTargetSkillSyncData(OwnerActor.Get());

	if (!SyncData)
		return;

	FSkillTableInfo_V2* SkillTableInfo = AppliedSkill->GetSkillInfo();
	if (nullptr == SkillTableInfo)
		return;

	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InCaster);
	if (!IsValid(SkillComponent))
		return;
	int32 slotCount = SyncData->ApplySlots.Num();
	if (0 < slotCount)
	{
		int32 SyncSlotIndex = -1;
		if (InIsCasterSlot)	SyncSlotIndex = SyncData->GetApplySlot(SkillTableInfo->CasterSlotType);
		else				SyncSlotIndex = SyncData->GetApplySlot(SkillTableInfo->TargetSlotType);

		if (0 <= SyncSlotIndex)
		{
			int32 SlotType = (int32)SyncData->ApplySlots[SyncSlotIndex];
			if (SlotType == 0)
			{
				SkillComponent->ApplyInstanceApplierGroup(InCaster, OwnerActor.Get(), SkillTableInfo, InIsCasterSlot, AppliedSkill);
			}
			else if (0 < SlotType)
			{
				SkillSlotList[SlotType]->SetSkillSyncData(SyncData);

				if (false == SyncData->RemoveSlots[SyncSlotIndex])
				{
					SkillSlotList[SlotType]->OpenSlot(AppliedSkill->GetSkillTableID(), InIsCasterSlot, OwnerActor.Get(), InCaster, AppliedSkill);
				}
			}
		}

		for (int32 n = 0; n < slotCount; ++n)
		{
			if (!SyncData->Target.IsValid())
				break;

			if (SyncData->RemoveSlots.IsValidIndex(n) && SyncData->RemoveSlots[n])
			{
				int32 Targetidx = INDEX_NONE; 
				if(SyncData->ApplySlots.IsValidIndex(n))
					Targetidx = (int32)SyncData->ApplySlots[n];

				if(Targetidx != INDEX_NONE)
					SkillSlotList[Targetidx]->CloseSlot();
			}
		}
	}
	else
	{
		int32 applierCount = SyncData->Appliers.Num();
		if (0 < applierCount)
		{
			SkillComponent->ApplyInstanceApplierGroup(InCaster, OwnerActor.Get(), SkillTableInfo, InIsCasterSlot, AppliedSkill);
		}
	}
}
