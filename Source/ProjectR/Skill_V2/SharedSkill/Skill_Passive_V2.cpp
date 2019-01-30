// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Skill_Passive_V2.h"
#include "Skill_V2/SharedSkill/SkillPassiveSlotContainer.h"
#include "Skill_V2/ContinuousSkillApplierActor.h"

#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "Table/ItemTableInfo.h"		// for FRuneWordSkillTable
#include "Table/TowerSkillTableInfo.h"
#include "UtilFunctionIntegrated.h"

bool USkill_Passive_V2::AttachPassiveSkill()
{
	//if (SkillResourceInfo == nullptr)
	//	return false;
	
	/*
	Apply Caster Applier
	Passive skills don't have slot for every skills.
	*/	
	if (CasterSkillEntityComponent->GetPassiveSkillSlotContainer())
	{
		CasterSkillEntityComponent->GetPassiveSkillSlotContainer()->AddPassiveSkillSlot(this, Caster.Get());
	}	

	/*
	Check Aura Skill
	*/
	if(SkillInfo_V2 && SkillInfo_V2->SkillControlType == ESkillControlTypeEnum::VE_Aura )
	{
		//if (SkillResourceInfo)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Caster.Get();
			SpawnParams.Instigator = Cast< APawn >(Caster.Get());

			AContinuousSkillApplierActor* ApplierActor = Caster->GetWorld()->SpawnActor< AContinuousSkillApplierActor >(AContinuousSkillApplierActor::StaticClass(), Caster->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

			if (ApplierActor)
			{
				ApplierActor->SetAttachedTarget(Caster.Get());
				ApplierActor->SpawnSkill(this, Caster->GetActorLocation());

				ContinuousApplierActor = ApplierActor;
			}
		}
	}

	return true;
}


bool USkill_Passive_V2::FireCoditionCheckSkill(bool bCasterSlot /*= false*/, bool bNoneTarget /*= true*/)
{
	if (CurrentCoolTime > 0.0f)
		return false;

	if (bApplying)
		return false;

	bApplying = true;

	StartCoolTime();

	if (SkillResourceInfo != nullptr)
	{
		AttachActionEffect(Caster.Get(), ESkillStateEnum::VE_Fire);
		AttachScreenEffect(Caster.Get(), ESkillStateEnum::VE_Fire);
	}

	SkillState = ESkillStateEnum::VE_Fire;

	CasterSkillEntityComponent->OnNotifySkillCasting.Broadcast(Caster.Get(), this);
	/*
	Check Area Skill
	*/
// 	if (SkillInfo_V2->SummonTime > 0.0f)
// 	{
// 		FActorSpawnParameters SpawnParams;
// 		SpawnParams.Owner = Caster.Get();
// 		SpawnParams.Instigator = Cast< APawn >(Caster.Get());
// 
// 		AContinuousSkillApplierActor* ApplierActor = Caster->GetWorld()->SpawnActor< AContinuousSkillApplierActor >(AContinuousSkillApplierActor::StaticClass(), TargetLocation, FRotator::ZeroRotator, SpawnParams);
// 		if (ApplierActor)
// 		{
// 			ApplierActor->SetAttachedTarget(Caster.Get());
// 			ApplierActor->SpawnSkill(this, TargetLocation);
// 
// 			ContinuousApplierActor = ApplierActor;
// 		}
// 	}
// 	else
// 	{
		GetApplyTarget();
		bool bApplyToTarget = (0 < ApplyTargetActors.Num());

		if (bApplyToTarget)
		{
			Apply();
		}
	//}

	// Not needed target skill apply.. ex) summon
	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
	if (!IsValid(SkillComponent))
		return false;

	SkillComponent->ApplyInstanceApplierGroup(Caster.Get(), Target.Get(), SkillInfo_V2, bCasterSlot, this, bNoneTarget);

	ApplyTargetActors.Empty();
	SkillSyncData.Clear();

	bApplying = false;

	return true;
}

