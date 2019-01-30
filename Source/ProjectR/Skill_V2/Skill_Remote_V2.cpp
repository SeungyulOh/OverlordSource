// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Skill_Remote_V2.h"
#include "Skill_V2/ContinuousSkillApplierActor.h"


bool USkill_Remote_V2::Fire()
{
	SkillState = ESkillStateEnum::VE_Fire;

	checkf(SkillResourceInfo != nullptr, TEXT("USkill_Remote_V2::Fire[SkillResourceInfo Is Empty]"));
// 	if (SkillResourceInfo == nullptr)
// 		return false;

	AttachActionEffect(Caster.Get(), ESkillStateEnum::VE_Fire);
	AttachScreenEffect(Caster.Get(), ESkillStateEnum::VE_Fire);

	return true;

	/*
	Apply Caster Applier
	*/
// 	if (EAbnormalSlotTypeEnum::VE_None != SkillInfo_V2->CasterSlotType )
// 	{
// 		CasterSkillEntityComponent->ApplySkill(this, Caster.Get(), true);
// 	}

	/*
	Check Area Skill
	*/
	if (SkillInfo_V2->GroundDuration > 0.0f)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Caster.Get();
		SpawnParams.Instigator = Cast< APawn >(Caster.Get());

		AContinuousSkillApplierActor* ApplierActor = Caster->GetWorld()->SpawnActor< AContinuousSkillApplierActor >(AContinuousSkillApplierActor::StaticClass(), TargetLocation, FRotator::ZeroRotator, SpawnParams);		
		if (ApplierActor)
		{
			ApplierActor->SetAttachedTarget(Caster.Get());
			ApplierActor->SpawnSkill( this, TargetLocation);

			ContinuousApplierActor = ApplierActor;
		}
	}
	else
	{
		GetApplyTarget();
		bool bApplyToTarget = (0 < ApplyTargetActors.Num());

		if(false == SkillResourceInfo->BulletID.IsNone())
		{
			FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
			if (nullptr == BulletTableInfo)
				return false;

			if (ESkillBulletTypeEnum::VE_Bullet == BulletTableInfo->BulletType)
			{
				CasterSkillEntityComponent->FireTargetBullet(this, Caster.Get(), Target.Get(), bApplyToTarget);
			}
			else if (ESkillBulletTypeEnum::VE_MultiBullet == BulletTableInfo->BulletType)
			{
				CasterSkillEntityComponent->FireMultiBullet(this, Caster.Get(), Target.Get(), TargetLocation, bApplyToTarget);
			}
			else if (ESkillBulletTypeEnum::VE_LocationBullet == BulletTableInfo->BulletType)
			{
				CasterSkillEntityComponent->FireLocationBullet(this, Caster.Get(), TargetLocation, bApplyToTarget);
			}
		}
		else
		{
			if (bApplyToTarget)
			{
				Apply();
			}
		}

	}

	// Not needed target skill apply.. ex) summon
	CasterSkillEntityComponent->ApplyInstanceApplierGroup(Caster.Get(), Target.Get(), SkillInfo_V2, false, this, true);
	return true;
}
