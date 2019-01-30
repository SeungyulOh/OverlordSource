// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillPassiveSlotContainer.h"
#include "GlobalIntegrated.h"

#include "Table/SkillTableInfo_V2.h"

#include "../RemoteSkillSlot.h"
#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"




USkillPassiveSlotContainer::USkillPassiveSlotContainer()
{

}

void USkillPassiveSlotContainer::Init(AActor* InOwner)
{
	OwnerActor = InOwner;

	PrepareSlots();
}

void USkillPassiveSlotContainer::PrepareSlots()
{
	SkillSlotList.Init(nullptr, 6);

	if (false == URealTimeModManager::IsRealTimeGameMode())
	{
		SkillSlotList[0] = NewObject<USkillSlot>();
		SkillSlotList[1] = NewObject<USkillSlot>();
		SkillSlotList[2] = NewObject<USkillSlot>();
		SkillSlotList[3] = NewObject<USkillSlot>();
		SkillSlotList[4] = NewObject<USkillSlot>();
		SkillSlotList[5] = NewObject<USkillSlot>();
	}
	else
	{
		SkillSlotList[0] = NewObject<URemoteSkillSlot>();
		SkillSlotList[1] = NewObject<URemoteSkillSlot>();
		SkillSlotList[2] = NewObject<URemoteSkillSlot>();
		SkillSlotList[3] = NewObject<URemoteSkillSlot>();
		SkillSlotList[4] = NewObject<URemoteSkillSlot>();
		SkillSlotList[5] = NewObject<URemoteSkillSlot>();
	}

	SkillSlotList[0]->SetSlotType(EAbnormalSlotTypeEnum::VE_Slot_Passive);
	SkillSlotList[1]->SetSlotType(EAbnormalSlotTypeEnum::VE_Slot_Passive_01);
	SkillSlotList[2]->SetSlotType(EAbnormalSlotTypeEnum::VE_Slot_Passive_02);
	SkillSlotList[3]->SetSlotType(EAbnormalSlotTypeEnum::VE_Slot_Passive_03);
	SkillSlotList[4]->SetSlotType(EAbnormalSlotTypeEnum::VE_Slot_Passive_04);
	SkillSlotList[5]->SetSlotType(EAbnormalSlotTypeEnum::VE_Slot_Passive_05);
}

void USkillPassiveSlotContainer::Update(float DeltaTime)
{	
	for (auto SkilSlot : SkillSlotList)
	{
		SkilSlot->Update(DeltaTime);
	}
}

void USkillPassiveSlotContainer::AddPassiveSkillSlot(UISkill_V2* AppliedSkill, AActor* InCaster )
{
	checkf(InCaster != nullptr, TEXT("USkillPassiveSlotContainer::AddPassiveSkillSlot[InCaster Is Empty]"));
	IEntityBaseProperty* CasterEntityBaseProperty = Cast< IEntityBaseProperty >(InCaster);
	if (CasterEntityBaseProperty == nullptr)
		return;

	FName SkillTableID = AppliedSkill->GetSkillTableID();

	int32 TargetIdx = (int32)AppliedSkill->GetSkillCategoryType() - (int32)ESkillCategoryTypeEnum::VE_PassiveSkill01;
	if (SkillSlotList.IsValidIndex(TargetIdx))
	{
		if(!SkillSlotList[TargetIdx]->IsActivated())
			SkillSlotList[TargetIdx]->OpenSlot(SkillTableID, true, OwnerActor.Get(), OwnerActor.Get(), AppliedSkill);
	}
}

void USkillPassiveSlotContainer::AddItemSkillSlot(UISkill_V2* AppliedSkill)
{
	checkf(AppliedSkill != nullptr, TEXT("USkillPassiveSlotContainer::AddPassiveSkillSlot[AppliedSkill Is Empty]"));
	FName SkillTableID = AppliedSkill->GetSkillTableID();
	int32 SlotIndex = GetSlotArrayIndex();
	if (SkillSlotList.IsValidIndex(SlotIndex))
	{
		// Caster applier
		SkillSlotList[SlotIndex]->OpenSlot(SkillTableID, true, OwnerActor.Get(), OwnerActor.Get(), AppliedSkill);
	}
}

bool USkillPassiveSlotContainer::GetSlotThatContainAppliers(ESkillApplierTypeEnum InApplierType, TArray< USkillSlot* >& OutSkillSlots, bool CheckRate)
{
	bool bResult = false;
	for (auto SkillSlot : SkillSlotList)
	{
		FSkillApplierBase* Applier = SkillSlot->ContainApplier(InApplierType);
		if (Applier)
		{
			if (CheckRate)
			{
				int32 rand_val = FMath::RandRange(0, 100);
				if (rand_val <= Applier->GetIntPercent())
				{
					OutSkillSlots.Emplace(SkillSlot);
					bResult = true;
				}
			}
			else
			{
				OutSkillSlots.Emplace(SkillSlot);
				bResult = true;
			}
		}
	}

	return bResult;
}

int32 USkillPassiveSlotContainer::GetSlotArrayIndex()
{
	for(int32 n = 0; n < SkillSlotList.Num(); ++n)
	{
		if (false == SkillSlotList[n]->IsActivated())
		{
			return n;
		}
	}

	USkillSlot* newSlot = NewObject<USkillSlot>();
	newSlot->SetSlotType(EAbnormalSlotTypeEnum::VE_Slot_Passive);
	SkillSlotList.Add(newSlot);
	int32 resultIndex = SkillSlotList.Num() - 1;
	return resultIndex;
}

bool USkillPassiveSlotContainer::GatherPersistentAppilerValue(ESkillApplierTypeEnum InSkillApplierType, FSkillApplierBase& OutSkillApplier)
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


TArray< USkillSlot* >& USkillPassiveSlotContainer::GetAllSlots()
{
	return SkillSlotList;
}

void USkillPassiveSlotContainer::RemoveSlot(FName& ItemSkillID)
{
	FSkillTableInfo_V2* SkillInfo = nullptr;
	for (auto SkilSlot : SkillSlotList)
	{
		if (0 == ItemSkillID.Compare(SkilSlot->GetSkillID()))
		{
			SkilSlot->CloseSlot();
			SkilSlot->BeginDestroy();
			SkilSlot = nullptr;
		}
	}
}

void USkillPassiveSlotContainer::RemoveAllSlots()
{
	for (auto SkilSlot : SkillSlotList)
	{
		if (SkilSlot->IsActivated())
		{
			SkilSlot->SetForceDetach();
			SkilSlot->CloseSlot();
		}
	}

	SkillSlotList.Empty();
}

void USkillPassiveSlotContainer::AddOneTimeValidApplier(ESkillApplierTypeEnum InApplier, USkillSlot* SkillSlot)
{
	OneTimeValidApplier.Emplace(InApplier, SkillSlot);
}

bool USkillPassiveSlotContainer::ContainOneTimeValidApplier(ESkillApplierTypeEnum InApplier, bool CheckRate)
{
	if (CheckRate)
	{
		if (OneTimeValidApplier.Contains(InApplier))
		{
			USkillSlot* SkillSlot = OneTimeValidApplier[InApplier];
			if (SkillSlot && SkillSlot->IsActivated())
			{
				FSkillApplierBase* Applier = SkillSlot->ContainApplier(InApplier);
				if (Applier)
				{
					int32 rand_val = FMath::RandRange(0, 100);
					return (rand_val <= Applier->GetIntPercent());
				}
			}
		}
	}
	else
	{
		if (OneTimeValidApplier.Contains(InApplier))
		{
			return true;
		}
	}
	return false;
}

void USkillPassiveSlotContainer::ClearOneTimeValidApplier(ESkillApplierTypeEnum InApplier)
{
	if (OneTimeValidApplier.Contains(InApplier))
	{
		OneTimeValidApplier.Remove(InApplier);
	}
}
