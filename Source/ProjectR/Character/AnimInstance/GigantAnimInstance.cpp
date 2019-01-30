// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GigantAnimInstance.h"
#include "Animation/AnimMontage.h" //by danzang
#include "Character/AnimInstance/AnimInstanceDefs.h"

#include "UtilFunctionIntegrated.h"

void UGigantAnimInstance::PlayAnimMontageNormalAttack()
{
	return;
}

void UGigantAnimInstance::PlayAnimMontageAbnormal(EAbonormalStateFlagEnum NewFlag, EAbonormalStateFlagEnum OldFlag)
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return;

	if (AnimState != EAnimStateEnum::VE_Abnormal)
		return;

	FName SectionName;
	switch (NewFlag)
	{
	case EAbonormalStateFlagEnum::BM_Down:		SectionName = ANIM_MONTAGE_ABNORMAL_DOWNSTART;	break;
	case EAbonormalStateFlagEnum::BM_Stun:		SectionName = ANIM_MONTAGE_ABNORMAL_STUN;		break;
	}
	if (SectionName.IsNone())
		return;

	UAnimMontage* AnimMontage = AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
	if (false == IsValid(AnimMontage))
		return;

	PlayAnimMontageToSectionName(AnimMontage, SectionName, AnimPlayRate);
}


void UGigantAnimInstance::PlayAnimMontageFromDamageResultType(EBattleDamageResultEnum DamageResultType)
{
	return;
}
