// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "AnimSetSkillAniState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleObjectAnimInstance.h"

void UAnimSetSkillAniState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimSetSkillAniState Begin"));
	UE_LOG(LogBattle, Log, TEXT(">>>>>>  Enter Skill Ani Received_Notify - %s"), *(Animation->GetName()));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	if (IsValid(BattleBaseAnimInstance))
	{
		CurrentSkillAniType = BattleBaseAnimInstance->SkillAniType;
		BattleBaseAnimInstance->BeginSkillAniState();
	}
	UBattleObjectAnimInstance* BattleObjectAnimInstance = Cast<UBattleObjectAnimInstance>(AnimInstance);
	if (IsValid(BattleObjectAnimInstance))
	{
		BattleObjectAnimInstance->BeginSkillAniState();
	}
}

void UAnimSetSkillAniState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimSetSkillAniState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimSetSkillAniState End"));
	UE_LOG(LogBattle, Log, TEXT(">>>>>>  End Skill Ani Received_Notify - %s"), *(Animation->GetName()));
#endif	

	if (IsValid(BattleBaseAnimInstance))
	{
		if (CurrentSkillAniType == BattleBaseAnimInstance->SkillAniType)
		{
			//UE_LOG(LogBattle, Log, TEXT("Success Cast"));
			BattleBaseAnimInstance->EndSkillAniState();
		}
	}

	UBattleObjectAnimInstance* BattleObjectAnimInstance = Cast<UBattleObjectAnimInstance>(AnimInstance);
	if (IsValid(BattleObjectAnimInstance))
	{
		BattleObjectAnimInstance->EndSkillAniState();
	}

}