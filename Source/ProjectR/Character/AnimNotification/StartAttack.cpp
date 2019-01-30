// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "StartAttack.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/BattleBaseAnimInstance.h"

void UStartAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	//UE_LOG(LogBattle, Log, TEXT("Enter StartAttack Received_Notify "));

	if (IsValid(BattleBaseAnimInstance))
	{
		//UE_LOG(LogBattle, Log, TEXT("Success Cast"));
		BattleBaseAnimInstance->BeginAttackState(ENormalAttackTypeEnum::VE_NormalAttackNone);
	}
}