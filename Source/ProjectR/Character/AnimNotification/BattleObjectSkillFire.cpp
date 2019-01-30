// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BattleObjectSkillFire.h"
#include "Character/BattleObject/BattleBaseObject.h"



void UBattleObjectSkillFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			ABattleBaseObject* battleObject = Cast<ABattleBaseObject>(Owner);
			if (IsValid(battleObject))
			{
				battleObject->SkillFire();
			}
		}
	}
}