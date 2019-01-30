// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EndSkillAni.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleObjectAnimInstance.h"

void UEndSkillAni::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

#if LOG_SKILL
	UE_LOG(LogBattle, Log, TEXT("Enter End Skill Ani Received_Notify "));
#endif

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->EndSkillAniState();
	}

	UBattleObjectAnimInstance* BattleObjectAnimInstance = Cast<UBattleObjectAnimInstance>(AnimInstance);
	if (IsValid(BattleObjectAnimInstance))
	{
		BattleObjectAnimInstance->EndSkillAniState();
	}

}