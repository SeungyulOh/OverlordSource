// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimSkillApplyState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleObjectAnimInstance.h"

void UAnimSkillApplyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimSkillApplyState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->BeginSkillApply();
	}
	
	UBattleObjectAnimInstance* BattleObjectAnimInstance = Cast<UBattleObjectAnimInstance>(AnimInstance);
	if (IsValid(BattleObjectAnimInstance))
	{
		BattleObjectAnimInstance->BeginSkillApply();
	}
}

void UAnimSkillApplyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimSkillApplyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimSkillApplyState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->EndSkillApply();
	}

	UBattleObjectAnimInstance* BattleObjectAnimInstance = Cast<UBattleObjectAnimInstance>(AnimInstance);
	if (IsValid(BattleObjectAnimInstance))
	{
		BattleObjectAnimInstance->EndSkillApply();
	}
}