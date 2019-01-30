// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "HitTargetDummy.h"
#include "Character/BattleBaseAnimInstance.h"

void UHitTargetDummy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->HitTargetDummy();
	}
}