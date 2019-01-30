// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RemoteSkillSlot.h"
#include "GlobalIntegrated.h"



#include "Table/SkillTableInfo_V2.h"
#include "Table/AbnormalStateGroupTableInfo_V2.h"

// #include "Utils/AIFunctionLibrary.h"



#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SkillApplierBase.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillPassiveSlotContainer.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/DamageFloater/SlateDamageFloaterComponentV2.h"
#include "Character/Components/CustomChangeMaterialComponent.h"
#include "Character/Components/SpecialMovingComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/AnimInstance/AnimInstanceDefs.h"

#include "Character/SharedBattle/BattleOperator.h"

#include "CustomStructures/SharedStructures/ProtectInfo.h"



URemoteSkillSlot::URemoteSkillSlot() : USkillSlot() { ; }

bool URemoteSkillSlot::Update(float DeltaTime)
{
	if (bActivated && Duration > 0)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= (Duration + 2.0f))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("CloseSlot called by Client"));
			CloseSlot();
			return false;
		}
	}

	return true;
}

bool URemoteSkillSlot::OpenSlot(FName InSkillID, bool InIsCaster, AActor* InOwner, AActor* InCaster, UISkill_V2* AppliedSkill)
{
	IsMyOwnerActor = (false == UCharacterHelpFunctionLibrary::IsRemoteEntity(InOwner));

	return Super::OpenSlot(InSkillID, InIsCaster, InOwner, InCaster, AppliedSkill);
}

void URemoteSkillSlot::SetApplierInfo()
{
	checkf(OwnerActor.IsValid(), TEXT("URemoteSkillSlot::SetApplierInfo[OwnerActor Is Empty]"));
// 	if (false == OwnerActor.IsValid())
// 		return;

	SkillApplierList.Empty();
	IEntityBaseProperty* CasterEntityBaseProperty = Cast< IEntityBaseProperty >(CasterActor.Get());
	if (CasterEntityBaseProperty == nullptr)
		return;

	FSkillTableInfo_V2* SkillTableInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(CasterEntityBaseProperty->GetCharacterCategory(), SkillTableID);
	checkf(SkillTableInfo, TEXT("URemoteSkillSlot::SetApplierInfo[SkillTableInfo Is Empty]"));

	IntervalTime = SkillTableInfo->Interval;

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

	int32 ApplierInfoIndex = -1;
	CurrentSyncApplierData = nullptr;
	if (TargetSkillSyncData)
	{
		int32 applierCount = TargetSkillSyncData->Appliers.Num();
		if (0 < applierCount)
		{
			FSkillApplierBase SkillApplier;
			for (int32 n = 0; n < applierCount; ++n)
			{
				CurrentSyncApplierData = &(TargetSkillSyncData->Appliers[n]);

				if (false == CurrentSyncApplierData->Remove)
				{
					ApplierInfoIndex = -1;
					for (int32 k = 0; k < 6; ++k)
					{
						if (ApplierTypes[k] == CurrentSyncApplierData->SlotApplier.SkillApplierType)
						{
							ApplierInfoIndex = k;
						}
					}

					if (0 <= ApplierInfoIndex)
					{
						SkillApplier.SkillApplierType = ApplierTypes[ApplierInfoIndex];
						SkillApplier.TargetPercent = Percents[ApplierInfoIndex];
						SkillApplier.TargetValue = Values[ApplierInfoIndex];

						if (AttachPersistentApplier(SkillApplier, true))
						{
							SkillApplierList.Add(SkillApplier);
						}
						else
						{
							USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(CasterActor.Get());
							if(IsValid(SkillComponent))
								SkillComponent->CS_ApplyInstanceApplier(CasterActor.Get(), OwnerActor.Get(), TargetSkillSyncData->Appliers[n], SkillApplier, SkillInst);
						}
					}
				}
			}
		}
	}
	CurrentSyncApplierData = nullptr;
}

