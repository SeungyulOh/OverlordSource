// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EnterSkillAni.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleObjectAnimInstance.h"



void UEnterSkillAni::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
#if LOG_SKILL
	UE_LOG(LogBattle, Log, TEXT("Enter EnterSkillAni Received_Notify "));
#endif

	if (IsValid(BattleBaseAnimInstance))
	{
		//UE_LOG(LogBattle, Log, TEXT("Success Cast"));
		BattleBaseAnimInstance->BeginSkillAniState();
	}

	UBattleObjectAnimInstance* BattleObjectAnimInstance = Cast<UBattleObjectAnimInstance>(AnimInstance);
	if (IsValid(BattleObjectAnimInstance))
	{
		BattleObjectAnimInstance->BeginSkillAniState();
	}
}