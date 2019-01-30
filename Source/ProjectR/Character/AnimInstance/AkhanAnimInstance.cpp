// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AkhanAnimInstance.h"
#include "Animation/AnimMontage.h" //by danzang
#include "Character/AnimInstance/AnimInstanceDefs.h"

#include "UtilFunctionIntegrated.h"

void UAkhanAnimInstance::PlayAnimMontageNormalAttack()
{
	Super::PlayAnimMontageNormalAttack();
	return;
}

void UAkhanAnimInstance::PlayAnimMontageAbnormal(EAbonormalStateFlagEnum NewFlag, EAbonormalStateFlagEnum OldFlag)
{
	if (FlyState)
	{
		AActor* OwnerCharacter = GetOwnerCharacter();
		if (IsValid(OwnerCharacter) == false)
			return;

		if (AnimState != EAnimStateEnum::VE_Abnormal)
			return;

		FName SectionName;
		if (EAbonormalStateFlagEnum::BM_Stun == NewFlag || EAbonormalStateFlagEnum::BM_Down == NewFlag || EAbonormalStateFlagEnum::BM_Frozen == NewFlag)
		{
			SectionName = ANIM_MONTAGE_ABNORMAL_UNACTABLE;
		}

		if (SectionName.IsNone())
			return;

		UAnimMontage* AnimMontage = AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
		if (false == IsValid(AnimMontage))
			return;

		PlayAnimMontageToSectionName(AnimMontage, SectionName, AnimPlayRate);
	}
	else
	{
		Super::PlayAnimMontageAbnormal(NewFlag, OldFlag);
	}
}