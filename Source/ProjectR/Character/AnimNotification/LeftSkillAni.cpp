// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LeftSkillAni.h"
#include "Character/BattleBaseAnimInstance.h"


void ULeftSkillAni::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	//UE_LOG(LogBattle, Log, TEXT("Enter LeftSkillAni Received_Notify "));

	if (IsValid(BattleBaseAnimInstance))
	{
		//UE_LOG(LogBattle, Log, TEXT("Success Cast"));
		BattleBaseAnimInstance->EndSkillAniState();
	}
}